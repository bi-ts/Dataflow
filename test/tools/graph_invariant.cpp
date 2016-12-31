
//  Copyright 2014-2016 Maksym V. Bilinets.
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

vertex_descriptor find_base(vertex_descriptor v)
{
  const auto& g = introspect::graph();

  const auto es = out_edges(v, g);

  const auto ei =
    std::find_if(es.first, es.second, introspect::is_dependency_logical);

  if (ei == es.second)
    return vertex_descriptor();

  return target(*ei, g);
}

vertex_descriptor inherit_base_for(vertex_descriptor u, vertex_descriptor v)
{
  assert(u != vertex_descriptor());
  assert(v != vertex_descriptor());

  if (introspect::conditional(u) && !introspect::activator(v))
  {
    const auto es = out_edges(u, introspect::graph());

    const auto ei =
      std::find_if(es.first, es.second, introspect::is_dependency_primary);

    assert(ei != es.second);

    return target(*ei, introspect::graph());
  }

  return find_base(u);
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

  template <typename G> void discover_vertex(vertex_descriptor v, const G& g)
  {
    assert(introspect::varying(v));

    auto vs = introspect::consumers(v);

    if (vs.first != vs.second)
    {
      const auto vi =
        std::min_element(vs.first,
                         vs.second,
                         [v](vertex_descriptor u, vertex_descriptor w)
                         {
                           return introspect::preceedes(inherit_base_for(u, v),
                                                        inherit_base_for(w, v));
                         });

      assert(vi != vs.second);

      if (find_base(v) != inherit_base_for(*vi, v))
      {
        bad_vertices_.push_back(v);
      }
    }
    else
    {
      // TODO: assert(eager);
    }
  }

  template <typename G> void tree_edge(edge_descriptor e, const G& g)
  {
    assert(introspect::is_dependency_active(e));

    const auto u = source(e, g);
    const auto v = target(e, g);

    if (!introspect::preceedes(v, u))
    {
      bad_edges_.push_back(e);
    }
  }

  template <typename G> void back_edge(edge_descriptor e, const G& g)
  {
    assert(introspect::is_dependency_active(e));

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
           boost::default_color_type> color_map;

  std::vector<edge_descriptor> bad_edges;
  std::vector<vertex_descriptor> bad_vertices;

  boost::depth_first_search(
    boost::make_filtered_graph(
      g, introspect::is_dependency_active, introspect::varying),
    validation_dfs_visitor(bad_edges, bad_vertices),
    boost::make_assoc_property_map(color_map));

  if (!bad_edges.empty() || !bad_vertices.empty())
  {
    return false;
  }

  return true;
}
