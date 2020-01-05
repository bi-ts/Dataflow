
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

#include <dataflow/prelude.h>

#include "tools/graph_invariant.h"

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_regression)

BOOST_AUTO_TEST_CASE(test_Mod)
{
  Engine engine;

  var<int> y = Var<int>(1);

  auto z = Mod(Const(60), y);

  BOOST_CHECK_EQUAL(introspect::label(z), "mod");

  auto a = Main(z);

  std::vector<int> mods(1, a());

  for (int i = 2; i < 10; ++i)
  {
    y = i;
    mods.push_back(a());
  }

  BOOST_TEST(mods == std::vector<int>({0, 0, 0, 0, 0, 0, 4, 4, 6}),
             boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(test_regression_eager_node_deactivation)
{
  Engine engine;

  const var<int> x = Var<int>(6);
  const ref<int> y = ++x;

  BOOST_CHECK(introspect::active_node(y) == false);

  const auto a = Main(y);

  BOOST_CHECK(introspect::active_node(y) == true);

  BOOST_CHECK_EQUAL(a(), 7);

  {
    const auto b = Main(a * 2);

    BOOST_CHECK_EQUAL(b(), 14);
  }

  BOOST_CHECK(introspect::active_node(y) == true);
}

BOOST_AUTO_TEST_CASE(test_regression_diamond_deactivation)
{
  Engine engine;

  const auto x = Var<int>(6);
  const auto y = Main(x * x);

  BOOST_CHECK_EQUAL(y(), 36);
}

BOOST_AUTO_TEST_CASE(test_regression_And_num_vertices)
{
  Engine engine;

  const auto a = And(Var(true), Var(true));

  BOOST_CHECK_EQUAL(introspect::num_vertices(), 6);
}

BOOST_AUTO_TEST_CASE(test_regression_Or_num_vertices)
{
  Engine engine;

  const auto a = Or(Var(true), Var(true));

  BOOST_CHECK_EQUAL(introspect::num_vertices(), 6);
}

BOOST_AUTO_TEST_CASE(test_regression_Recursion_independent_on_previous_state)
{
  Engine engine;

  auto x = Var<int>(1);

  const auto tf = [=](const ref<int>&) { return x; };

  const auto y = Main([=](dtime t0) { return Recursion(0, tf, t0); });

  BOOST_CHECK_EQUAL(y(), 1);

  x = 33;

  BOOST_CHECK_EQUAL(y(), 33);
}

BOOST_AUTO_TEST_CASE(test_regression_diamond_destruction)
{
  Engine engine;

  const auto y = []() {
    const auto x = Var<int>(6);
    return Main(x * x);
  }();

  BOOST_CHECK_EQUAL(y(), 36);
}

BOOST_AUTO_TEST_CASE(test_regression_compound_deactivation)
{
  Engine engine;

  auto x = Var<bool>(true);

  const auto y = If(
    x,
    [](dtime) { return Const<int>(100); },
    [](dtime) { return Const<int>(200); });

  const auto z = Main(y);

  BOOST_CHECK_EQUAL(z(), 100);

  x = false;

  BOOST_CHECK_EQUAL(z(), 200);
}

BOOST_AUTO_TEST_CASE(test_regression_no_bad_ref_type_conversion)
{
  BOOST_CHECK((!std::is_constructible<ref<int>, ref<std::string>>::value));
}

BOOST_AUTO_TEST_CASE(test_regression_no_bad_val_type_conversion)
{
  BOOST_CHECK((!std::is_constructible<val<int>, val<std::string>>::value));
}

BOOST_AUTO_TEST_CASE(test_regression_no_bad_var_type_conversion)
{
  BOOST_CHECK((!std::is_constructible<var<int>, var<std::string>>::value));
}

BOOST_AUTO_TEST_SUITE_END()
}
