
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

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>

#include <boost/preprocessor/repetition.hpp>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

class test_prelude_basic
{
protected:
  test_prelude_basic()
  {
  }

private:
  Engine engine_;
};

class test_prelude_binary : public test_prelude_basic
{
protected:
  test_prelude_binary()
  : x(Var<int>(1))
  , y(Var<int>(1))
  {
  }

protected:
  var<int> x;
  var<int> y;
};

class test_prelude_logical : public test_prelude_basic
{
protected:
  test_prelude_logical()
  : x(Var<bool>(false))
  , y(Var<bool>(false))
  {
  }

protected:
  var<bool> x;
  var<bool> y;
};

#define TEST_BINARY_OPERATOR(name, op)                                         \
  BOOST_FIXTURE_TEST_CASE(test_operator_##name, test_prelude_binary)           \
  {                                                                            \
    const auto a = *(x op y);                                                  \
    const auto b = *(x op 2);                                                  \
    const auto c = *(2 op y);                                                  \
                                                                               \
    x = 1;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL((1 op 1), a());                                          \
    BOOST_CHECK_EQUAL((1 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 1), c());                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL((1 op 2), a());                                          \
    BOOST_CHECK_EQUAL((1 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 2), c());                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL((1 op 3), a());                                          \
    BOOST_CHECK_EQUAL((1 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 3), c());                                          \
    x = 2;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL((2 op 1), a());                                          \
    BOOST_CHECK_EQUAL((2 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 1), c());                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL((2 op 2), a());                                          \
    BOOST_CHECK_EQUAL((2 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 2), c());                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL((2 op 3), a());                                          \
    BOOST_CHECK_EQUAL((2 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 3), c());                                          \
    x = 3;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL((3 op 1), a());                                          \
    BOOST_CHECK_EQUAL((3 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 1), c());                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL((3 op 2), a());                                          \
    BOOST_CHECK_EQUAL((3 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 2), c());                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL((3 op 3), a());                                          \
    BOOST_CHECK_EQUAL((3 op 2), b());                                          \
    BOOST_CHECK_EQUAL((2 op 3), c());                                          \
  }

#define TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(name, op)                    \
  BOOST_FIXTURE_TEST_CASE(test_operator_##name##_string, test_prelude_basic)   \
  {                                                                            \
    const auto x = Var<std::string>("str1");                                   \
    const auto y = Var<std::string>("str1");                                   \
                                                                               \
    const auto a = *(x op y);                                                  \
    const auto b = *(x op "str2");                                             \
    const auto c = *("str2" op y);                                             \
                                                                               \
    BOOST_CHECK_EQUAL((std::string("str1") op std::string("str1")), a());      \
    BOOST_CHECK_EQUAL((std::string("str1") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str1")), c());      \
    y = "str2";                                                                \
    BOOST_CHECK_EQUAL((std::string("str1") op std::string("str2")), a());      \
    BOOST_CHECK_EQUAL((std::string("str1") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), c());      \
    y = "str3";                                                                \
    BOOST_CHECK_EQUAL((std::string("str1") op std::string("str3")), a());      \
    BOOST_CHECK_EQUAL((std::string("str1") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str3")), c());      \
    x = "str2";                                                                \
    y = "str1";                                                                \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str1")), a());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str1")), c());      \
    y = "str2";                                                                \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), a());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), c());      \
    y = "str3";                                                                \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str3")), a());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str3")), c());      \
    x = "str3";                                                                \
    y = "str1";                                                                \
    BOOST_CHECK_EQUAL((std::string("str3") op std::string("str1")), a());      \
    BOOST_CHECK_EQUAL((std::string("str3") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str1")), c());      \
    y = "str2";                                                                \
    BOOST_CHECK_EQUAL((std::string("str3") op std::string("str2")), a());      \
    BOOST_CHECK_EQUAL((std::string("str3") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str2")), c());      \
    y = "str3";                                                                \
    BOOST_CHECK_EQUAL((std::string("str3") op std::string("str3")), a());      \
    BOOST_CHECK_EQUAL((std::string("str3") op std::string("str2")), b());      \
    BOOST_CHECK_EQUAL((std::string("str2") op std::string("str3")), c());      \
  }

