
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

#include <dataflow/prelude/stateful.h>

#include <dataflow/prelude.h>

#include "../tools/graph_invariant.h"
#include "../tools/io_fixture.h"

#include <dataflow/introspect.h>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_stateful)

using good_sm_definition_function_types = boost::mpl::list<
  std::function<std::tuple<std::pair<ref<bool>, int>>(ref<int>)>,
  std::function<std::tuple<std::pair<ref<bool>, ref<int>>>(ref<int>)>,
  std::function<
    std::tuple<std::pair<ref<bool>, std::function<ref<int>(dtime)>>>(ref<int>)>,
  std::function<
    std::tuple<std::pair<ref<bool>, int>,
               std::pair<ref<bool>, ref<int>>,
               std::pair<ref<bool>, std::function<ref<int>(dtime)>>>(ref<int>)>,
  std::tuple<std::pair<ref<bool>, int>> (*)(ref<int>),
  std::tuple<std::pair<ref<bool>, ref<int>>> (*)(ref<int>),
  std::tuple<std::pair<ref<bool>, int>,
             std::pair<ref<bool>, ref<int>>,
             std::pair<ref<bool>, ref<int> (*)(dtime)>> (*)(ref<int>)>;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_sm_definition_function,
                              T,
                              good_sm_definition_function_types)
{
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, int>::value), true);

  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, char>::value),
                    false);
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, float>::value),
                    false);
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, void>::value),
                    false);
  BOOST_CHECK_EQUAL(
    (stateful::is_sm_definition_function<T, const char*>::value), false);
}

using bad_sm_definition_function_types = boost::mpl::list<
  std::function<std::tuple<std::pair<ref<bool>, char>>(ref<int>)>,
  std::function<std::tuple<std::pair<ref<bool>, ref<char>>>(ref<int>)>,
  std::function<std::tuple<std::pair<ref<bool>, ref<int>>>(ref<char>)>,
  std::function<std::tuple<std::pair<ref<char>, ref<int>>>(ref<int>)>,
  std::function<std::tuple<
    std::pair<ref<bool>, std::function<ref<char>(dtime)>>>(ref<int>)>,
  std::function<std::tuple<
    std::pair<ref<bool>, std::function<ref<int>(dtime)>>>(ref<char>)>,
  std::function<
    std::tuple<std::pair<ref<char>, std::function<ref<int>(dtime)>>>(ref<int>)>,
  std::function<std::tuple<std::pair<ref<bool>, ref<int>>,
                           std::pair<ref<bool>, char>>(ref<int>)>,
  std::tuple<std::pair<ref<bool>, char>> (*)(ref<int>),
  std::tuple<std::pair<ref<bool>, ref<char>>> (*)(ref<int>),
  std::tuple<std::pair<ref<bool>, ref<int>>> (*)(ref<char>),
  std::tuple<std::pair<ref<bool>, ref<int>>,
             std::pair<ref<bool>, ref<char>>> (*)(ref<int>),
  int,
  void>;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_sm_definition_function_false,
                              T,
                              bad_sm_definition_function_types)
{
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, int>::value),
                    false);
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, char>::value),
                    false);
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, float>::value),
                    false);
  BOOST_CHECK_EQUAL((stateful::is_sm_definition_function<T, void>::value),
                    false);
  BOOST_CHECK_EQUAL(
    (stateful::is_sm_definition_function<T, const char*>::value), false);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_minimal)
{
  Engine engine;

  const auto m = Main(StateMachine(0, [=](const ref<int>& sp) {
    // TODO: should be just `return Transitions()` ?;
    return Transitions(On(Const(false), Const(1)));
  }));

  BOOST_CHECK_EQUAL(*m, 0);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_num_vertices_refs)
{
  Engine engine;

  const auto t = Signal();
  const auto f = Signal();

  const auto m = Main(StateMachine(true, [=](const ref<bool>&) {
    return Transitions(On(t, Const(true)), On(f, Const(false)));
  }));

  // TODO: calculate and illustrate how many nodes are really needed
  BOOST_CHECK_EQUAL(introspect::num_vertices(), 27);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_num_vertices_fots)
{
  Engine engine;

  const auto t = Signal();
  const auto f = Signal();

  const auto m = Main(StateMachine(true, [=](const ref<bool>&) {
    return Transitions(On(t, [](dtime) { return Const(true); }),
                       On(f, [](dtime) { return Const(false); }));
  }));

  // TODO: calculate and illustrate how many nodes are really needed
  BOOST_CHECK_EQUAL(introspect::num_vertices(), 39);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_refs)
{
  Engine engine;

  const auto toggle = Signal();

  const auto light =
    Main(StateMachine(false, [=](const ref<bool>& switched_on) {
      return Transitions(On(switched_on && toggle, false),
                         On(!switched_on && toggle, true));
    }));

  BOOST_CHECK_EQUAL(*light, false);

  toggle();

  BOOST_CHECK_EQUAL(*light, true);

  toggle();

  BOOST_CHECK_EQUAL(*light, false);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_fot_ref)
{
  Engine engine;

  const auto toggle = Signal();

  const auto light = Main(StateMachine(false, [=](const ref<bool>& sp) {
    return Transitions(On(toggle, [=](dtime t0) { return !sp(t0); }),
                       On(!toggle, sp));
  }));

  BOOST_CHECK_EQUAL(*light, false);

  toggle();

  BOOST_CHECK_EQUAL(*light, true);

  toggle();

  BOOST_CHECK_EQUAL(*light, false);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_self_transition)
{
  Engine engine;

  const auto toggle = Signal();

  const auto light = Main(StateMachine(false, [=](const ref<bool>& sp) {
    return Transitions(On(toggle, [=](dtime t0) { return !sp(t0); }));
  }));

  BOOST_CHECK_EQUAL(*light, false);

  toggle();

  BOOST_CHECK_EQUAL(*light, true);

  toggle();

  BOOST_CHECK_EQUAL(*light, false);
}

