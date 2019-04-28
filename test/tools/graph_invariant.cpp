
//  Copyright (c) 2014 - 2019 Maksym V. Bilinets.
//
//  This file is part of Dataflow++.
//
//  Dataflow++ is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Dataflow++ is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with Dataflow++. If not, see <http://www.gnu.org/licenses/>.

#include "graph_invariant.h"

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <map>
#include <vector>

using namespace dataflow;

namespace
{
using vertex_descriptor =
  boost::graph_traits<introspect::dependency_graph>::vertex_descriptor;
using edge_descriptor =
  boost::graph_traits<introspect::dependency_graph>::edge_descriptor;

vertex_descriptor find_activator(vertex_descriptor v,
                                 const introspect::dependency_graph& g)
{
  assert(introspect::active_node(v));

  const auto es = out_edges(v, g);

  const auto ei =
    std::find_if(es.first, es.second, introspect::logical_dependency);

  if (ei == es.second)
    return vertex_descriptor();

  return target(*ei, g);
}

vertex_descriptor implied_activator(vertex_descriptor u,
                                    vertex_descriptor v,
                                    const introspect::dependency_graph& g)
{
  assert(introspect::active_node(u));
  assert(introspect::active_node(v));

  if (introspect::conditional_node(u) && !introspect::activator_node(v))
  {
    const auto es = out_edges(u, introspect::graph());

    const auto ei =
      std::find_if(es.first, es.second, introspect::primary_dependency);

    assert(ei != es.second);

    return target(*ei, introspect::graph());
  }

  return find_activator(u, g);
}

class validation_dfs_visitor : public boost::default_dfs_visitor
{
public:
  validation_dfs_visitor(std::vector<edge_descriptor>& bad_edges,
                         std::vector<vertex_descriptor>& bad_vertices)
  : bad_edges_(bad_edges)
  , bad_vertices_(bad_vertices)
  {
  }

  template <typename G> void discover_vertex(vertex_descriptor v, const G&)
  {
    assert(introspect::active_node(v));

    const auto vs = introspect::consumers(v);

    if (vs.first != vs.second)
    {
      std::vector<vertex_descriptor> potential_activators;

      std::transform(vs.first,
                     vs.second,
                     std::back_inserter(potential_activators),
                     [v](vertex_descriptor u) {
                       return implied_activator(u, v, introspect::graph());
                     });

      const auto vi =
        std::min_element(potential_activators.begin(),
                         potential_activators.end(),
                         [](vertex_descriptor u, vertex_descriptor w) {
                           return introspect::update_order(u, w) > 0;
                         });

      assert(vi != potential_activators.end());

      if (find_activator(v, introspect::graph()) != *vi)
      {
        bad_vertices_.push_back(v);
      }
    }
    else
    {
      if (!introspect::eager_node(v))
        bad_vertices_.push_back(v);
    }
  }

  template <typename G> void tree_edge(edge_descriptor e, const G& g)
  {
    assert(introspect::active_dependency(e));

    if (introspect::update_order(source(e, g), target(e, g)) >= 0)
    {
      bad_edges_.push_back(e);
    }
  }

  template <typename G> void back_edge(edge_descriptor e, const G& g)
  {
    assert(introspect::active_dependency(e));

    bad_edges_.push_back(e);
  }

private:
  std::vector<edge_descriptor>& bad_edges_;
  std::vector<vertex_descriptor>& bad_vertices_;
};
} // anonymous

bool dataflow_test::graph_invariant_holds(const introspect::dependency_graph& g)
{
  std::map<introspect::dependency_graph::vertex_descriptor,
           boost::default_color_type>
    color_map;

  std::vector<edge_descriptor> bad_edges;
  std::vector<vertex_descriptor> bad_vertices;

  boost::depth_first_search(
    boost::make_filtered_graph(
      g, introspect::active_dependency, introspect::active_node),
    validation_dfs_visitor(bad_edges, bad_vertices),
    boost::make_assoc_property_map(color_map));

  if (!bad_edges.empty() || !bad_vertices.empty())
  {
    return false;
  }

  return true;
}
