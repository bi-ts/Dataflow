
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

BOOST_AUTO_TEST_SUITE(test_prelude)

// Arithmetic

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

// Comparison

TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Eq, == );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(NotEq, != );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Gr, > );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(Less, < );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(GrEq, >= );
TEST_BINARY_OPERATOR_OVERLOAD_FOR_STRINGS(LessEq, <= );

BOOST_AUTO_TEST_SUITE_END()
}
