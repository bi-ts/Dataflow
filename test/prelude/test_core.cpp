
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

#include "../tools/graph_invariant.h"
#include "../tools/io_fixture.h"

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

template <typename T> class box final : core::aggregate_base
{
private:
  struct data
  {
    ref<T> boxed;
  };

public:
  box() = default;

  explicit box(const ref<T>& value)
  : p_data_(std::make_shared<data>(data{value}))
  {
  }

  bool operator==(const box& other) const
  {
    return p_data_ == other.p_data_;
  }

  bool operator!=(const box& other) const
  {
    return !(*this == other);
  }

  friend std::ostream& operator<<(std::ostream& out, const box&)
  {
    out << "box";
    return out;
  }

  friend ref<T> Boxed(const ref<box>& x)
  {
    struct policy
    {
      static std::string label()
      {
        return "boxed";
      }
      static const ref<T>& calculate(const box& v)
      {
        return v.p_data_->boxed;
      }
    };

    return core::LiftSelector<policy>(x);
  }

  friend ref<T>
  BoxedOrDefault(const ref<box>& x, const ref<bool>& b, const ref<T>& d)
  {
    struct policy
    {
      static std::string label()
      {
        return "boxed-or-default";
      }
      const ref<T>& calculate(const box& x, bool b)
      {
        return b ? x.p_data_->boxed : d_;
      }

      ref<T> d_;
    };

    return core::LiftSelector<policy>(policy{d}, x, b);
  }

private:
  std::shared_ptr<data> p_data_;
};

template <typename T> box<T> make_box(const ref<T>& value)
{
  return box<T>(value);
}

template <typename T> ref<box<T>> Box(const ref<T>& value)
{
  return Const(make_box(value));
}

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

BOOST_AUTO_TEST_CASE(test_Box)
{
  Engine engine;

  auto x = Const<int>(13);
  auto y = Var<int>(22);
  auto z = Var<box<int>>(make_box(x));

  auto a = Boxed(z);

  BOOST_CHECK_EQUAL(introspect::label(a), "boxed");
  BOOST_CHECK(graph_invariant_holds());

  auto b = Curr(a);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(b(), 13);

  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);

  z = make_box(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);

  BOOST_CHECK_EQUAL(b(), 22);

  y = 32;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(b(), 32);
}

BOOST_AUTO_TEST_CASE(test_Box_activation)
{
  Engine engine;

  auto x = Var(0);
  auto y = Var(make_box(x));

  auto a = Curr(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(x), false);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
}

BOOST_AUTO_TEST_CASE(test_Box_conditional)
{
  Engine engine;

  auto x = Var(true);
  auto y = Var(22);
  auto z = If(x, Box(y), Box(Const(1)));

  auto a = Boxed(z);

  BOOST_CHECK_EQUAL(introspect::label(a), "boxed");
  BOOST_CHECK(graph_invariant_holds());

  auto b = Curr(a);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(b(), 22);

  y = 33;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(b(), 33);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(b(), 1);

  y = 44;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(b(), 1);
}

