
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
    BOOST_CHECK_EQUAL(a(), (1 op 1));                                          \
    BOOST_CHECK_EQUAL(b(), (1 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 1));                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL(a(), (1 op 2));                                          \
    BOOST_CHECK_EQUAL(b(), (1 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 2));                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL(a(), (1 op 3));                                          \
    BOOST_CHECK_EQUAL(b(), (1 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 3));                                          \
    x = 2;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL(a(), (2 op 1));                                          \
    BOOST_CHECK_EQUAL(b(), (2 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 1));                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL(a(), (2 op 2));                                          \
    BOOST_CHECK_EQUAL(b(), (2 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 2));                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL(a(), (2 op 3));                                          \
    BOOST_CHECK_EQUAL(b(), (2 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 3));                                          \
    x = 3;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL(a(), (3 op 1));                                          \
    BOOST_CHECK_EQUAL(b(), (3 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 1));                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL(a(), (3 op 2));                                          \
    BOOST_CHECK_EQUAL(b(), (3 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 2));                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL(a(), (3 op 3));                                          \
    BOOST_CHECK_EQUAL(b(), (3 op 2));                                          \
    BOOST_CHECK_EQUAL(c(), (2 op 3));                                          \
  }

#define TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(name, op)                    \
  BOOST_FIXTURE_TEST_CASE(test_operator_##name##_string, test_prelude_basic)   \
  {                                                                            \
    const auto x = Var("str1");                                                \
    const auto y = Var("str1");                                                \
                                                                               \
    const auto a = *(x op y);                                                  \
    const auto b = *(x op "str2");                                             \
    const auto c = *("str2" op y);                                             \
                                                                               \
    BOOST_CHECK_EQUAL(a(), (std::string("str1") op std::string("str1")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str1") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str1")));      \
    y = "str2";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str1") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str1") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str2")));      \
    y = "str3";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str1") op std::string("str3")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str1") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str3")));      \
    x = "str2";                                                                \
    y = "str1";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str2") op std::string("str1")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str2") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str1")));      \
    y = "str2";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str2") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str2") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str2")));      \
    y = "str3";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str2") op std::string("str3")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str2") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str3")));      \
    x = "str3";                                                                \
    y = "str1";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str3") op std::string("str1")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str3") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str1")));      \
    y = "str2";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str3") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str3") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str2")));      \
    y = "str3";                                                                \
    BOOST_CHECK_EQUAL(a(), (std::string("str3") op std::string("str3")));      \
    BOOST_CHECK_EQUAL(b(), (std::string("str3") op std::string("str2")));      \
    BOOST_CHECK_EQUAL(c(), (std::string("str2") op std::string("str3")));      \
  }

#define TEST_BINARY_FUNCTION(fn, op)                                           \
  BOOST_FIXTURE_TEST_CASE(test_##fn, test_prelude_binary)                      \
  {                                                                            \
    const auto z = fn(x, y);                                                   \
    const auto a = *z;                                                         \
    BOOST_CHECK_EQUAL(introspect::label(z), #op);                              \
                                                                               \
    x = 1;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL(a(), (1 op 1));                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL(a(), (1 op 2));                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL(a(), (1 op 3));                                          \
    x = 2;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL(a(), (2 op 1));                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL(a(), (2 op 2));                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL(a(), (2 op 3));                                          \
    x = 3;                                                                     \
    y = 1;                                                                     \
    BOOST_CHECK_EQUAL(a(), (3 op 1));                                          \
    y = 2;                                                                     \
    BOOST_CHECK_EQUAL(a(), (3 op 2));                                          \
    y = 3;                                                                     \
    BOOST_CHECK_EQUAL(a(), (3 op 3));                                          \
  }

BOOST_AUTO_TEST_SUITE(test_prelude)

// Arithmetic

BOOST_FIXTURE_TEST_CASE(test_Abs, test_prelude_basic)
{
  const auto x = Var<int>(-13);

  const auto z = Abs(x);

  BOOST_CHECK_EQUAL(introspect::label(z), "abs");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 13);

  x = 5;

  BOOST_CHECK_EQUAL(a(), 5);
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

  BOOST_CHECK_EQUAL(introspect::label(z), "(+)");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 13);

  x = -5;

  BOOST_CHECK_EQUAL(a(), -5);
}

BOOST_FIXTURE_TEST_CASE(test_Plus_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = +x;

  BOOST_CHECK_EQUAL(introspect::label(z), "(+)");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 13);

  x = -5;

  BOOST_CHECK_EQUAL(a(), -5);
}

