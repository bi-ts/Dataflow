
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

#include "../tools/io_fixture.h"

#include <dataflow/prelude/core.h>
#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

class test_core_fixture : public io_fixture
{
protected:
  test_core_fixture()
  {
  }

private:
  Engine engine_;
};

BOOST_AUTO_TEST_SUITE(test_core)

BOOST_FIXTURE_TEST_CASE(test_Const, test_core_fixture)
{
  const ref<int> x = Const(17);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  const auto y = Curr(x);

  BOOST_CHECK_EQUAL(y(), 17);
}

BOOST_FIXTURE_TEST_CASE(test_Const_no_params, test_core_fixture)
{
  const ref<int> x = Const<int>();

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  auto y = Curr(x);

  BOOST_CHECK_EQUAL(y(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_Const_string_literal, test_core_fixture)
{
  const ref<std::string> x = Const("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  auto y = Curr(x);

  BOOST_CHECK_EQUAL(y(), "some text");
}

BOOST_FIXTURE_TEST_CASE(test_Snapshot, test_core_fixture)
{
  const var<int> x = Var<int>(3);

  const auto y = Main([=](const Time& t0)
                      {
                        return x(t0);
                      });

  BOOST_CHECK(!introspect::active_node(x));

  BOOST_CHECK_EQUAL(y(), 3);

  x = 4;

  BOOST_CHECK_EQUAL(y(), 3);

  x = 6;

  BOOST_CHECK_EQUAL(y(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_Var, test_core_fixture)
{
  const var<int> x = Var(17);

  BOOST_CHECK_EQUAL(introspect::label(x), "var");

  auto y = Curr(x);

  BOOST_CHECK_EQUAL(y(), 17);

  x = 6;

  BOOST_CHECK_EQUAL(y(), 6);
}

BOOST_FIXTURE_TEST_CASE(test_Var_default, test_core_fixture)
{
  auto y = Curr(Var<int>());

  BOOST_CHECK_EQUAL(y(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_Var_string_literal, test_core_fixture)
{
  const var<std::string> x = Var("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "var");

  auto y = Curr(x);

  BOOST_CHECK_EQUAL(y(), "some text");

  x = "other text";

  BOOST_CHECK_EQUAL(y(), "other text");
}

BOOST_FIXTURE_TEST_CASE(test_Curr, test_core_fixture)
{
  const var<int> x = Var<int>(6);

  BOOST_CHECK_EQUAL(introspect::ref_count(x), 1);

  const auto y = Curr(x);

  BOOST_CHECK_EQUAL(introspect::ref_count(x), 2);

  BOOST_CHECK_EQUAL(introspect::label(y), "main");
  BOOST_CHECK_EQUAL(y(), 6);
}

BOOST_FIXTURE_TEST_CASE(test_Main, test_core_fixture)
{
  const var<int> x = Var<int>(6);

  const auto y = Main([=](const Time& t)
                      {
                        return x;
                      });

  BOOST_CHECK_EQUAL(y(), 6);

  x = 25;

  BOOST_CHECK_EQUAL(y(), 25);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_var, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);
  auto z = Var<int>(20);

  auto f = *If(x, y, z);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_var, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto z = Var<int>(20);

  auto f = *If(x, 10, z);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_int, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);

  auto f = *If(x, y, 20);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_int, test_core_fixture)
{
  auto x = Var<bool>(true);

  auto f = *If(x, 10, 20);

  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_policy_static_func, test_core_fixture)
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

  const auto y = core::Lift(x, policy());
  const auto z = Curr(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "shift");

  BOOST_CHECK_EQUAL(z(), 'C');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_policy_member_func, test_core_fixture)
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

  const auto y = core::Lift(x, policy());
  const auto z = Curr(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "lowercase");

  BOOST_CHECK_EQUAL(z(), 'c');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_lambda, test_core_fixture)
{
  const auto x = Var<char>('B');

  const auto y = core::Lift("trinity",
                            x,
                            [](char c)
                            {
                              return std::string(3, c);
                            });

  const auto z = Curr(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "trinity");

  BOOST_CHECK_EQUAL(z(), "BBB");
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_function_pointer, test_core_fixture)
{
  const auto x = Var<char>('C');

  struct tool
  {
    static std::string duplicate(char c)
    {
      return std::string(2, c);
    };
  };

  const auto y = core::Lift("duplicate", x, tool::duplicate);

  const auto z = Curr(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "duplicate");

  BOOST_CHECK_EQUAL(z(), "CC");
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_policy_static_func, test_core_fixture)
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

  const auto z = core::Lift(x, y, policy());
  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "shift");

  BOOST_CHECK_EQUAL(a(), 'E');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_policy_member_func, test_core_fixture)
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

  const auto z = core::Lift(x, y, policy());
  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "lowercase");

  BOOST_CHECK_EQUAL(a(), 'c');

  y = false;

  BOOST_CHECK_EQUAL(a(), 'C');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_lambda, test_core_fixture)
{
  const auto x = Var<char>('B');
  const auto y = Var<int>(4);

  const auto z = core::Lift("multiply",
                            x,
                            y,
                            [](char c, int n)
                            {
                              return std::string(n, c);
                            });

  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "multiply");

  BOOST_CHECK_EQUAL(a(), "BBBB");

  y = 2;

  BOOST_CHECK_EQUAL(a(), "BB");

  x = 'A';

  BOOST_CHECK_EQUAL(a(), "AA");
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_function_pointer, test_core_fixture)
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

  const auto z = core::Lift("multiply", x, y, tool::multiply);

  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "multiply");

  BOOST_CHECK_EQUAL(a(), "CCC");

  y = 2;

  BOOST_CHECK_EQUAL(a(), "CC");

  x = 'A';

  BOOST_CHECK_EQUAL(a(), "AA");
}

BOOST_FIXTURE_TEST_CASE(test_Curr_operator, test_core_fixture)
{
  const eager<int> x = *Var<int>(15);

  BOOST_CHECK_EQUAL(introspect::label(x), "main");
  BOOST_CHECK_EQUAL(x(), 15);
}

BOOST_FIXTURE_TEST_CASE(test_Prev, test_core_fixture)
{
  const var<int> x = Var<int>(3);

  struct tool
  {
    static ref<int> WriteInt(ref<int> x)
    {
      return core::Lift("output",
                        x,
                        [](int v)
                        {
                          std::cout << v << std::endl;
                          return v;
                        });
    }
  };

  capture_output();

  const auto y = Main([=](const Time& t)
                      {
                        return tool::WriteInt(Prev(t, Const(1), x));
                      });

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "1;3;");

  capture_output();

  x = 5;

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "1;3;5;");

  capture_output();

  x = 9;

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "1;3;5;9;");
}

BOOST_AUTO_TEST_SUITE_END()
}