BOOST_AUTO_TEST_CASE(test_Box_LiftSelector_additional_parameters)
{
  Engine engine;

  auto a = Var(11);
  auto b = Box(a);
  auto c = Var(true);
  auto d = Var(22);

  auto e = BoxedOrDefault(b, c, d);

  BOOST_CHECK_EQUAL(introspect::label(e), "boxed-or-default");
  BOOST_CHECK(graph_invariant_holds());

  auto f = Curr(e);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 11);

  a = 2;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 2);

  c = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 22);

  d = 3;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_CurrentTime, test_core_fixture)
{
  const auto x = Main([=](const Time& t0) { return CurrentTime(); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(x(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_ref, test_core_fixture)
{
  BOOST_CHECK_EQUAL((std::is_copy_assignable<ref<int>>::value), false);
  BOOST_CHECK_EQUAL((std::is_copy_constructible<ref<int>>::value), true);
}

BOOST_FIXTURE_TEST_CASE(test_Const, test_core_fixture)
{
  const ref<int> x = Const(17);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 17);
}

BOOST_FIXTURE_TEST_CASE(test_Const_default_constructor, test_core_fixture)
{
  const ref<int> x = Const<int>();

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_Const_forward_args, test_core_fixture)
{
  const auto x = Const(17);
  const auto y = Const<box<int>>(x);

  BOOST_CHECK_EQUAL(introspect::label(y), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto z = Curr(y);

  BOOST_CHECK(graph_invariant_holds());
}

BOOST_FIXTURE_TEST_CASE(test_Const_forward_args_to_string_constructor,
                        test_core_fixture)
{
  const auto x = Const<std::string>(10, '*');

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), "**********");
}

BOOST_FIXTURE_TEST_CASE(test_Const_string_literal, test_core_fixture)
{
  const ref<std::string> x = Const("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), "some text");
}

BOOST_FIXTURE_TEST_CASE(test_Const_via_arg_ctor, test_core_fixture)
{
  std::function<ref<std::string>(const arg<std::string>& x)> fn =
    [](const arg<std::string>& x) { return x; };

  const ref<std::string> x = fn("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), "some text");
}

BOOST_FIXTURE_TEST_CASE(test_Signal, test_core_fixture)
{
  const sig x = Signal();

  BOOST_CHECK_EQUAL(introspect::label(x), "signal");
  BOOST_CHECK(graph_invariant_holds());

  {
    capture_output();

    auto y = Curr(introspect::Log(x, "x"));

    reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(y(), false);

    BOOST_CHECK_EQUAL(log_string(), "x = false;");

    capture_output();

    x.emit();

    reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(y(), false);

    BOOST_CHECK_EQUAL(log_string(),
                      "x = false;x = true;"
                      "x = false;");
  }

  {
    capture_output();

    auto y = Curr(introspect::Log(x, "x"));

    reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(y(), false);

    BOOST_CHECK_EQUAL(log_string(),
                      "x = false;"
                      "x = true;x = false;"
                      "x = false;");

    capture_output();

    x.emit();

    reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(y(), false);

    BOOST_CHECK_EQUAL(log_string(),
                      "x = false;"
                      "x = true;x = false;"
                      "x = false;"
                      "x = true;x = false;");
  }
}

BOOST_FIXTURE_TEST_CASE(test_Snapshot, test_core_fixture)
{
  var<int> x = Var<int>(3);

  const auto y = Main([=](const Time& t0) { return x(t0); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK(!introspect::active_node(x));
  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(y(), 3);

  x = 4;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 3);

  x = 6;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 3);
}

BOOST_FIXTURE_TEST_CASE(test_Var, test_core_fixture)
{
  var<int> x = Var(17);

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 17);

  x = 6;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 6);
}

BOOST_FIXTURE_TEST_CASE(test_Var_default_constructor, test_core_fixture)
{
  auto x = Var<int>();

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_Var_forward_args, test_core_fixture)
{
  const auto a = Const(1);
  const auto b = Const(2);
  auto x = Var<box<int>>(a);

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  x = make_box(b);

  auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
}

BOOST_FIXTURE_TEST_CASE(test_Var_forward_args_to_string_constructor,
                        test_core_fixture)
{
  const auto x = Var<std::string>(10, '*');

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), "**********");
}

BOOST_FIXTURE_TEST_CASE(test_Var_string_literal, test_core_fixture)
{
  var<std::string> x = Var("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), "some text");

  x = "other text";

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), "other text");
}

BOOST_FIXTURE_TEST_CASE(test_Curr, test_core_fixture)
{
  const var<int> x = Var<int>(6);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::ref_count(x), 1);

  const auto y = Curr(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::ref_count(x), 2);

  BOOST_CHECK_EQUAL(introspect::label(y), "main");
  BOOST_CHECK_EQUAL(y(), 6);
}

