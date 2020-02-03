
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#include <dataflow/introspect.h>

#include "tools/graph_invariant.h"
#include "tools/io_fixture.h"

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
  Engine engine_;
};

BOOST_AUTO_TEST_SUITE(test_introspect)

BOOST_FIXTURE_TEST_CASE(test_value_int, test_introspect_fixture)
{
  auto x = Const<int>(42);

  BOOST_CHECK_EQUAL(introspect::value(x), "42");
}

BOOST_FIXTURE_TEST_CASE(test_value_bool, test_introspect_fixture)
{
  auto x = Const<bool>(false);
  auto y = Const<bool>(true);

  BOOST_CHECK_EQUAL(introspect::value(x), "false");
  BOOST_CHECK_EQUAL(introspect::value(y), "true");
}

BOOST_FIXTURE_TEST_CASE(test_value_string, test_introspect_fixture)
{
  auto x = Const<std::string>("hello");

  BOOST_CHECK_EQUAL(introspect::value(x), "\"hello\"");
}

BOOST_FIXTURE_TEST_CASE(test_incidence, test_introspect_fixture)
{
  const auto x = Var<int>(42);
  const auto y = x + x;
  const auto z = -y;

  const auto& g = introspect::graph();

  BOOST_CHECK_EQUAL(introspect::out_degree(x), 0);
  BOOST_CHECK_EQUAL(out_degree(y, g), 2);
  BOOST_CHECK_EQUAL(out_degree(z, g), 1);

  const auto x_es = out_edges(x, g);
  const auto y_es = introspect::out_edges(y);
  const auto z_es = introspect::out_edges(z);

  BOOST_CHECK_EQUAL(std::distance(x_es.first, x_es.second), 0);
  BOOST_CHECK_EQUAL(std::distance(y_es.first, y_es.second), 2);
  BOOST_CHECK_EQUAL(std::distance(z_es.first, z_es.second), 1);
}

BOOST_FIXTURE_TEST_CASE(test_vertex_list, test_introspect_fixture)
{
  const auto x = Var<int>(42);
  const auto y = -(x + x);

  const auto& g = introspect::graph();

  BOOST_CHECK_EQUAL(introspect::num_vertices(), 4);
  BOOST_CHECK_EQUAL(num_vertices(g), 4);

  boost::graph_traits<introspect::dependency_graph>::vertex_iterator from, to;
  std::tie(from, to) = vertices(g);

  BOOST_CHECK_EQUAL(std::distance(from, to), 4);

  std::tie(from, to) = introspect::vertices();

  BOOST_CHECK_EQUAL(std::distance(from, to), 4);
}

BOOST_FIXTURE_TEST_CASE(test_with_boost_topological_sort,
                        test_introspect_fixture)
{
  const auto x = Var<int>(42);

  const auto y = If(x < 0, -x, 2 * x + 1);

  const auto z = Main(y);

  const auto g = boost::make_filtered_graph(introspect::graph(),
                                            introspect::active_dependency,
                                            introspect::active_node);

  std::map<vertex_descriptor, boost::default_color_type> cm;

  std::vector<vertex_descriptor> order;
  boost::topological_sort(g,
                          std::back_inserter(order),
                          boost::color_map(boost::make_assoc_property_map(cm)));

  const auto labels = vertex_labels(order.begin(), order.end());

  std::vector<std::string> expected_labels(
    {"time", "var", "<", "if-activator", "*", "+", "if", "main"});

  BOOST_CHECK_EQUAL_COLLECTIONS(labels.begin(),
                                labels.end(),
                                expected_labels.begin(),
                                expected_labels.end());
}

BOOST_FIXTURE_TEST_CASE(test_topological_order, test_introspect_fixture)
{
  auto x = Var<int>(-42);

  const auto y = If(x < 0, -x, 2 * x + 1);

  boost::graph_traits<introspect::dependency_graph>::vertex_iterator it, it_end;

  std::tie(it, it_end) = introspect::topological_order();

  BOOST_CHECK_EQUAL(std::distance(it, it_end), 1);

  {
    const auto z = Main(y);

    std::tie(it, it_end) = introspect::topological_order();

    BOOST_CHECK_EQUAL(std::distance(it, it_end), 7);

    BOOST_TEST(vertex_labels(introspect::topological_order()) ==
                 std::vector<std::string>(
                   {"time", "var", "<", "if-activator", "(-)", "if", "main"}),
               boost::test_tools::per_element());

    BOOST_TEST(graph_invariant_holds());

    x = 1;

    std::tie(it, it_end) = introspect::topological_order();

    BOOST_CHECK_EQUAL(std::distance(it, it_end), 8);

    BOOST_TEST(
      vertex_labels(introspect::topological_order()) ==
        std::vector<std::string>(
          {"time", "var", "<", "if-activator", "*", "+", "if", "main"}),
      boost::test_tools::per_element());

    BOOST_TEST(graph_invariant_holds());
  }

  std::tie(it, it_end) = introspect::topological_order();

  BOOST_CHECK_EQUAL(std::distance(it, it_end), 1);
}