BOOST_AUTO_TEST_CASE(test_StateMachine_initial_state)
{
  Engine engine;

  auto x = Var(0);

  const auto m = Main(StateMachine(x, [=](const ref<int>& sp) {
    return Transitions(On(Const(false), Const(0)));
  }));

  BOOST_CHECK_EQUAL(*m, 0);

  x = 11;

  BOOST_CHECK_EQUAL(*m, 11);

  x = 22;

  BOOST_CHECK_EQUAL(*m, 22);
}

BOOST_AUTO_TEST_CASE(test_Prev)
{
  Engine engine;

  io_fixture io;

  const ref<int> v0 = Var<int>(1);
  var<int> x = Var<int>(3);

  io.capture_output();

  const auto z = Main([=, x = x.as_ref()](dtime t0) {
    return introspect::Log(Prev(v0, x, t0), "prev");
  });

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(v0), false);
  BOOST_CHECK_EQUAL(introspect::active_node(x), true);

  BOOST_CHECK_EQUAL(io.log_string(), "[t=0] prev = 1;[t=1] prev = 3;");

  io.capture_output();

  x = 5; // t=3

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] prev = 1;[t=1] prev = 3;[t=4] prev = 5;");

  io.capture_output();

  x = 9; // t=6

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(
    io.log_string(),
    "[t=0] prev = 1;[t=1] prev = 3;[t=4] prev = 5;[t=7] prev = 9;");
}

BOOST_AUTO_TEST_CASE(test_Prev_deferred_use)
{
  Engine engine;

  io_fixture io;

  const ref<int> v0 = Var<int>(1);
  var<int> x = Var<int>(3);
  var<bool> b = Var<bool>(false);

  io.capture_output();

  const auto z = Main([=, x = x.as_ref(), b = b.as_ref()](dtime t0) {
    return If(b, introspect::Log(Prev(v0, x, t0), "prev"), 0);
  });

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(v0), false);
  BOOST_CHECK_EQUAL(introspect::active_node(x), true);

  io.capture_output();

  x = 7; // t=3

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(), "");

  io.capture_output();

  b = true; // t=5

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(), "[t=6] prev = 7;");

  io.capture_output();

  x = 5; // t=7

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(), "[t=6] prev = 7;[t=8] prev = 5;");

  io.capture_output();

  x = 9; // t=10

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=6] prev = 7;[t=8] prev = 5;[t=11] prev = 9;");
}

BOOST_AUTO_TEST_CASE(test_Prev_overloads)
{
  Engine engine;

  io_fixture io;

  var<int> x = Var<int>(3);

  io.capture_output();

  const auto z = Main([x = x.as_ref()](dtime t0) {
    return introspect::Log(Prev(x, Prev(33, x, t0), t0), "prev");
  });

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] prev = 3;[t=1] prev = 33;[t=2] prev = 3;");

  io.capture_output();

  x = 10; // t=4

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(
    io.log_string(),
    "[t=0] prev = 3;[t=1] prev = 33;[t=2] prev = 3;[t=6] prev = 10;");
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