BOOST_FIXTURE_TEST_CASE(test_Main, test_core_fixture)
{
  var<int> x = Var<int>(6);

  const auto y = Main([=](const Time& t) { return x; });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 6);

  x = 25;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 25);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_var, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);
  auto z = Var<int>(20);

  auto f = *If(x, y, z);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_var, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto z = Var<int>(20);

  auto f = *If(x, 10, z);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_var_int, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(10);

  auto f = *If(x, y, 20);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_int_int, test_core_fixture)
{
  auto x = Var<bool>(true);

  auto f = *If(x, 10, 20);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 20);
}

BOOST_FIXTURE_TEST_CASE(test_If_var_str_strliteral, test_core_fixture)
{
  auto x = Var<bool>(true);

  auto f = *If(x, std::string("str"), "strliteral");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "str");

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "strliteral");
}

BOOST_FIXTURE_TEST_CASE(test_If_fn_fn, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main(If(
    x,
    [=](const Time& t) { return y; },
    [=](const Time& t) { return If(x, y, z); }));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(f(), 11);

  y = 13;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 13);

  const auto count = introspect::num_active_nodes();

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), count + 2);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
  BOOST_CHECK_EQUAL(f(), 12);

  z = 14;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 14);

  x = true;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), count);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(f(), 13);
}

BOOST_FIXTURE_TEST_CASE(test_If_ref_fn, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main(If(x, y, [=](const Time& t0) { return z; }));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(f(), 11);

  y = 34;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 34);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
  BOOST_CHECK_EQUAL(f(), 12);

  z = 15;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 15);
}

BOOST_FIXTURE_TEST_CASE(test_If_fn_ref, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main(If(
    x, [=](const Time& t0) { return y; }, z));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(f(), 11);

  y = 34;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 34);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
  BOOST_CHECK_EQUAL(f(), 12);

  z = 15;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), 15);
}

BOOST_FIXTURE_TEST_CASE(test_If_fn_fn_eagerness, test_core_fixture)
{
  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main([=](const Time& t0) {
    auto zz = If(
      x,
      [=](const Time& t) { return z; },
      [=](const Time& t) { return z; },
      t0);

    return If(x, y, zz);
  });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);

  x = true;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
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
    static char calculate(int v)
    {
      return static_cast<char>(v + 2);
    }
  };

  const auto y = core::Lift(policy(), x);
  const auto z = Curr(y);

  BOOST_CHECK(graph_invariant_holds());
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
    }

  private:
    char cache_;
  };

  const auto y = core::Lift(policy(), x);
  const auto z = Curr(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::label(y), "lowercase");

  BOOST_CHECK_EQUAL(z(), 'c');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_unary_lambda, test_core_fixture)
{
  const auto x = Var<char>('B');

  const auto y =
    core::Lift("trinity", x, [](char c) { return std::string(3, c); });

  const auto z = Curr(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "trinity");

  BOOST_CHECK(graph_invariant_holds());
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

  BOOST_CHECK(graph_invariant_holds());
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
    static char calculate(char v, int offset)
    {
      return static_cast<char>(v + offset);
    }
  };

  const auto z = core::Lift(policy(), x, y);
  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "shift");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), 'E');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_policy_member_func, test_core_fixture)
{
  const var<int> x = Var<int>('C');
  var<bool> y = Var<bool>(true);

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
    }

  private:
    char cache_;
  };

  const auto z = core::Lift(policy(), x, y);
  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "lowercase");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), 'c');

  y = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), 'C');
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_lambda, test_core_fixture)
{
  auto x = Var<char>('B');
  auto y = Var<int>(4);

  const auto z = core::Lift(
    "multiply", x, y, [](char c, int n) { return std::string(n, c); });

  const auto a = Curr(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "multiply");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), "BBBB");

  y = 2;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), "BB");

  x = 'A';

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), "AA");
}

