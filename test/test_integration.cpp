
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
  Engine engine;

  for (int i = -4; i <= 4; ++i)
  {
    for (int j = -4; j <= 4; ++j)
    {
      auto x = Var<int>(i);

      auto abs = If(x >= 0, x, -x);

      auto g = 4 - If(x >= 0, x * x, 2 * abs);

      auto f = *If(g >= 0, g, 2 - abs);

      BOOST_TEST(f() == reference_f(i));
      BOOST_TEST(graph_invariant_holds());

      x = j;

      BOOST_TEST(f() == reference_f(j));
      BOOST_TEST(graph_invariant_holds());
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
}
