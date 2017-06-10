
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

#include <dataflow/comparison.h>
#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_comparison)

BOOST_AUTO_TEST_CASE(test_Eq_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = Eq(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "==");

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 == 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 == 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 == 5));
}

BOOST_AUTO_TEST_CASE(test_NotEq_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = NotEq(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "!=");

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 != 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 != 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 != 5));
}

BOOST_AUTO_TEST_CASE(test_Gr_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = Gr(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), ">");

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 > 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 > 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 > 5));
}

BOOST_AUTO_TEST_CASE(test_Less_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = Less(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "<");

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 < 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 < 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 < 5));
}

BOOST_AUTO_TEST_CASE(test_GrEq_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = GrEq(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), ">=");

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 >= 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 >= 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 >= 5));
}

BOOST_AUTO_TEST_CASE(test_LessEq_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = LessEq(x, y);

  BOOST_CHECK_EQUAL(introspect::label(a), "<=");

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 <= 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 <= 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 <= 5));
}

// Comparison operators

// Equal to

BOOST_AUTO_TEST_CASE(test_Eq_operator_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = x == y;

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 == 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 == 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 == 5));
}

BOOST_AUTO_TEST_CASE(test_Eq_operator_string)
{
  Engine engine;

  const auto x = Var<std::string>();
  const auto y = Var<std::string>();

  const auto a = x == y;

  const auto f = Curr(a);

  x = "aa1bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") == std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") == std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") == std::string("aa3bb")));

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa3bb")));

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") == std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") == std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") == std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_Eq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>(-3);

  const auto a = 2 == y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (2 == -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 == 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 == 5));
}

BOOST_AUTO_TEST_CASE(test_Eq_operator_rhs_literal_int)
{
  Engine engine;

  const auto x = Var<int>();

  const auto a = x == 2;

  const auto f = Curr(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 == 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 == 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 == 2));
}

BOOST_AUTO_TEST_CASE(test_Eq_operator_lhs_literal_string)
{
  Engine engine;

  const auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" == y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_Eq_operator_rhs_literal_string)
{
  Engine engine;

  const auto x = Var<std::string>("aa1bb");

  const auto a = x == "aa2bb";

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") == std::string("aa2bb")));

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") == std::string("aa2bb")));

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") == std::string("aa2bb")));
}

// Not equal to

BOOST_AUTO_TEST_CASE(test_NotEq_operator_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = x != y;

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 != 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 != 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 != 5));
}

BOOST_AUTO_TEST_CASE(test_NotEq_operator_string)
{
  Engine engine;

  const auto x = Var<std::string>();
  const auto y = Var<std::string>();

  const auto a = x != y;

  const auto f = Curr(a);

  x = "aa1bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") != std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") != std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") != std::string("aa3bb")));

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa3bb")));

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") != std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") != std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") != std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_NotEq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>(-3);

  const auto a = 2 != y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (2 != -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 != 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 != 5));
}

BOOST_AUTO_TEST_CASE(test_NotEq_operator_rhs_literal_int)
{
  Engine engine;

  const auto x = Var<int>();

  const auto a = x != 2;

  const auto f = Curr(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 != 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 != 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 != 2));
}

BOOST_AUTO_TEST_CASE(test_NotEq_operator_lhs_literal_string)
{
  Engine engine;

  const auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" != y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_NotEq_operator_rhs_literal_string)
{
  Engine engine;

  const auto x = Var<std::string>("aa1bb");

  const auto a = x != "aa2bb";

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") != std::string("aa2bb")));

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") != std::string("aa2bb")));

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") != std::string("aa2bb")));
}

// Greater than

BOOST_AUTO_TEST_CASE(test_Gr_operator_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = x > y;

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 > 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 > 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 > 5));
}

BOOST_AUTO_TEST_CASE(test_Gr_operator_string)
{
  Engine engine;

  const auto x = Var<std::string>();
  const auto y = Var<std::string>();

  const auto a = x > y;

  const auto f = Curr(a);

  x = "aa1bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") > std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") > std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") > std::string("aa3bb")));

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa3bb")));

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") > std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") > std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") > std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_Gr_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>(-3);

  const auto a = 2 > y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (2 > -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 > 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 > 5));
}

BOOST_AUTO_TEST_CASE(test_Gr_operator_rhs_literal_int)
{
  Engine engine;

  const auto x = Var<int>();

  const auto a = x > 2;

  const auto f = Curr(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 > 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 > 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 > 2));
}

BOOST_AUTO_TEST_CASE(test_Gr_operator_lhs_literal_string)
{
  Engine engine;

  const auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" > y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_Gr_operator_rhs_literal_string)
{
  Engine engine;

  const auto x = Var<std::string>("aa1bb");

  const auto a = x > "aa2bb";

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") > std::string("aa2bb")));

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") > std::string("aa2bb")));

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") > std::string("aa2bb")));
}

// Less than

BOOST_AUTO_TEST_CASE(test_Less_operator_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = x < y;

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 < 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 < 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 < 5));
}

