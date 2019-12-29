
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

#include <dataflow/prelude/core.h>

#include "../../tools/graph_invariant.h"
#include "../../tools/io_fixture.h"

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_core)

BOOST_AUTO_TEST_CASE(test_dtimestamp_default_ctor)
{
  dtimestamp ts;

  BOOST_CHECK_EQUAL(ts, 0);
}

BOOST_AUTO_TEST_CASE(test_dtimestamp_ctor)
{
  Engine engine;

  auto x = Var(100);

  struct policy
  {
    static std::string label()
    {
      return ">=100";
    }
    static bool calculate(int v)
    {
      return v >= 100;
    }
  };

  const auto make_timestamp = core::Lift(policy(), x);

  // t = 0
  const val<dtimestamp> ts = Main(If(
    make_timestamp,
    [=](dtime t0) { return Const<dtimestamp>(t0); },
    [=](dtime t0) { return Const<dtimestamp>(); }));

  BOOST_CHECK_EQUAL(ts(), 0);

  // t = 1
  x = 0;

  BOOST_CHECK_EQUAL(ts(), 0);

  // t = 2
  x = 110;

  BOOST_CHECK_EQUAL(ts(), 2);

  // t = 3
  x = 10;

  BOOST_CHECK_EQUAL(ts(), 0);

  // t = 4
  x = 120;

  BOOST_CHECK_EQUAL(ts(), 4);

  // t = 5
  x = 130;

  BOOST_CHECK_EQUAL(ts(), 4);

  // t = 6
  x = 20;

  BOOST_CHECK_EQUAL(ts(), 0);

  // t = 7
  x = 140;

  BOOST_CHECK_EQUAL(ts(), 7);
}

BOOST_AUTO_TEST_SUITE_END()
}
