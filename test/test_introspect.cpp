
//  Copyright (c) 2014 - 2016 Maksym V. Bilinets.
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

#include "tools/graph_invariant.h"

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace dataflow;

namespace dataflow_test
{

namespace
{

template <typename Iterator>
static std::vector<std::string> vertex_labels(Iterator from, Iterator to)
{
  std::vector<std::string> order;

  std::transform(from, to, std::back_inserter(order), introspect::label);

  return order;
}

template <typename Iterator>
static std::vector<std::string> vertex_labels(std::pair<Iterator, Iterator> vs)
{
  return vertex_labels(vs.first, vs.second);
}

} // namespace

class test_introspect_fixture
{
public:
  using dependency_graph = introspect::dependency_graph;
  using edge_descriptor =
    boost::graph_traits<dependency_graph>::edge_descriptor;
  using vertex_descriptor =
    boost::graph_traits<dependency_graph>::vertex_descriptor;

private:
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<dependency_graph>));
  BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<dependency_graph>));

private:
  dataflow::Engine engine_;
};

BOOST_AUTO_TEST_SUITE(test_introspect)

BOOST_FIXTURE_TEST_CASE(test_incidence, test_introspect_fixture)
{
  const auto x = Var<int>(42);
  const auto y = x + x;
  const auto z = -y;

  const auto& g = introspect::graph();

  BOOST_CHECK_EQUAL(0, introspect::out_degree(x));
  BOOST_CHECK_EQUAL(2, out_degree(y, g));
  BOOST_CHECK_EQUAL(1, out_degree(z, g));

  const auto x_es = out_edges(x, g);
  const auto y_es = introspect::out_edges(y);
  const auto z_es = introspect::out_edges(z);

  BOOST_CHECK_EQUAL(0, std::distance(x_es.first, x_es.second));
  BOOST_CHECK_EQUAL(2, std::distance(y_es.first, y_es.second));
  BOOST_CHECK_EQUAL(1, std::distance(z_es.first, z_es.second));
}

BOOST_FIXTURE_TEST_CASE(test_vertex_list, test_introspect_fixture)
{
  const auto x = Var<int>(42);
  const auto y = -(x + x);

  const auto& g = introspect::graph();

  BOOST_CHECK_EQUAL(3, introspect::num_vertices());
  BOOST_CHECK_EQUAL(3, num_vertices(g));

  boost::graph_traits<introspect::dependency_graph>::vertex_iterator from, to;
  std::tie(from, to) = vertices(g);

  BOOST_CHECK_EQUAL(3, std::distance(from, to));

  std::tie(from, to) = introspect::vertices();

  BOOST_CHECK_EQUAL(3, std::distance(from, to));
}

BOOST_FIXTURE_TEST_CASE(test_with_boost_topological_sort,
                        test_introspect_fixture)
{
  const auto x = Var<int>(42);

  const auto y = If(x < 0, -x, 2 * x + 1);

  const auto z = *y;

  const auto g = boost::make_filtered_graph(
    introspect::graph(), introspect::is_dependency_active, introspect::varying);

  std::map<vertex_descriptor, boost::default_color_type> cm;

  std::vector<vertex_descriptor> order;
  boost::topological_sort(g,
                          std::back_inserter(order),
                          boost::color_map(boost::make_assoc_property_map(cm)));

  const auto labels = vertex_labels(order.begin(), order.end());

  std::vector<std::string> expected_labels(
    {"var", "<", "activator", "*", "+", "if", "curr"});

  BOOST_CHECK_EQUAL_COLLECTIONS(labels.begin(),
                                labels.end(),
                                expected_labels.begin(),
                                expected_labels.end());
}

BOOST_FIXTURE_TEST_CASE(test_topological_order, test_introspect_fixture)
{
  const auto x = Var<int>(-42);

  const auto y = If(x < 0, -x, 2 * x + 1);

  boost::graph_traits<introspect::dependency_graph>::vertex_iterator it, it_end;

  std::tie(it, it_end) = introspect::topological_order();

  BOOST_CHECK_EQUAL(0, std::distance(it, it_end));

  {
    const auto z = *y;

    std::tie(it, it_end) = introspect::topological_order();

    BOOST_CHECK_EQUAL(6, std::distance(it, it_end));

    BOOST_TEST(vertex_labels(introspect::topological_order()) ==
                 std::vector<std::string>(
                   {"var", "<", "activator", "(-)", "if", "curr"}),
               boost::test_tools::per_element());

    BOOST_TEST(graph_invariant_holds());

    x = 1;

    std::tie(it, it_end) = introspect::topological_order();

    BOOST_CHECK_EQUAL(7, std::distance(it, it_end));

    BOOST_TEST(vertex_labels(introspect::topological_order()) ==
                 std::vector<std::string>(
                   {"var", "<", "activator", "*", "+", "if", "curr"}),
               boost::test_tools::per_element());

    BOOST_TEST(graph_invariant_holds());
  }

  std::tie(it, it_end) = introspect::topological_order();

  BOOST_CHECK_EQUAL(0, std::distance(it, it_end));
}

BOOST_FIXTURE_TEST_CASE(test_nodes_traversal, test_introspect_fixture)
{
  BOOST_CHECK_EQUAL(0, introspect::num_vertices());

  boost::graph_traits<introspect::dependency_graph>::vertex_iterator it, it_end;

  std::tie(it, it_end) = introspect::vertices();

  BOOST_CHECK_EQUAL(0, std::distance(it, it_end));

  {
    const auto x = Var<int>(-42);

    const auto y = If(x < 0, -x, x);

    BOOST_CHECK_EQUAL(6, introspect::num_vertices());

    std::tie(it, it_end) = introspect::vertices();

    BOOST_CHECK_EQUAL(6, std::distance(it, it_end));
  }

  BOOST_CHECK_EQUAL(0, introspect::num_vertices());

  std::tie(it, it_end) = introspect::vertices();

  BOOST_CHECK_EQUAL(0, std::distance(it, it_end));
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