BOOST_FIXTURE_TEST_CASE(test_Lift_binary_function_pointer, test_core_fixture)
{
  auto x = Var<char>('C');
  auto y = Var<int>(3);

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

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), "CCC");

  y = 2;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), "CC");

  x = 'A';

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(a(), "AA");
}

BOOST_FIXTURE_TEST_CASE(test_Lift_n_ary_policy_static_func, test_core_fixture)
{
  var<std::string> a = Var("text");
  var<char> b = Var('A');
  var<int> c = Var(4);
  var<double> d = Var(3.14);

  struct policy
  {
    static std::string label()
    {
      return "4-ary";
    }
    static std::string calculate(std::string s, char c, int count, double v)
    {
      return s + "/" + std::string(count, c) + "/" +
             std::to_string(static_cast<int>(v * 100)) + "%";
    }
  };

  const auto e = core::Lift<policy>(a, b, c, d);
  const auto f = Curr(e);

  BOOST_CHECK_EQUAL(introspect::label(e), "4-ary");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "text/AAAA/314%");

  b = 'B';

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "text/BBBB/314%");

  c = 6;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "text/BBBBBB/314%");

  d = 1.11;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "text/BBBBBB/111%");

  a = "other-text";

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(f(), "other-text/BBBBBB/111%");
}

BOOST_FIXTURE_TEST_CASE(test_LiftPuller_n_ary_policy_static_func,
                        test_core_fixture)
{
  const auto a = Var(33);
  const auto b = Var(44);
  const auto c = Var(55);

  struct policy
  {
    static std::string label()
    {
      return "test-sum-3";
    }
    static int calculate(int x, int y, int z)
    {
      return x + y + z;
    }
  };

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(a), false);
  BOOST_CHECK_EQUAL(introspect::active_node(b), false);
  BOOST_CHECK_EQUAL(introspect::active_node(c), false);

  const auto d = core::LiftPuller<policy>(a, b, c);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(a), true);
  BOOST_CHECK_EQUAL(introspect::active_node(b), true);
  BOOST_CHECK_EQUAL(introspect::active_node(c), true);

  const auto e = Main([d](const Time&) { return d; });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(e(), 132);
}

BOOST_FIXTURE_TEST_CASE(test_Curr_operator, test_core_fixture)
{
  const val<int> x = *Var<int>(15);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::label(x), "main");
  BOOST_CHECK_EQUAL(x(), 15);
}

BOOST_FIXTURE_TEST_CASE(test_Prev, test_core_fixture)
{
  const ref<int> v0 = Var<int>(1);
  var<int> x = Var<int>(3);

  capture_output();

  const auto z =
    Main([=](const Time& t0) { return introspect::Log(Prev(v0, x, t0)); });

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(v0), false);
  BOOST_CHECK_EQUAL(introspect::active_node(x), true);

  BOOST_CHECK_EQUAL(log_string(), "prev = 1;prev = 3;");

  capture_output();

  x = 5;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 1;prev = 3;prev = 5;");

  capture_output();

  x = 9;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 1;prev = 3;prev = 5;prev = 9;");
}

BOOST_FIXTURE_TEST_CASE(test_Prev_deferred_use, test_core_fixture)
{
  const ref<int> v0 = Var<int>(1);
  var<int> x = Var<int>(3);
  var<bool> b = Var<bool>(false);

  capture_output();

  const auto z = Main(
    [=](const Time& t0) { return If(b, introspect::Log(Prev(v0, x, t0)), 0); });

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(v0), false);
  BOOST_CHECK_EQUAL(introspect::active_node(x), true);

  capture_output();

  x = 7;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "");

  capture_output();

  b = true;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 7;");

  capture_output();

  x = 5;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 7;prev = 5;");

  capture_output();

  x = 9;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 7;prev = 5;prev = 9;");
}

