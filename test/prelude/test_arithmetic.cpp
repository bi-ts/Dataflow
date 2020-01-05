
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

#include <dataflow/prelude/arithmetic.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_arithmetic)

// Absolute value

BOOST_AUTO_TEST_CASE(test_Abs_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Abs(x);

  BOOST_CHECK_EQUAL(introspect::label(a), "abs");

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), 3);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 2);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 5);
}

// Addition

BOOST_AUTO_TEST_CASE(test_Add_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>();

  const auto a = Add(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "+");

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -6);

  y = 2;

  BOOST_CHECK_EQUAL(f(), -1);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 2);

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -1);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 7);

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 2);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 7);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 10);
}

BOOST_AUTO_TEST_CASE(test_Add_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = Add(2, y);

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), -1);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 7);
}

BOOST_AUTO_TEST_CASE(test_Add_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Add(x, 2);

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -1);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 7);
}

// Subtraction

BOOST_AUTO_TEST_CASE(test_Sub_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>();

  const auto a = Sub(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "-");

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 0);

  y = 2;

  BOOST_CHECK_EQUAL(f(), -5);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -8);

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 5);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 0);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -3);

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 8);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 3);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 0);
}

BOOST_AUTO_TEST_CASE(test_Sub_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = Sub(2, y);

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), 5);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 0);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -3);
}

BOOST_AUTO_TEST_CASE(test_Sub_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Sub(x, 2);

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -5);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 0);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 3);
}

// Unary plus

BOOST_AUTO_TEST_CASE(test_Plus_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Plus(x);

  BOOST_CHECK_EQUAL(introspect::label(a), "(+)");

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -3);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 2);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 5);
}

// Additive inverse

BOOST_AUTO_TEST_CASE(test_Inv_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Inv(x);

  BOOST_CHECK_EQUAL(introspect::label(a), "(-)");

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), 3);

  x = 2;

  BOOST_CHECK_EQUAL(f(), -2);

  x = 5;

  BOOST_CHECK_EQUAL(f(), -5);
}

// Multiplication

BOOST_AUTO_TEST_CASE(test_Mult_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>();

  const auto a = Mult(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "*");

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 9);

  y = 2;

  BOOST_CHECK_EQUAL(f(), -6);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -15);

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -6);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 10);

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -15);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 10);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 25);
}

BOOST_AUTO_TEST_CASE(test_Mult_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = Mult(2, y);

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), -6);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 10);
}

BOOST_AUTO_TEST_CASE(test_Mult_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Mult(x, 2);

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -6);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 10);
}

// Division

BOOST_AUTO_TEST_CASE(test_Div_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>(1);

  const auto a = Div(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "/");

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 / 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 / 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 / 5));
}

BOOST_AUTO_TEST_CASE(test_Div_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = Div(2, y);

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), (2 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 / 5));
}

BOOST_AUTO_TEST_CASE(test_Div_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Div(x, 2);

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 / 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 / 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 / 2));
}

// Modulo (integer remainder)

BOOST_AUTO_TEST_CASE(test_Mod_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>(1);

  const auto a = Mod(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "mod");

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 % 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 % 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 % 5));
}

BOOST_AUTO_TEST_CASE(test_Mod_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = Mod(2, y);

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), (2 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 % 5));
}

BOOST_AUTO_TEST_CASE(test_Mod_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Mod(x, 2);

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 % 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 % 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 % 2));
}

// Increment

BOOST_AUTO_TEST_CASE(test_Incr_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Incr(x);

  BOOST_CHECK_EQUAL(introspect::label(a), "incr");

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -2);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 3);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 6);
}

// Decrement

BOOST_AUTO_TEST_CASE(test_Decr_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = Decr(x);

  BOOST_CHECK_EQUAL(introspect::label(a), "decr");

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -4);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 1);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 4);
}

// Addition

BOOST_AUTO_TEST_CASE(test_Add_operator_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>();

  const auto a = x + y;

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -6);

  y = 2;

  BOOST_CHECK_EQUAL(f(), -1);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 2);

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -1);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 7);

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 2);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 7);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 10);
}

BOOST_AUTO_TEST_CASE(test_Add_operator_string)
{
  Engine engine;

  auto x = Var<std::string>("aa1bb");
  auto y = Var<std::string>("aa1bb");

  const auto a = x + y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), "aa1bbaa1bb");

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), "aa1bbaa2bb");

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), "aa1bbaa3bb");

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), "aa2bbaa1bb");

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), "aa2bbaa2bb");

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), "aa2bbaa3bb");

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), "aa3bbaa1bb");

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), "aa3bbaa2bb");

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), "aa3bbaa3bb");
}

