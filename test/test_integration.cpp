
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

namespace
{
int reference_f(int x)
{
  const auto abs = x >= 0 ? x : -x;

  const auto g = 4 - (x >= 0 ? x * x : 2 * abs);

  return g >= 0 ? g : 2 - abs;
}
}

BOOST_AUTO_TEST_SUITE(test_integration)

BOOST_AUTO_TEST_CASE(test_hat_function)
{
  for (int i = -4; i <= 4; ++i)
  {
    for (int j = -4; j <= 4; ++j)
    {
      Engine engine;

      auto x = Var<int>(i);

      auto f = Main([x](const Time& t)
                    {
                      auto abs = If(x >= 0, x, -x);

                      auto g = 4 - If(x >= 0, x * x, 2 * abs);

                      return If(g >= 0, g, 2 - abs);
                    });

      BOOST_TEST(f() == reference_f(i));
      BOOST_TEST(graph_invariant_holds());

      x = j;

      BOOST_TEST(f() == reference_f(j));
      BOOST_TEST(graph_invariant_holds());
    }
  }
}

BOOST_AUTO_TEST_CASE(regression_test_eager_node_deactivation)
{
  Engine engine;

  const var<int> x = Var<int>(6);
  const ref<int> y = ++x;

  BOOST_CHECK(introspect::active_node(y) == false);

  const auto a = Curr(y);

  BOOST_CHECK(introspect::active_node(y) == true);

  BOOST_CHECK_EQUAL(a(), 7);

  {
    const auto b = Curr(a * 2);

    BOOST_CHECK_EQUAL(b(), 14);
  }

  BOOST_CHECK(introspect::active_node(y) == true);
}

BOOST_AUTO_TEST_CASE(regression_test_diamond_deactivation)
{
  Engine engine;

  const auto x = Var<int>(6);
  const auto y = Curr(x * x);

  BOOST_CHECK_EQUAL(y(), 36);
}

BOOST_AUTO_TEST_SUITE_END()
}