BOOST_FIXTURE_TEST_CASE(test_Inv, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = Inv(x);

  BOOST_CHECK_EQUAL(introspect::label(z), "(-)");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), -13);

  x = -5;

  BOOST_CHECK_EQUAL(a(), 5);
}

BOOST_FIXTURE_TEST_CASE(test_Inv_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = -x;

  BOOST_CHECK_EQUAL(introspect::label(z), "(-)");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), -13);

  x = -5;

  BOOST_CHECK_EQUAL(a(), 5);
}

TEST_BINARY_FUNCTION(Mult, *);
TEST_BINARY_OPERATOR(Mult, *);

TEST_BINARY_FUNCTION(Div, / );
TEST_BINARY_OPERATOR(Div, / );

BOOST_FIXTURE_TEST_CASE(test_Mod, test_prelude_binary)
{
  y = 1;

  auto z = Mod(Const(60), y);

  BOOST_CHECK_EQUAL(introspect::label(z), "mod");

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

  BOOST_CHECK_EQUAL(introspect::label(z), "incr");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 14);

  x = 42;

  BOOST_CHECK_EQUAL(a(), 43);
}

BOOST_FIXTURE_TEST_CASE(test_Incr_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto y = ++x;

  BOOST_CHECK_EQUAL(introspect::label(y), "incr");

  const auto z = y++;

  BOOST_CHECK_EQUAL(introspect::label(z), "incr");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 15);

  x = -42;

  BOOST_CHECK_EQUAL(a(), -40);
}

BOOST_FIXTURE_TEST_CASE(test_Decr, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto z = Decr(x);

  BOOST_CHECK_EQUAL(introspect::label(z), "decr");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 12);

  x = 42;

  BOOST_CHECK_EQUAL(a(), 41);
}