BOOST_AUTO_TEST_CASE(test_Add_operator_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = 2 + y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), -1);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 7);
}

BOOST_AUTO_TEST_CASE(test_Add_operator_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = x + 2;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -1);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 7);
}

BOOST_AUTO_TEST_CASE(test_Add_operator_lhs_literal_string)
{
  Engine engine;

  auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" + y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), "aa2bbaa1bb");

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), "aa2bbaa2bb");

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), "aa2bbaa3bb");
}

BOOST_AUTO_TEST_CASE(test_Add_operator_rhs_literal_string)
{
  Engine engine;

  auto x = Var<std::string>("aa1bb");

  const auto a = x + "aa2bb";

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), "aa1bbaa2bb");

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), "aa2bbaa2bb");

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), "aa3bbaa2bb");
}

// Subtraction

BOOST_AUTO_TEST_CASE(test_Sub_operator_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>();

  const auto a = x - y;

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 0);

  y = 2;

  BOOST_CHECK_EQUAL(f(), -5);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -8);

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 5);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 0);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -3);

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 8);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 3);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 0);
}

BOOST_AUTO_TEST_CASE(test_Sub_operator_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = 2 - y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), 5);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 0);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -3);
}

BOOST_AUTO_TEST_CASE(test_Sub_operator_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = x - 2;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -5);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 0);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 3);
}

// Unary plus

BOOST_AUTO_TEST_CASE(test_Plus_operator_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = +x;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -3);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 2);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 5);
}

// Unary minus (additive inverse)

BOOST_AUTO_TEST_CASE(test_Inv_operator_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = -x;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), 3);

  x = 2;

  BOOST_CHECK_EQUAL(f(), -2);

  x = 5;

  BOOST_CHECK_EQUAL(f(), -5);
}

// Multiplication

BOOST_AUTO_TEST_CASE(test_Mult_operator_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>();

  const auto a = x * y;

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), 9);

  y = 2;

  BOOST_CHECK_EQUAL(f(), -6);

  y = 5;

  BOOST_CHECK_EQUAL(f(), -15);

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -6);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 10);

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), -15);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 10);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 25);
}

BOOST_AUTO_TEST_CASE(test_Mult_operator_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = 2 * y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), -6);

  y = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  y = 5;

  BOOST_CHECK_EQUAL(f(), 10);
}

BOOST_AUTO_TEST_CASE(test_Mult_operator_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = x * 2;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -6);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 4);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 10);
}

// Division

BOOST_AUTO_TEST_CASE(test_Div_operator_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>(1);

  const auto a = x / y;

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 / 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 / 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 / 5));
}

BOOST_AUTO_TEST_CASE(test_Div_operator_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = 2 / y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), (2 / -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 / 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 / 5));
}

BOOST_AUTO_TEST_CASE(test_Div_operator_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = x / 2;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 / 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 / 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 / 2));
}

// Modulo (integer remainder)

BOOST_AUTO_TEST_CASE(test_Mod_operator_int)
{
  Engine engine;

  auto x = Var<int>();
  auto y = Var<int>(1);

  const auto a = x % y;

  const auto f = Main(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 % 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 % 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 % 5));
}

BOOST_AUTO_TEST_CASE(test_Mod_operator_lhs_literal_int)
{
  Engine engine;

  auto y = Var<int>(-3);

  const auto a = 2 % y;

  const auto f = Main(a);

  BOOST_CHECK_EQUAL(f(), (2 % -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 % 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 % 5));
}

BOOST_AUTO_TEST_CASE(test_Mod_operator_rhs_literal_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = x % 2;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 % 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 % 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 % 2));
}

// Increment

BOOST_AUTO_TEST_CASE(test_Incr_operator_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = ++x;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -2);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 3);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 6);
}

// Decrement

BOOST_AUTO_TEST_CASE(test_Decr_operator_int)
{
  Engine engine;

  auto x = Var<int>();

  const auto a = --x;

  const auto f = Main(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), -4);

  x = 2;

  BOOST_CHECK_EQUAL(f(), 1);

  x = 5;

  BOOST_CHECK_EQUAL(f(), 4);
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
