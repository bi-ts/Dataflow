
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

#include <dataflow/prelude/logical.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_logical)

// Logical negation (`not`)

BOOST_AUTO_TEST_CASE(test_Not)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = Not(x);

  BOOST_CHECK_EQUAL(introspect::label(a), "not");

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (!true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (!false));
}

// Logical `and` (lazy)

BOOST_AUTO_TEST_CASE(test_And_1_arg)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = And(x);

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), true);

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);

  x = false;

  BOOST_CHECK_EQUAL(f(), false);

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
}

BOOST_AUTO_TEST_CASE(test_And)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = And(x, y);

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
}

BOOST_AUTO_TEST_CASE(test_And_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = And(false, y);

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false && true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));
}

BOOST_AUTO_TEST_CASE(test_And_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = And(x, true);

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));
}

BOOST_AUTO_TEST_CASE(test_And_4_mixed_arguments)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = And(x, true, y, true);

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true && true && true && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true && true && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && true && false && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && true && false && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
}

// Logical `or` (lazy)

BOOST_AUTO_TEST_CASE(test_Or)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = Or(x, y);

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
}

BOOST_AUTO_TEST_CASE(test_Or_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = Or(false, y);

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false || true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));
}

BOOST_AUTO_TEST_CASE(test_Or_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = Or(x, true);

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));
}

// Logical `and` (eager)

BOOST_AUTO_TEST_CASE(test_AndE)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = AndE(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "&");

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));
}

BOOST_AUTO_TEST_CASE(test_AndE_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = AndE(false, y);

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false && true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));
}

BOOST_AUTO_TEST_CASE(test_AndE_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = AndE(x, true);

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));
}

// Logical `or` (eager)

BOOST_AUTO_TEST_CASE(test_OrE)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = OrE(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "|");

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));
}

BOOST_AUTO_TEST_CASE(test_OrE_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = OrE(false, y);

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false || true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));
}

BOOST_AUTO_TEST_CASE(test_OrE_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = OrE(x, true);

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));
}

// Logical negation (`not`)

BOOST_AUTO_TEST_CASE(test_Not_operator)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = !x;

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (!true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (!false));
}

// Logical `and` (lazy)

BOOST_AUTO_TEST_CASE(test_And_operator)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = x && y;

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
}

BOOST_AUTO_TEST_CASE(test_And_operator_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = false && y;

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false && true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));
}

BOOST_AUTO_TEST_CASE(test_And_operator_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = x && true;

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));
}

// Logical `or` (lazy)

BOOST_AUTO_TEST_CASE(test_Or_operator)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = x || y;

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || false));

  BOOST_CHECK_EQUAL(introspect::active_node(x), true);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
}

BOOST_AUTO_TEST_CASE(test_Or_operator_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = false || y;

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false || true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));
}

BOOST_AUTO_TEST_CASE(test_Or_operator_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = x || true;

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));
}

// Logical `and` (eager)

BOOST_AUTO_TEST_CASE(test_AndE_operator)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = x & y;

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));
}

BOOST_AUTO_TEST_CASE(test_AndE_operator_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = false & y;

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false && true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false && false));
}

BOOST_AUTO_TEST_CASE(test_AndE_operator_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = x & true;

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true && true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false && true));
}

// Logical `or` (eager)

BOOST_AUTO_TEST_CASE(test_OrE_operator)
{
  Engine engine;

  const auto x = Var<bool>();
  const auto y = Var<bool>();

  const auto a = x | y;

  const auto f = Curr(a);

  x = true;

  y = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || false));

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));
}

BOOST_AUTO_TEST_CASE(test_OrE_operator_lhs_literal)
{
  Engine engine;

  const auto y = Var<bool>();

  const auto a = false | y;

  const auto f = Curr(a);

  y = true;

  BOOST_CHECK_EQUAL(f(), (false || true));

  y = false;

  BOOST_CHECK_EQUAL(f(), (false || false));
}

BOOST_AUTO_TEST_CASE(test_OrE_operator_rhs_literal)
{
  Engine engine;

  const auto x = Var<bool>();

  const auto a = x | true;

  const auto f = Curr(a);

  x = true;

  BOOST_CHECK_EQUAL(f(), (true || true));

  x = false;

  BOOST_CHECK_EQUAL(f(), (false || true));
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