BOOST_FIXTURE_TEST_CASE(test_Decr_operator, test_prelude_basic)
{
  const auto x = Var<int>(13);

  const auto y = --x;

  BOOST_CHECK_EQUAL(introspect::label(y), "decr");

  const auto z = y--;

  BOOST_CHECK_EQUAL(introspect::label(z), "decr");

  auto a = Curr(z);

  BOOST_CHECK_EQUAL(a(), 11);

  x = -42;

  BOOST_CHECK_EQUAL(a(), -44);
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

  BOOST_CHECK_EQUAL(introspect::label(z), "not");

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

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_var, test_prelude_basic)
{
  auto x = Var<bool>(true);
  auto z = Var<int>(20);

  auto f = *If(x, 10, z);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_int, test_prelude_basic)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);

  auto f = *If(x, y, 20);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_int, test_prelude_basic)
{
  auto x = Var<bool>(true);

  auto f = *If(x, 10, 20);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_Switch_if_int, test_prelude_basic)
{
  auto x = Var("ten");

  auto f = *Switch(Case(x == "one", 1),
                   Case(x == "three", 3),
                   Case(x == "five", 5),
                   Case(x == "ten", 10),
                   Default(0));

  BOOST_CHECK_EQUAL(f(), 10);

  x = "one";

  BOOST_CHECK_EQUAL(f(), 1);

  x = "five";

  BOOST_CHECK_EQUAL(f(), 5);

  x = "three";

  BOOST_CHECK_EQUAL(f(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_Switch_string_int, test_prelude_basic)
{
  auto x = Var("ten");
  auto y1 = Var(1);
  auto y3 = Var(3);
  auto y5 = Var(5);

  auto f = *Switch(x,
                   Case(Const("one"), y1),
                   Case(Const("two"), 2),
                   Case(std::string("three"), y3),
                   Case(std::string("four"), 4),
                   Case("five", y5),
                   Case("ten", 10),
                   Default(0));

  BOOST_CHECK_EQUAL(f(), 10);

  x = "one";

  BOOST_CHECK_EQUAL(f(), 1);

  x = "five";

  BOOST_CHECK_EQUAL(f(), 5);

  x = "two";

  BOOST_CHECK_EQUAL(f(), 2);

  x = "four";

  BOOST_CHECK_EQUAL(f(), 4);

  x = "three";

  BOOST_CHECK_EQUAL(f(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_Switch_string_string, test_prelude_basic)
{
  auto x = Var("yes");

  auto f = *Switch(x,
                   Case(Const("yes"), "ja"),
                   Case(std::string("no"), "nee"),
                   Case("maybe", "misschien"),
                   Default(""));

  BOOST_CHECK_EQUAL(f(), "ja");

  x = "maybe";

  BOOST_CHECK_EQUAL(f(), "misschien");

  x = "no";

  BOOST_CHECK_EQUAL(f(), "nee");
}

BOOST_FIXTURE_TEST_CASE(test_Switch_if_string, test_prelude_basic)
{
  auto x = Var<int>(1);
  auto y = Var("one");

  auto f = *Switch(Case(x == 1, y),
                   Case(x == 2, Const("two")),
                   Case(x == 3, std::string("three")),
                   Case(x == 4, "four"),
                   Default("error"));

  BOOST_CHECK_EQUAL(f(), "one");

  x = 2;

  BOOST_CHECK_EQUAL(f(), "two");

  x = 3;

  BOOST_CHECK_EQUAL(f(), "three");

  x = 4;

  BOOST_CHECK_EQUAL(f(), "four");

  x = 5;

  BOOST_CHECK_EQUAL(f(), "error");
}

BOOST_FIXTURE_TEST_CASE(test_Switch_if_operator, test_prelude_basic)
{
  auto x = Var<int>(1);
  auto y = Var("three");

  auto f = *Switch(x == 1 >>= "one",
                   x == 2 >>= std::string("two"),
                   x == 3 >>= y,
                   x == 4 >>= Const("four"),
                   Default("error"));

  BOOST_CHECK_EQUAL(f(), "one");

  x = 2;

  BOOST_CHECK_EQUAL(f(), "two");

  x = 3;

  BOOST_CHECK_EQUAL(f(), "three");

  x = 4;

  BOOST_CHECK_EQUAL(f(), "four");

  x = 5;

  BOOST_CHECK_EQUAL(f(), "error");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_int, test_prelude_basic)
{
  auto x = Var<int>(42);
  auto y = ToString(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "ToString");

  BOOST_CHECK_EQUAL(f(), "42");

  x = 53;

  BOOST_CHECK_EQUAL(f(), "53");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_string, test_prelude_basic)
{
  auto x = Var("str");
  auto y = ToString(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "var");

  BOOST_CHECK_EQUAL(f(), "str");

  x = "long string";

  BOOST_CHECK_EQUAL(f(), "long string");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_int_leteral, test_prelude_basic)
{
  auto y = ToString(5);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), "5");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_string_leteral, test_prelude_basic)
{
  auto y = ToString("string literal");
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), "string literal");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_concatenation, test_prelude_basic)
{
  auto x = Var<int>(42);
  auto y = Var<float>(42.5);
  auto z = ToString(x, " != ", y);
  auto f = *z;

  BOOST_CHECK_EQUAL(f(), "42 != 42.5");

  x = 43;

  BOOST_CHECK_EQUAL(f(), "43 != 42.5");
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int, test_prelude_basic)
{
  auto x = Var("42");
  auto y = FromString<int>(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "FromString");

  BOOST_CHECK_EQUAL(f(), 42);

  x = "55";

  BOOST_CHECK_EQUAL(f(), 55);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int_default, test_prelude_basic)
{
  auto x = Var("NAN");
  auto y = FromString<int>(x, 5);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "FromString");

  BOOST_CHECK_EQUAL(f(), 5);

  x = "55";

  BOOST_CHECK_EQUAL(f(), 55);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int_from_empty_string,
                        test_prelude_basic)
{
  auto x = Var("");
  auto y = FromString<int>(x, 5);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "FromString");

  BOOST_CHECK_EQUAL(f(), 5);

  x = "55";

  BOOST_CHECK_EQUAL(f(), 55);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_string, test_prelude_basic)
{
  auto x = Var("str");
  auto y = FromString<std::string>(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "var");

  BOOST_CHECK_EQUAL(f(), "str");

  x = "long string";

  BOOST_CHECK_EQUAL(f(), "long string");
}

BOOST_FIXTURE_TEST_CASE(test_FromString_string_default, test_prelude_basic)
{
  auto x = Var("str");
  auto y = FromString<std::string>(x, "never used");
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "var");

  BOOST_CHECK_EQUAL(f(), "str");

  x = "long string";

  BOOST_CHECK_EQUAL(f(), "long string");
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_string_literal, test_prelude_basic)
{
  auto x = FromString<int>("54");
  auto f = *x;

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  BOOST_CHECK_EQUAL(f(), 54);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_string_literal_default,
                        test_prelude_basic)
{
  auto x = FromString<int>("NAN", 3);
  auto y = FromString<int>("39", 10);
  auto f = *(x + y);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), 42);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_empty_string_literal,
                        test_prelude_basic)
{
  auto x = FromString<int>("", 13);
  auto y = FromString<int>("29", 10);
  auto f = *(x + y);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), 42);
}

BOOST_AUTO_TEST_SUITE_END()
}