BOOST_FIXTURE_TEST_CASE(test_num_active_changed_updated_nodes,
                        test_introspect_fixture)
{
  // TODO: move this test to test_core?

  //          y
  //          |
  //       ,-(+)-.
  //       |     |
  //    ,-(-)-.  5
  //    |     |
  // ,-(*)-.  |
  // |     |  |
  // `-----+--'
  //       |
  //       x

  auto x = Var(1);

  const auto y = Main(x * x - x + 5);

  BOOST_CHECK_EQUAL(*y, 5);
  BOOST_CHECK_EQUAL(introspect::num_vertices(), 7);
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), 6);

  // TODO: should be 6 (all but const)?
  BOOST_CHECK_EQUAL(introspect::num_changed_nodes(), 5);

  BOOST_CHECK_EQUAL(introspect::num_updated_nodes(), 6);

  x = 0;

  BOOST_CHECK_EQUAL(*y, 5);
  BOOST_CHECK_EQUAL(introspect::num_vertices(), 7);
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), 6);
  BOOST_CHECK_EQUAL(introspect::num_changed_nodes(), 3);
  BOOST_CHECK_EQUAL(introspect::num_updated_nodes(), 4);

  x = 10;

  BOOST_CHECK_EQUAL(*y, 95);
  BOOST_CHECK_EQUAL(introspect::num_vertices(), 7);
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), 6);
  BOOST_CHECK_EQUAL(introspect::num_changed_nodes(), 6);
  BOOST_CHECK_EQUAL(introspect::num_updated_nodes(), 6);
}

BOOST_FIXTURE_TEST_CASE(test_nodes_traversal, test_introspect_fixture)
{
  BOOST_CHECK_EQUAL(introspect::num_vertices(), 1);

  boost::graph_traits<introspect::dependency_graph>::vertex_iterator it, it_end;

  std::tie(it, it_end) = introspect::vertices();

  BOOST_CHECK_EQUAL(std::distance(it, it_end), 1);

  {
    const auto x = Var<int>(-42);

    const auto y = If(x < 0, -x, x);

    BOOST_CHECK_EQUAL(introspect::num_vertices(), 7);

    std::tie(it, it_end) = introspect::vertices();

    BOOST_CHECK_EQUAL(std::distance(it, it_end), 7);
  }

  BOOST_CHECK_EQUAL(introspect::num_vertices(), 1);

  std::tie(it, it_end) = introspect::vertices();

  BOOST_CHECK_EQUAL(std::distance(it, it_end), 1);
}

BOOST_FIXTURE_TEST_CASE(test_update_order, test_introspect_fixture)
{
  const auto null_vertex = boost::graph_traits<dependency_graph>::null_vertex();

  const auto c = Const<int>(0);
  const auto x = Var<int>(-42);
  const auto y = If(x < 0, -x, x + c);

  BOOST_CHECK_EQUAL(introspect::update_order(x, null_vertex), 0);
  BOOST_CHECK_EQUAL(introspect::update_order(null_vertex, x), 0);
  BOOST_CHECK_EQUAL(introspect::update_order(null_vertex, null_vertex), 0);

  BOOST_CHECK_EQUAL(introspect::update_order(x, y), 0);
  BOOST_CHECK_EQUAL(introspect::update_order(x, x), 0);
  BOOST_CHECK_EQUAL(introspect::update_order(y, x), 0);

  const auto z = Main(y);

  BOOST_CHECK_EQUAL(introspect::update_order(x, y), 1);
  BOOST_CHECK_EQUAL(introspect::update_order(x, x), 0);
  BOOST_CHECK_EQUAL(introspect::update_order(y, x), -1);

  BOOST_CHECK_EQUAL(introspect::update_order(null_vertex, x), 1);
  BOOST_CHECK_EQUAL(introspect::update_order(x, null_vertex), -1);
  BOOST_CHECK_EQUAL(introspect::update_order(c, x), 1);
  BOOST_CHECK_EQUAL(introspect::update_order(x, c), -1);
}

BOOST_AUTO_TEST_CASE(test_introspect_Log)
{
  Engine engine;

  io_fixture io;

  auto x = Var<int>(11);
  auto y = introspect::Log(x + 12, "x + 12");
  auto z = y * 2;

  BOOST_CHECK_EQUAL(introspect::label(y), "introspect-log");

  io.capture_output();

  auto a = Main(z);

  io.reset_output();

  BOOST_CHECK_EQUAL(io.log_string(), "x + 12 = 23;");
  BOOST_CHECK_EQUAL(*a, 46);

  io.capture_output(true);

  x = 22;

  io.reset_output();

  BOOST_CHECK_EQUAL(io.log_string(), "x + 12 = 34;");
  BOOST_CHECK_EQUAL(*a, 68);
}

BOOST_AUTO_TEST_CASE(test_introspect_Log_default_label)
{
  Engine engine;

  io_fixture io;

  auto x = Var<int>(256);

  io.capture_output();

  auto a = Main(introspect::Log(x));

  io.reset_output();

  BOOST_CHECK_EQUAL(io.log_string(), "var = 256;");
  BOOST_CHECK_EQUAL(*a, 256);
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