BOOST_AUTO_TEST_CASE(test_Less_operator_string)
{
  Engine engine;

  const auto x = Var<std::string>();
  const auto y = Var<std::string>();

  const auto a = x < y;

  const auto f = Curr(a);

  x = "aa1bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") < std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") < std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") < std::string("aa3bb")));

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa3bb")));

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") < std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") < std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") < std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_Less_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>(-3);

  const auto a = 2 < y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (2 < -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 < 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 < 5));
}

BOOST_AUTO_TEST_CASE(test_Less_operator_rhs_literal_int)
{
  Engine engine;

  const auto x = Var<int>();

  const auto a = x < 2;

  const auto f = Curr(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 < 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 < 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 < 2));
}

BOOST_AUTO_TEST_CASE(test_Less_operator_lhs_literal_string)
{
  Engine engine;

  const auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" < y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_Less_operator_rhs_literal_string)
{
  Engine engine;

  const auto x = Var<std::string>("aa1bb");

  const auto a = x < "aa2bb";

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") < std::string("aa2bb")));

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") < std::string("aa2bb")));

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") < std::string("aa2bb")));
}

// Greater than or equal to

BOOST_AUTO_TEST_CASE(test_GrEq_operator_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = x >= y;

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 >= 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 >= 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 >= 5));
}

BOOST_AUTO_TEST_CASE(test_GrEq_operator_string)
{
  Engine engine;

  const auto x = Var<std::string>();
  const auto y = Var<std::string>();

  const auto a = x >= y;

  const auto f = Curr(a);

  x = "aa1bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") >= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") >= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") >= std::string("aa3bb")));

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa3bb")));

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") >= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") >= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") >= std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_GrEq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>(-3);

  const auto a = 2 >= y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (2 >= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 >= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 >= 5));
}

BOOST_AUTO_TEST_CASE(test_GrEq_operator_rhs_literal_int)
{
  Engine engine;

  const auto x = Var<int>();

  const auto a = x >= 2;

  const auto f = Curr(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 >= 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 >= 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 >= 2));
}

BOOST_AUTO_TEST_CASE(test_GrEq_operator_lhs_literal_string)
{
  Engine engine;

  const auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" >= y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_GrEq_operator_rhs_literal_string)
{
  Engine engine;

  const auto x = Var<std::string>("aa1bb");

  const auto a = x >= "aa2bb";

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") >= std::string("aa2bb")));

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") >= std::string("aa2bb")));

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") >= std::string("aa2bb")));
}

// Less than or equal to

BOOST_AUTO_TEST_CASE(test_LessEq_operator_int)
{
  Engine engine;

  const auto x = Var<int>();
  const auto y = Var<int>();

  const auto a = x <= y;

  const auto f = Curr(a);

  x = -3;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (-3 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (-3 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (-3 <= 5));

  x = 2;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (2 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 <= 5));

  x = 5;

  y = -3;

  BOOST_CHECK_EQUAL(f(), (5 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (5 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (5 <= 5));
}

BOOST_AUTO_TEST_CASE(test_LessEq_operator_string)
{
  Engine engine;

  const auto x = Var<std::string>();
  const auto y = Var<std::string>();

  const auto a = x <= y;

  const auto f = Curr(a);

  x = "aa1bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") <= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") <= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") <= std::string("aa3bb")));

  x = "aa2bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa3bb")));

  x = "aa3bb";

  y = "aa1bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") <= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") <= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") <= std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_LessEq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>(-3);

  const auto a = 2 <= y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (2 <= -3));

  y = 2;

  BOOST_CHECK_EQUAL(f(), (2 <= 2));

  y = 5;

  BOOST_CHECK_EQUAL(f(), (2 <= 5));
}

BOOST_AUTO_TEST_CASE(test_LessEq_operator_rhs_literal_int)
{
  Engine engine;

  const auto x = Var<int>();

  const auto a = x <= 2;

  const auto f = Curr(a);

  x = -3;

  BOOST_CHECK_EQUAL(f(), (-3 <= 2));

  x = 2;

  BOOST_CHECK_EQUAL(f(), (2 <= 2));

  x = 5;

  BOOST_CHECK_EQUAL(f(), (5 <= 2));
}

BOOST_AUTO_TEST_CASE(test_LessEq_operator_lhs_literal_string)
{
  Engine engine;

  const auto y = Var<std::string>("aa1bb");

  const auto a = "aa2bb" <= y;

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa1bb")));

  y = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa2bb")));

  y = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa3bb")));
}

BOOST_AUTO_TEST_CASE(test_LessEq_operator_rhs_literal_string)
{
  Engine engine;

  const auto x = Var<std::string>("aa1bb");

  const auto a = x <= "aa2bb";

  const auto f = Curr(a);

  BOOST_CHECK_EQUAL(f(), (std::string("aa1bb") <= std::string("aa2bb")));

  x = "aa2bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa2bb") <= std::string("aa2bb")));

  x = "aa3bb";

  BOOST_CHECK_EQUAL(f(), (std::string("aa3bb") <= std::string("aa2bb")));
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
