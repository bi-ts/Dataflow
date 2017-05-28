
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

BOOST_AUTO_TEST_SUITE_END()
}
