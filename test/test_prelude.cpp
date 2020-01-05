
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

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_prelude_header)

BOOST_AUTO_TEST_CASE(test_prelude_contains_all_submodules)
{
  Engine engine;

  const auto x = Var<int>(1);
  const auto y = Var<int>(2);

  // Includes arithmetic
  const auto a = Main((x + y));

  BOOST_CHECK_EQUAL(a(), 3);

  // Includes comparison
  const auto b = Main((x < y));

  BOOST_CHECK_EQUAL(b(), true);

  // Includes conditional
  const auto c = Main(Switch(Case(b, y), Default(0)));

  BOOST_CHECK_EQUAL(c(), 2);

  // Includes core
  const auto d = Main(Const(3));

  BOOST_CHECK_EQUAL(d(), 3);

  // Includes logical
  const auto e = Main((b && false));

  BOOST_CHECK_EQUAL(e(), false);

  // Includes stateful
  const auto f = Main([](dtime t0) {
    return StateMachine(
      0, [](const ref<int>&) { return Transitions(On(true, 1)); }, t0);
  });

  BOOST_CHECK_EQUAL(f(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
}