#define TEST_BINARY_FUNCTION(fn, op)                                           \
  BOOST_FIXTURE_TEST_CASE(test_##fn, test_prelude_binary)                      \
  {                                                                            \
    const auto z = fn(x, y);                                                   \
    const auto a = *z;                                                         \
    BOOST_CHECK_EQUAL(#op, introspect::label(z));                              \
                                                                               \
    x = 1;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL((1 op 1), a());                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL((1 op 2), a());                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL((1 op 3), a());                                          \
    x = 2;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL((2 op 1), a());                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL((2 op 2), a());                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL((2 op 3), a());                                          \
    x = 3;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL((3 op 1), a());                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL((3 op 2), a());                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL((3 op 3), a());                                          \
  }

// Basic

BOOST_AUTO_TEST_SUITE(test_prelude)

BOOST_FIXTURE_TEST_CASE(test_Const, test_prelude_basic)
{
  const ref<int> x = Const<int>(17);

  BOOST_CHECK_EQUAL("const", introspect::label(x));

  const auto y = Curr(x);

  BOOST_CHECK_EQUAL(17, y());
}

BOOST_FIXTURE_TEST_CASE(test_Const_no_params, test_prelude_basic)
{
  const ref<int> x = Const<int>();

  BOOST_CHECK_EQUAL("const", introspect::label(x));

  auto y = Curr(x);

  BOOST_CHECK_EQUAL(0, y());
}

BOOST_FIXTURE_TEST_CASE(test_Const_string_literal, test_prelude_basic)
{
  const ref<std::string> x = Const("some text");

  BOOST_CHECK_EQUAL("const", introspect::label(x));

  auto y = Curr(x);

  BOOST_CHECK_EQUAL("some text", y());
}

BOOST_FIXTURE_TEST_CASE(test_Var, test_prelude_basic)
{
  const var<int> x = Var<int>(17);

  BOOST_CHECK_EQUAL("var", introspect::label(x));

  auto y = Curr(x);

  BOOST_CHECK_EQUAL(17, y());

  x = 6;

  BOOST_CHECK_EQUAL(6, y());
}

BOOST_FIXTURE_TEST_CASE(test_Curr, test_prelude_basic)
{
  const var<int> x = Var<int>(6);

  BOOST_CHECK_EQUAL(1, introspect::ref_count(x));

  const auto y = Curr(x);

  BOOST_CHECK_EQUAL(2, introspect::ref_count(x));

  BOOST_CHECK_EQUAL("curr", introspect::label(y));
  BOOST_CHECK_EQUAL(6, y());
}

BOOST_FIXTURE_TEST_CASE(test_Main, test_prelude_basic)
{
  const var<int> x = Var<int>(6);
  const var<int> y = Var<int>(25);

  const auto z = Main([=](const Time& t)
                      {
                        return x + y;
                      });

  BOOST_CHECK_EQUAL(31, z());

  x = 25;

  BOOST_CHECK_EQUAL(50, z());

  y = 7;

  BOOST_CHECK_EQUAL(32, z());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_policy_static_func, test_prelude_basic)
{
  const var<int> x = Var<int>('A');

  struct policy
  {
    static std::string label()
    {
      return "shift";
    }
    char calculate(int v)
    {
      return static_cast<char>(v + 2);
    };
  };

  const auto y = Lift(x, policy());
  const auto z = Curr(y);

  BOOST_CHECK_EQUAL("shift", introspect::label(y));

  BOOST_CHECK_EQUAL('C', z());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_policy_member_func, test_prelude_basic)
{
  const var<int> x = Var<int>('C');

  struct policy
  {
    static std::string label()
    {
      return "lowercase";
    }
    const char& calculate(int v)
    {
      cache_ = static_cast<char>(v + ('a' - 'A'));
      return cache_;
    };

  private:
    char cache_;
  };

  const auto y = Lift(x, policy());
  const auto z = Curr(y);

  BOOST_CHECK_EQUAL("lowercase", introspect::label(y));

  BOOST_CHECK_EQUAL('c', z());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_lambda, test_prelude_basic)
{
  const auto x = Var<char>('B');

  const auto y = Lift("trinity",
                      x,
                      [](char c)
                      {
                        return std::string(3, c);
                      });

  const auto z = Curr(y);

  BOOST_CHECK_EQUAL("trinity", introspect::label(y));

  BOOST_CHECK_EQUAL("BBB", z());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_function_pointer, test_prelude_basic)
{
  const auto x = Var<char>('C');

  struct tool
  {
    static std::string duplicate(char c)
    {
      return std::string(2, c);
    };
  };

  const auto y = Lift("duplicate", x, tool::duplicate);

  const auto z = Curr(y);

  BOOST_CHECK_EQUAL("duplicate", introspect::label(y));

  BOOST_CHECK_EQUAL("CC", z());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_policy_static_func, test_prelude_basic)
{
  const var<char> x = Var<char>('A');
  const var<int> y = Var<int>(4);

  struct policy
  {
    static std::string label()
    {
      return "shift";
    }
    char calculate(char v, int offset)
    {
      return static_cast<char>(v + offset);
    };
  };

  const auto z = Lift(x, y, policy());
  const auto a = Curr(z);

  BOOST_CHECK_EQUAL("shift", introspect::label(z));

  BOOST_CHECK_EQUAL('E', a());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_policy_member_func, test_prelude_basic)
{
  const var<int> x = Var<int>('C');
  const var<bool> y = Var<bool>(true);

  struct policy
  {
    static std::string label()
    {
      return "lowercase";
    }
    const char& calculate(int v, bool b)
    {
      cache_ = static_cast<char>(v + (b ? ('a' - 'A') : 0));
      return cache_;
    };

  private:
    char cache_;
  };

  const auto z = Lift(x, y, policy());
  const auto a = Curr(z);

  BOOST_CHECK_EQUAL("lowercase", introspect::label(z));

  BOOST_CHECK_EQUAL('c', a());

  y = false;

  BOOST_CHECK_EQUAL('C', a());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_lambda, test_prelude_basic)
{
  const auto x = Var<char>('B');
  const auto y = Var<int>(4);

  const auto z = Lift("multiply",
                      x,
                      y,
                      [](char c, int n)
                      {
                        return std::string(n, c);
                      });

  const auto a = Curr(z);

  BOOST_CHECK_EQUAL("multiply", introspect::label(z));

  BOOST_CHECK_EQUAL("BBBB", a());

  y = 2;

  BOOST_CHECK_EQUAL("BB", a());

  x = 'A';

  BOOST_CHECK_EQUAL("AA", a());
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_function_pointer, test_prelude_basic)
{
  const auto x = Var<char>('C');
  const auto y = Var<int>(3);

  struct tool
  {
    static std::string multiply(char c, int n)
    {
      return std::string(n, c);
    };
  };

  const auto z = Lift("multiply", x, y, tool::multiply);

  const auto a = Curr(z);

  BOOST_CHECK_EQUAL("multiply", introspect::label(z));

  BOOST_CHECK_EQUAL("CCC", a());

  y = 2;

  BOOST_CHECK_EQUAL("CC", a());

  x = 'A';

  BOOST_CHECK_EQUAL("AA", a());
}

BOOST_FIXTURE_TEST_CASE(test_Curr_operator, test_prelude_basic)
{
  const eager<int> x = *Var<int>(15);

  BOOST_CHECK_EQUAL("curr", introspect::label(x));
  BOOST_CHECK_EQUAL(15, x());
}

// Arithmetic

BOOST_FIXTURE_TEST_CASE(test_Abs, test_prelude_basic)
{
  const auto x = Var<int>(-13);

  const auto z = Abs(x);

  BOOST_CHECK_EQUAL("abs", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(13, a());

  x = 5;

  BOOST_CHECK_EQUAL(5, a());
}

TEST_BINARY_FUNCTION(Add, +);
TEST_BINARY_OPERATOR(Add, +);
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Add, +);

TEST_BINARY_FUNCTION(Sub, -);
TEST_BINARY_OPERATOR(Sub, -);

BOOST_FIXTURE_TEST_CASE(test_Plus, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = Plus(x);

  BOOST_CHECK_EQUAL("(+)", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(13, a());

  x = -5;

  BOOST_CHECK_EQUAL(-5, a());
}

BOOST_FIXTURE_TEST_CASE(test_Plus_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = +x;

  BOOST_CHECK_EQUAL("(+)", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(13, a());

  x = -5;

  BOOST_CHECK_EQUAL(-5, a());
}

BOOST_FIXTURE_TEST_CASE(test_Inv, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = Inv(x);

  BOOST_CHECK_EQUAL("(-)", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(-13, a());

  x = -5;

  BOOST_CHECK_EQUAL(5, a());
}

BOOST_FIXTURE_TEST_CASE(test_Inv_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = -x;

  BOOST_CHECK_EQUAL("(-)", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(-13, a());

  x = -5;

  BOOST_CHECK_EQUAL(5, a());
}

TEST_BINARY_FUNCTION(Mult, *);
TEST_BINARY_OPERATOR(Mult, *);

TEST_BINARY_FUNCTION(Div, / );
TEST_BINARY_OPERATOR(Div, / );

BOOST_FIXTURE_TEST_CASE(test_Mod, test_prelude_binary)
{
  y = 1;

  auto z = Mod(Const<int>(60), y);

  BOOST_CHECK_EQUAL("mod", introspect::label(z));

  auto a = Curr(z);

  std::vector<int> mods(1, a());

  for (int i = 2; i < 10; ++i)
  {
    y = i;
    mods.push_back(a());
  }

  BOOST_TEST(mods == std::vector<int>({0, 0, 0, 0, 0, 0, 4, 4, 6}),
             boost::test_tools::per_element());
}

TEST_BINARY_OPERATOR(Mod, % );

BOOST_FIXTURE_TEST_CASE(test_Incr, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = Incr(x);

  BOOST_CHECK_EQUAL("incr", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(14, a());

  x = 42;

  BOOST_CHECK_EQUAL(43, a());
}

BOOST_FIXTURE_TEST_CASE(test_Incr_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto y = ++x;

  BOOST_CHECK_EQUAL("incr", introspect::label(y));

  const auto z = y++;

  BOOST_CHECK_EQUAL("incr", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(15, a());

  x = -42;

  BOOST_CHECK_EQUAL(-40, a());
}

BOOST_FIXTURE_TEST_CASE(test_Decr, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = Decr(x);

  BOOST_CHECK_EQUAL("decr", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(12, a());

  x = 42;

  BOOST_CHECK_EQUAL(41, a());
}

BOOST_FIXTURE_TEST_CASE(test_Decr_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto y = --x;

  BOOST_CHECK_EQUAL("decr", introspect::label(y));

  const auto z = y--;

  BOOST_CHECK_EQUAL("decr", introspect::label(z));

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(11, a());

  x = -42;

  BOOST_CHECK_EQUAL(-44, a());
}

// Comparison

TEST_BINARY_FUNCTION(Eq, == );
TEST_BINARY_OPERATOR(Eq, == );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Eq, == );

TEST_BINARY_FUNCTION(NotEq, != );
TEST_BINARY_OPERATOR(NotEq, != );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(NotEq, != );

TEST_BINARY_FUNCTION(Gr, > );
TEST_BINARY_OPERATOR(Gr, > );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Gr, > );

TEST_BINARY_FUNCTION(Less, < );
TEST_BINARY_OPERATOR(Less, < );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Less, < );

TEST_BINARY_FUNCTION(GrEq, >= );
TEST_BINARY_OPERATOR(GrEq, >= );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(GrEq, >= );

TEST_BINARY_FUNCTION(LessEq, <= );
TEST_BINARY_OPERATOR(LessEq, <= );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(LessEq, <= );

// Logical

BOOST_FIXTURE_TEST_CASE(test_Not, test_prelude_logical)
{
  x = false;

  const auto z = Not(x);

  BOOST_CHECK_EQUAL("not", introspect::label(z));

  const auto a = Curr(z);

  BOOST_CHECK(a());

  x = true;

  BOOST_CHECK(!a());
}

BOOST_FIXTURE_TEST_CASE(test_Not_operator, test_prelude_logical)
{
  x = false;

  const auto a = Curr(!x);

  BOOST_CHECK(a());

  x = true;

  BOOST_CHECK(!a());
}

BOOST_FIXTURE_TEST_CASE(test_And, test_prelude_logical)
{
  x = false;
  y = false;

  const auto a = Curr(And(x, y));

  // Check that we have less than 6 vertices:
  // - x, y, curr;
  // - `And` shouldn't add more than 3 vertices.
  BOOST_CHECK_GE(6, introspect::num_vertices());

  BOOST_CHECK(!a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));

  x = true;

  BOOST_CHECK(!a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = true;

  BOOST_CHECK(a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = false;

  BOOST_CHECK(!a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));
}

BOOST_FIXTURE_TEST_CASE(test_And_operator, test_prelude_logical)
{
  x = false;
  y = false;

  const auto a = Curr(x && y);
  const auto b = Curr(false && y);
  const auto c = Curr(x && true);

  BOOST_CHECK(!a());
  BOOST_CHECK(!b());
  BOOST_CHECK(!c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));

  x = true;

  BOOST_CHECK(!a());
  BOOST_CHECK(!b());
  BOOST_CHECK(c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = true;

  BOOST_CHECK(a());
  BOOST_CHECK(!b());
  BOOST_CHECK(c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = false;

  BOOST_CHECK(!a());
  BOOST_CHECK(!b());
  BOOST_CHECK(!c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));
}

BOOST_FIXTURE_TEST_CASE(test_Or, test_prelude_logical)
{
  x = false;
  y = false;

  const auto a = Curr(Or(x, y));

  // Check that we have less than 6 vertices:
  // - x, y, curr;
  // - `Or` shouldn't add more than 3 vertices.
  BOOST_CHECK_GE(6, introspect::num_vertices());

  BOOST_CHECK(!a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK(a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));

  y = true;

  BOOST_CHECK(a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));

  x = false;

  BOOST_CHECK(a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));
}

BOOST_FIXTURE_TEST_CASE(test_Or_operator, test_prelude_logical)
{
  x = false;
  y = false;

  const auto a = Curr(x || y);
  const auto b = Curr(true || y);
  const auto c = Curr(x || false);

  BOOST_CHECK(!a());
  BOOST_CHECK(b());
  BOOST_CHECK(!c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK(a());
  BOOST_CHECK(b());
  BOOST_CHECK(c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));

  y = true;

  BOOST_CHECK(a());
  BOOST_CHECK(b());
  BOOST_CHECK(c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(!introspect::active_node(y));

  x = false;

  BOOST_CHECK(a());
  BOOST_CHECK(b());
  BOOST_CHECK(!c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));
}

BOOST_FIXTURE_TEST_CASE(test_AndE, test_prelude_logical)
{
  x = false;
  y = false;

  const auto a = Curr(AndE(x, y));

  BOOST_CHECK(!a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK(!a());

  y = true;

  BOOST_CHECK(a());

  x = false;

  BOOST_CHECK(!a());
}

BOOST_FIXTURE_TEST_CASE(test_AndE_operator, test_prelude_logical)
{
  x = false;
  y = false;

  const auto a = Curr(x & y);
  const auto b = Curr(false & y);
  const auto c = Curr(x & true);

  BOOST_CHECK(!a());
  BOOST_CHECK(!b());
  BOOST_CHECK(!c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  x = true;

  BOOST_CHECK(!a());
  BOOST_CHECK(!b());
  BOOST_CHECK(c());

  y = true;

  BOOST_CHECK(a());
  BOOST_CHECK(!b());
  BOOST_CHECK(c());

  x = false;

  BOOST_CHECK(!a());
  BOOST_CHECK(!b());
  BOOST_CHECK(!c());
}

BOOST_FIXTURE_TEST_CASE(test_OrE, test_prelude_logical)
{
  x = true;
  y = false;

  const auto a = Curr(OrE(x, y));

  BOOST_CHECK(a());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = true;

  BOOST_CHECK(a());

  x = false;

  BOOST_CHECK(a());

  y = false;

  BOOST_CHECK(!a());
}

BOOST_FIXTURE_TEST_CASE(test_OrE_operator, test_prelude_logical)
{
  x = true;
  y = false;

  const auto a = Curr(x | y);
  const auto b = Curr(true | y);
  const auto c = Curr(x | false);

  BOOST_CHECK(a());
  BOOST_CHECK(b());
  BOOST_CHECK(c());

  BOOST_CHECK(introspect::active_node(x));
  BOOST_CHECK(introspect::active_node(y));

  y = true;

  BOOST_CHECK(a());
  BOOST_CHECK(b());
  BOOST_CHECK(c());

  x = false;

  BOOST_CHECK(a());
  BOOST_CHECK(b());
  BOOST_CHECK(!c());

  y = false;

  BOOST_CHECK(!a());
  BOOST_CHECK(b());
  BOOST_CHECK(!c());
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_var, test_prelude_basic)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);
  auto z = Var<int>(20);

  auto f = *If(x, y, z);

  BOOST_CHECK_EQUAL(10, f());

  x = false;

  BOOST_CHECK_EQUAL(20, f());
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_var, test_prelude_basic)
{
  auto x = Var<bool>(true);
  auto z = Var<int>(20);

  auto f = *If(x, 10, z);

  BOOST_CHECK_EQUAL(10, f());

  x = false;

  BOOST_CHECK_EQUAL(20, f());
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_int, test_prelude_basic)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);

  auto f = *If(x, y, 20);

  BOOST_CHECK_EQUAL(10, f());

  x = false;

  BOOST_CHECK_EQUAL(20, f());
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_int, test_prelude_basic)
{
  auto x = Var<bool>(true);

  auto f = *If(x, 10, 20);

  BOOST_CHECK_EQUAL(10, f());

  x = false;

  BOOST_CHECK_EQUAL(20, f());
}

BOOST_FIXTURE_TEST_CASE(test_Switch, test_prelude_basic)
{
  auto x = Var<std::string>("ten");

  auto f = *Switch(Case(x == "one", 1),
                   Case(x == "three", 3),
                   Case(x == "five", 5),
                   Case(x == "ten", 10),
                   Default(0));

  BOOST_CHECK_EQUAL(10, f());

  x = "one";

  BOOST_CHECK_EQUAL(1, f());

  x = "five";

  BOOST_CHECK_EQUAL(5, f());

  x = "three";

  BOOST_CHECK_EQUAL(3, f());
}

BOOST_FIXTURE_TEST_CASE(test_Switch_string, test_prelude_basic)
{
  auto x = Var<int>(1);

  auto f = *Switch(Case(x == 1, "one"),
                   Case(x == 2, "two"),
                   Case(x == 3, "three"),
                   Case(x == 4, "four"),
                   Default("error"));

  BOOST_CHECK_EQUAL("one", f());

  x = 2;

  BOOST_CHECK_EQUAL("two", f());

  x = 3;

  BOOST_CHECK_EQUAL("three", f());

  x = 4;

  BOOST_CHECK_EQUAL("four", f());

  x = 5;

  BOOST_CHECK_EQUAL("error", f());
}

BOOST_FIXTURE_TEST_CASE(test_ToString_int, test_prelude_basic)
{
  auto x = Var<int>(42);
  auto y = ToString(x);
  auto f = *y;

  BOOST_CHECK_EQUAL("ToString", introspect::label(y));

  BOOST_CHECK_EQUAL("42", f());

  x = 53;

  BOOST_CHECK_EQUAL("53", f());
}

BOOST_FIXTURE_TEST_CASE(test_ToString_string, test_prelude_basic)
{
  auto x = Var<std::string>("str");
  auto y = ToString(x);
  auto f = *y;

  BOOST_CHECK_EQUAL("var", introspect::label(y));

  BOOST_CHECK_EQUAL("str", f());

  x = "long string";

  BOOST_CHECK_EQUAL("long string", f());
}

BOOST_FIXTURE_TEST_CASE(test_ToString_int_leteral, test_prelude_basic)
{
  auto y = ToString(5);
  auto f = *y;

  BOOST_CHECK_EQUAL("const", introspect::label(y));

  BOOST_CHECK_EQUAL("5", f());
}

BOOST_FIXTURE_TEST_CASE(test_ToString_string_leteral, test_prelude_basic)
{
  auto y = ToString("string literal");
  auto f = *y;

  BOOST_CHECK_EQUAL("const", introspect::label(y));

  BOOST_CHECK_EQUAL("string literal", f());
}

BOOST_FIXTURE_TEST_CASE(test_ToString_concatenation, test_prelude_basic)
{
  auto x = Var<int>(42);
  auto y = Var<float>(42.5);
  auto z = ToString(x, " != ", y);
  auto f = *z;

  BOOST_CHECK_EQUAL("42 != 42.5", f());

  x = 43;

  BOOST_CHECK_EQUAL("43 != 42.5", f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int, test_prelude_basic)
{
  auto x = Var<std::string>("42");
  auto y = FromString<int>(x);
  auto f = *y;

  BOOST_CHECK_EQUAL("FromString", introspect::label(y));

  BOOST_CHECK_EQUAL(42, f());

  x = "55";

  BOOST_CHECK_EQUAL(55, f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int_default, test_prelude_basic)
{
  auto x = Var<std::string>("NAN");
  auto y = FromString<int>(x, 5);
  auto f = *y;

  BOOST_CHECK_EQUAL("FromString", introspect::label(y));

  BOOST_CHECK_EQUAL(5, f());

  x = "55";

  BOOST_CHECK_EQUAL(55, f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int_from_empty_string,
                        test_prelude_basic)
{
  auto x = Var<std::string>("");
  auto y = FromString<int>(x, 5);
  auto f = *y;

  BOOST_CHECK_EQUAL("FromString", introspect::label(y));

  BOOST_CHECK_EQUAL(5, f());

  x = "55";

  BOOST_CHECK_EQUAL(55, f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_string, test_prelude_basic)
{
  auto x = Var<std::string>("str");
  auto y = FromString<std::string>(x);
  auto f = *y;

  BOOST_CHECK_EQUAL("var", introspect::label(y));

  BOOST_CHECK_EQUAL("str", f());

  x = "long string";

  BOOST_CHECK_EQUAL("long string", f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_string_default, test_prelude_basic)
{
  auto x = Var<std::string>("str");
  auto y = FromString<std::string>(x, "never used");
  auto f = *y;

  BOOST_CHECK_EQUAL("var", introspect::label(y));

  BOOST_CHECK_EQUAL("str", f());

  x = "long string";

  BOOST_CHECK_EQUAL("long string", f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_string_literal, test_prelude_basic)
{
  auto x = FromString<int>("54");
  auto f = *x;

  BOOST_CHECK_EQUAL("const", introspect::label(x));

  BOOST_CHECK_EQUAL(54, f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_string_literal_default,
                        test_prelude_basic)
{
  auto x = FromString<int>("NAN", 3);
  auto y = FromString<int>("39", 10);
  auto f = *(x + y);

  BOOST_CHECK_EQUAL("const", introspect::label(x));
  BOOST_CHECK_EQUAL("const", introspect::label(y));

  BOOST_CHECK_EQUAL(42, f());
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_empty_string_literal,
                        test_prelude_basic)
{
  auto x = FromString<int>("", 13);
  auto y = FromString<int>("29", 10);
  auto f = *(x + y);

  BOOST_CHECK_EQUAL("const", introspect::label(x));
  BOOST_CHECK_EQUAL("const", introspect::label(y));

  BOOST_CHECK_EQUAL(42, f());
}

BOOST_AUTO_TEST_SUITE_END()
}