BOOST_FIXTURE_TEST_CASE(test_Prev_overloads, test_core_fixture)
{
  var<int> x = Var<int>(3);

  capture_output();

  const auto z = Main([=](const Time& t0) {
    return introspect::Log(Prev(x, Prev(33, x, t0), t0));
  });

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 3;prev = 33;prev = 3;");

  capture_output();

  x = 10;

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "prev = 3;prev = 33;prev = 3;prev = 10;");
}

BOOST_FIXTURE_TEST_CASE(test_StateMachine, test_core_fixture)
{
  var<char> x = Var<char>('a');

  capture_output();

  const auto y = Main([=](const Time& t0) {
    const auto tf = [=](ref<int> s) {
      struct policy
      {
        static std::string label()
        {
          return "test-state-machine";
        }
        int calculate(int s, char x)
        {
          switch (s)
          {
          case 0:
            return x == 'a' ? 1 : s;
          case 1:
            return x == 'b' ? 2 : s;
          case 2:
            return x == 'c' ? 3 : s;
          case 3:
            return x == 'd' ? 4 : s;
          };
          return s;
        }
      };

      return core::Lift<policy>(s, x);
    };

    const auto s = StateMachine(0, tf, t0);

    return introspect::Log(s);
  });

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "state = 0;state = 1;");

  capture_output();

  x = 'b';

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "state = 0;state = 1;state = 2;");

  capture_output();

  x = 'd';

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "state = 0;state = 1;state = 2;");

  capture_output();

  x = 'c';

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(), "state = 0;state = 1;state = 2;state = 3;");

  capture_output();

  x = 'd';

  reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(log_string(),
                    "state = 0;state = 1;state = 2;state = 3;state = 4;");
}

BOOST_AUTO_TEST_CASE(
  test_StateMachine_transition_function_requires_initialization)
{
  Engine engine;

  auto x = Var<int>(1);

  const auto tf = [=](const ref<int>&) {
    return [=](const Time& t0) { return x(t0); };
  };

  const auto y = Main([=](const Time& t0) { return StateMachine(0, tf, t0); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 1);

  x = 33;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(y(), 1);
}

BOOST_AUTO_TEST_CASE(test_core_to_string_ref)
{
  Engine engine;

  const auto x = Const(0);

  std::stringstream ss;
  ss << std::hex << x.id();

  BOOST_CHECK_EQUAL(core::to_string(x), ss.str());
}

BOOST_AUTO_TEST_CASE(test_core_to_string_flowable)
{
  Engine engine;

  BOOST_CHECK_EQUAL(core::to_string(1), "1");
}

BOOST_AUTO_TEST_CASE(test_core_to_string_convertible_to_flowable)
{
  Engine engine;

  BOOST_CHECK_EQUAL(core::to_string("str"), "str");
}

BOOST_AUTO_TEST_CASE(test_Since)
{
  Engine engine;

  auto x = Var(0);
  auto use_since = Var(true);

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

  const auto y = core::Lift(policy(), x);

  // t = 0
  const auto m = Main([=](const Time& t0) {
    const auto z = If(
      y,
      [](const Time& t0) { return Const(dtimestamp(t0)); },
      [](const Time& t0) { return Const(dtimestamp(t0)); },
      t0);

    return If(
      use_since,
      Since(z,
            [](const Time& t0) { return Const<std::size_t>(dtimestamp(t0)); }),
      std::size_t(101010),
      t0);
  });

  BOOST_CHECK_EQUAL(m(), 0);

  // t = 1
  x = 10;

  BOOST_CHECK_EQUAL(m(), 0);

  // t = 2
  x = 100;

  BOOST_CHECK_EQUAL(m(), 2);

  // t = 3
  x = 110;

  BOOST_CHECK_EQUAL(m(), 2);

  // t = 4
  use_since = false;

  BOOST_CHECK_EQUAL(m(), 101010);

  // t = 5
  use_since = true;

  BOOST_CHECK_EQUAL(m(), 5);
}

BOOST_AUTO_TEST_SUITE_END()
}
