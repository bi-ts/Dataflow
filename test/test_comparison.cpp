
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

BOOST_AUTO_TEST_CASE(test_Eq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>();

  const auto a = 2 == y;

  const auto f = Curr(a);

  y = -3;

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

BOOST_AUTO_TEST_CASE(test_NotEq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>();

  const auto a = 2 != y;

  const auto f = Curr(a);

  y = -3;

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

BOOST_AUTO_TEST_CASE(test_Gr_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>();

  const auto a = 2 > y;

  const auto f = Curr(a);

  y = -3;

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

BOOST_AUTO_TEST_CASE(test_Less_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>();

  const auto a = 2 < y;

  const auto f = Curr(a);

  y = -3;

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

BOOST_AUTO_TEST_CASE(test_GrEq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>();

  const auto a = 2 >= y;

  const auto f = Curr(a);

  y = -3;

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

BOOST_AUTO_TEST_CASE(test_LessEq_operator_lhs_literal_int)
{
  Engine engine;

  const auto y = Var<int>();

  const auto a = 2 <= y;

  const auto f = Curr(a);

  y = -3;

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

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
