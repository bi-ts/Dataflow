
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
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

#include "../tools/EngineTest.h"
#include "../tools/graph_invariant.h"
#include "../tools/io_fixture.h"

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

struct test_policy_base
{
  // Make sure that a move-only policy is enough for lifters
  test_policy_base() = default;
  test_policy_base(const test_policy_base&) = delete;
  test_policy_base(test_policy_base&&) = default;
};

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

BOOST_AUTO_TEST_SUITE(test_core)

BOOST_AUTO_TEST_CASE(test_Box)
{
  EngineTest engine;

  auto x = Const<int>(13);
  auto y = Var<int>(22);
  auto z = Var<box<int>>(make_box(x));

  auto a = Boxed(z);

  BOOST_CHECK_EQUAL(introspect::label(a), "boxed");
  BOOST_CHECK(graph_invariant_holds());

  auto b = Main(a);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*b, 13);

  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);

  z = make_box(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);

  BOOST_CHECK_EQUAL(*b, 22);

  y = 32;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*b, 32);
}

BOOST_AUTO_TEST_CASE(test_Box_activation)
{
  EngineTest engine;

  auto x = Var(0);
  auto y = Var(make_box(x));

  auto a = Main(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(x), false);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
}

BOOST_AUTO_TEST_CASE(test_Box_conditional)
{
  EngineTest engine;

  auto x = Var(true);
  auto y = Var(22);
  auto z = If(x, Box(y), Box(Const(1)));

  auto a = Boxed(z);

  BOOST_CHECK_EQUAL(introspect::label(a), "boxed");
  BOOST_CHECK(graph_invariant_holds());

  auto b = Main(a);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*b, 22);

  y = 33;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*b, 33);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*b, 1);

  y = 44;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*b, 1);
}

BOOST_AUTO_TEST_CASE(test_Box_LiftSelector_additional_parameters)
{
  EngineTest engine;

  auto a = Var(11);
  auto b = Box(a);
  auto c = Var(true);
  auto d = Var(22);

  auto e = BoxedOrDefault(b, c, d);

  BOOST_CHECK_EQUAL(introspect::label(e), "boxed-or-default");
  BOOST_CHECK(graph_invariant_holds());

  auto f = Main(e);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 11);

  a = 2;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 2);

  c = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 22);

  d = 3;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 3);
}

BOOST_AUTO_TEST_CASE(test_CurrentTime)
{
  EngineTest engine;

  const auto x = Main([=](dtime t0) { return CurrentTime(); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*x, 0);
}

BOOST_AUTO_TEST_CASE(test_ref)
{
  EngineTest engine;

  BOOST_CHECK_EQUAL((std::is_copy_assignable<ref<int>>::value), false);
  BOOST_CHECK_EQUAL((std::is_copy_constructible<ref<int>>::value), true);
}

BOOST_AUTO_TEST_CASE(test_Const)
{
  EngineTest engine;

  const ref<int> x = Const(17);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 17);
}

BOOST_AUTO_TEST_CASE(test_Const_default_constructor)
{
  EngineTest engine;

  const ref<int> x = Const<int>();

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 0);
}

BOOST_AUTO_TEST_CASE(test_Const_forward_args)
{
  EngineTest engine;

  const auto x = Const(17);
  const auto y = Const<box<int>>(x);

  BOOST_CHECK_EQUAL(introspect::label(y), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto z = Main(y);

  BOOST_CHECK(graph_invariant_holds());
}

BOOST_AUTO_TEST_CASE(test_Const_forward_args_to_string_constructor)
{
  EngineTest engine;

  const auto x = Const<std::string>(10, '*');

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, "**********");
}

BOOST_AUTO_TEST_CASE(test_Const_string_literal)
{
  EngineTest engine;

  const ref<std::string> x = Const("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, "some text");
}

BOOST_AUTO_TEST_CASE(test_Const_via_arg_ctor)
{
  EngineTest engine;

  std::function<ref<std::string>(const arg<std::string>& x)> fn =
    [](const arg<std::string>& x) { return x; };

  const ref<std::string> x = fn("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, "some text");
}

BOOST_AUTO_TEST_CASE(test_regression_arg_ctor_mixed_types)
{
  EngineTest engine;

  const arg<std::uint16_t> x{44};

  const auto y = Main(x);

  BOOST_CHECK_EQUAL(*y, 44);
}

BOOST_AUTO_TEST_CASE(test_regression_arg_ctor_forward_arg)
{
  EngineTest engine;

  std::unique_ptr<int> p_int{new int{275}};

  const arg<std::shared_ptr<int>> x{std::move(p_int)};

  const auto y = Main(x);

  BOOST_CHECK_EQUAL(**y, 275);
}

BOOST_AUTO_TEST_CASE(test_Signal)
{
  EngineTest engine;

  io_fixture io;

  const sig x = Signal();

  BOOST_CHECK_EQUAL(introspect::label(x), "signal");
  BOOST_CHECK(graph_invariant_holds());

  {
    io.capture_output();

    auto y = Main(introspect::Log(x, "x"));

    io.reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(*y, false);

    BOOST_CHECK_EQUAL(io.log_string(), "[t=0] x = false;");

    io.capture_output();

    x();

    io.reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(*y, false);

    BOOST_CHECK_EQUAL(io.log_string(),
                      "[t=0] x = false;"
                      "[t=1] x = true;[t=2] x = false;");
  }

  {
    io.capture_output();

    auto y = Main(introspect::Log(x, "x"));

    io.reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(*y, false);

    BOOST_CHECK_EQUAL(io.log_string(),
                      "[t=0] x = false;"
                      "[t=1] x = true;[t=2] x = false;"
                      "[t=3] x = false;");

    io.capture_output();

    x();

    io.reset_output();

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL(*y, false);

    BOOST_CHECK_EQUAL(io.log_string(),
                      "[t=0] x = false;"
                      "[t=1] x = true;[t=2] x = false;"
                      "[t=3] x = false;"
                      "[t=4] x = true;[t=5] x = false;");
  }
}

BOOST_AUTO_TEST_CASE(test_Signal_as_ref)
{
  EngineTest engine;

  const sig x = Signal();

  const auto m = Main([x = x.as_ref()](dtime) { return x; });

  BOOST_CHECK_EQUAL(*m, false);
}

BOOST_AUTO_TEST_CASE(test_Snapshot)
{
  EngineTest engine;

  var<int> x = Var<int>(3);

  const auto y = Main([x = x.as_ref()](dtime t0) { return x(t0); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK(!introspect::active_node(x));
  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*y, 3);

  x = 4;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 3);

  x = 6;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 3);
}

BOOST_AUTO_TEST_CASE(test_Var)
{
  EngineTest engine;

  var<int> x = Var(1);

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(introspect::current_time(), -1);

  x = 17;

  BOOST_CHECK_EQUAL(introspect::current_time(), -1);

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(introspect::current_time(), 0);

  BOOST_CHECK_EQUAL(*y, 17);

  x = 6;

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(introspect::current_time(), 1);

  BOOST_CHECK_EQUAL(*y, 6);

  x = 6;

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(introspect::current_time(), 1);

  BOOST_CHECK_EQUAL(*y, 6);

  x = 7;

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(introspect::current_time(), 2);

  BOOST_CHECK_EQUAL(*y, 7);
}

BOOST_AUTO_TEST_CASE(test_Var_indirection)
{
  EngineTest engine;

  var<int> x = Var(6);

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*x, 6);

  x = 1;

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*x, 1);
  BOOST_CHECK_EQUAL(*y, 1);

  x = 2020;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*x, 2020);
  BOOST_CHECK_EQUAL(*y, 2020);
}

BOOST_AUTO_TEST_CASE(test_Var_default_constructor)
{
  EngineTest engine;

  auto x = Var<int>();

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 0);
}

BOOST_AUTO_TEST_CASE(test_Var_forward_args)
{
  EngineTest engine;

  const auto a = Const(1);
  const auto b = Const(2);
  auto x = Var<box<int>>(a);

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  x = make_box(b);

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
}

BOOST_AUTO_TEST_CASE(test_Var_forward_args_to_string_constructor)
{
  EngineTest engine;

  const auto x = Var<std::string>(10, '*');

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  const auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, "**********");
}

BOOST_AUTO_TEST_CASE(test_Var_string_literal)
{
  EngineTest engine;

  var<std::string> x = Var("some text");

  BOOST_CHECK_EQUAL(introspect::label(x), "var");
  BOOST_CHECK(graph_invariant_holds());

  auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, "some text");

  x = "other text";

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, "other text");
}

BOOST_AUTO_TEST_CASE(test_Var_copy_constructor)
{
  EngineTest engine;

  auto x = Var<int>();

  auto y = x;

  auto z = std::move(x);

  BOOST_CHECK_EQUAL((std::is_convertible<var<int>&, var<int>>::value), true);
  BOOST_CHECK_EQUAL((std::is_convertible<var<int>&&, var<int>>::value), true);

#ifdef DATAFLOW_CONFIG_VAR_NO_CONST_COPY_CTOR
  BOOST_CHECK_EQUAL((std::is_convertible<const var<int>&, var<int>>::value),
                    false);
#endif
}

BOOST_AUTO_TEST_CASE(test_Var_deferred_assign)
{
  EngineTest engine;

  io_fixture io;

  var<int> x = Var<int>(0);

  struct policy
  {
    static std::string label()
    {
      return "double-step";
    }
    int calculate(int v)
    {
      x_ = v % 2 ? v + 1 : v;
      return v * 100;
    }

    var<int> x_;
  };

  const auto y = core::Lift(policy{x}, x);

  io.capture_output();

  const auto z = Main(introspect::Log(y, "y"));

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(), "[t=0] y = 0;");

  BOOST_CHECK_EQUAL(*z, 0);

  io.capture_output();

  x = 1;

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] y = 0;[t=1] y = 100;[t=2] y = 200;");

  BOOST_CHECK_EQUAL(*z, 200);
}

BOOST_AUTO_TEST_CASE(test_Cast_double_to_int)
{
  EngineTest engine;

  auto x = Var(3.14);
  const auto y = Cast<int>(x);
  const auto m = Main(y);

  BOOST_CHECK_EQUAL(*m, 3);

  x = 2.17;

  BOOST_CHECK_EQUAL(*m, 2);

  x = 4.99;

  BOOST_CHECK_EQUAL(*m, 4);
}

BOOST_AUTO_TEST_CASE(test_Main_ref_arg)
{
  EngineTest engine;

  const var<int> x = Var<int>(6);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::ref_count(x), 1);

  const auto y = Main(x);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::ref_count(x), 2);

  BOOST_CHECK_EQUAL(introspect::label(y), "main");
  BOOST_CHECK_EQUAL(*y, 6);
}

BOOST_AUTO_TEST_CASE(test_Main_init_func_arg)
{
  EngineTest engine;

  var<int> x = Var<int>(6);

  const auto y = Main([x = x.as_ref()](dtime t) { return x; });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 6);

  x = 25;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 25);
}

BOOST_AUTO_TEST_CASE(test_If_var_var_var)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto y = Var<int>(10);
  auto z = Var<int>(20);

  auto f = Main(If(x, y, z));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 20);
}

BOOST_AUTO_TEST_CASE(test_If_var_int_var)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto z = Var<int>(20);

  auto f = Main(If(x, 10, z));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 20);
}

BOOST_AUTO_TEST_CASE(test_If_var_var_int)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto y = Var<int>(10);

  auto f = Main(If(x, y, 20));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 20);
}

BOOST_AUTO_TEST_CASE(test_If_var_int_int)
{
  EngineTest engine;

  auto x = Var<bool>(true);

  auto f = Main(If(x, 10, 20));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 10);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 20);
}

BOOST_AUTO_TEST_CASE(test_If_var_str_strliteral)
{
  EngineTest engine;

  auto x = Var<bool>(true);

  auto f = Main(If(x, std::string("str"), "strliteral"));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "str");

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "strliteral");
}

BOOST_AUTO_TEST_CASE(test_If_fn_fn)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main(If(
    x,
    [y = y.as_ref()](dtime t) { return y; },
    [x = x.as_ref(), y = y.as_ref(), z = z.as_ref()](dtime t) {
      return If(x, y, z);
    }));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(*f, 11);

  y = 13;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 13);

  const auto count = introspect::num_active_nodes();

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), count + 2);
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
  BOOST_CHECK_EQUAL(*f, 12);

  z = 14;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 14);

  x = true;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::num_active_nodes(), count);
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(*f, 13);
}

BOOST_AUTO_TEST_CASE(test_If_ref_fn)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main(If(x, y, [z = z.as_ref()](dtime t0) { return z; }));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(*f, 11);

  y = 34;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 34);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
  BOOST_CHECK_EQUAL(*f, 12);

  z = 15;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 15);
}

BOOST_AUTO_TEST_CASE(test_If_fn_ref)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main(If(
    x, [y = y.as_ref()](dtime t0) { return y; }, z));

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), true);
  BOOST_CHECK_EQUAL(introspect::active_node(z), false);
  BOOST_CHECK_EQUAL(*f, 11);

  y = 34;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 34);

  x = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::active_node(y), false);
  BOOST_CHECK_EQUAL(introspect::active_node(z), true);
  BOOST_CHECK_EQUAL(*f, 12);

  z = 15;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, 15);
}

BOOST_AUTO_TEST_CASE(test_If_fn_fn_eagerness)
{
  EngineTest engine;

  auto x = Var<bool>(true);
  auto y = Var<int>(11);
  auto z = Var<int>(12);

  auto f = Main([x = x.as_ref(), y = y.as_ref(), z = z.as_ref()](dtime t0) {
    auto zz = If(
      x, [=](dtime t) { return z; }, [=](dtime t) { return z; }, t0);

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

BOOST_AUTO_TEST_CASE(test_Lift_policy_member_func_no_args)
{
  EngineTest engine;

  struct policy
  {
    static std::string label()
    {
      return "no-args";
    }

    int calculate()
    {
      return 19042020;
    }
  };

  const auto x = core::Lift<policy>();

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  const auto y = Main(x);

  BOOST_CHECK_EQUAL(*y, 19042020);
}

BOOST_AUTO_TEST_CASE(test_Lift_unary_policy_static_func)
{
  EngineTest engine;

  const var<int> x = Var<int>('A');

  struct policy : test_policy_base
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
  const auto z = Main(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::label(y), "shift");

  BOOST_CHECK_EQUAL(*z, 'C');
}

BOOST_AUTO_TEST_CASE(test_Lift_unary_policy_member_func)
{
  EngineTest engine;

  const var<int> x = Var<int>('C');

  struct policy : test_policy_base
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
  const auto z = Main(y);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(introspect::label(y), "lowercase");

  BOOST_CHECK_EQUAL(*z, 'c');
}

BOOST_AUTO_TEST_CASE(test_Lift_unary_lambda)
{
  EngineTest engine;

  const auto x = Var<char>('B');

  const auto y =
    core::Lift("trinity", x, [](char c) { return std::string(3, c); });

  const auto z = Main(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "trinity");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*z, "BBB");
}

BOOST_AUTO_TEST_CASE(test_Lift_unary_function_pointer)
{
  EngineTest engine;

  const auto x = Var<char>('C');

  struct tool
  {
    static std::string duplicate(char c)
    {
      return std::string(2, c);
    };
  };

  const auto y = core::Lift("duplicate", x, tool::duplicate);

  const auto z = Main(y);

  BOOST_CHECK_EQUAL(introspect::label(y), "duplicate");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*z, "CC");
}

BOOST_AUTO_TEST_CASE(test_Lift_binary_policy_static_func)
{
  EngineTest engine;

  const var<char> x = Var<char>('A');
  const var<int> y = Var<int>(4);

  struct policy : test_policy_base
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
  const auto a = Main(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "shift");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, 'E');
}

BOOST_AUTO_TEST_CASE(test_Lift_binary_policy_member_func)
{
  EngineTest engine;

  const var<int> x = Var<int>('C');
  var<bool> y = Var<bool>(true);

  struct policy : test_policy_base
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
  const auto a = Main(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "lowercase");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, 'c');

  y = false;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, 'C');
}

BOOST_AUTO_TEST_CASE(test_Lift_binary_lambda)
{
  EngineTest engine;

  auto x = Var<char>('B');
  auto y = Var<int>(4);

  const auto z = core::Lift(
    "multiply", x, y, [](char c, int n) { return std::string(n, c); });

  const auto a = Main(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "multiply");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, "BBBB");

  y = 2;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, "BB");

  x = 'A';

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, "AA");
}

BOOST_AUTO_TEST_CASE(test_Lift_binary_function_pointer)
{
  EngineTest engine;

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

  const auto a = Main(z);

  BOOST_CHECK_EQUAL(introspect::label(z), "multiply");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, "CCC");

  y = 2;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, "CC");

  x = 'A';

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*a, "AA");
}

BOOST_AUTO_TEST_CASE(test_Lift_n_ary_policy_static_func)
{
  EngineTest engine;

  var<std::string> a = Var("text");
  var<char> b = Var('A');
  var<int> c = Var(4);
  var<double> d = Var(3.14);

  struct policy : test_policy_base
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
  const auto f = Main(e);

  BOOST_CHECK_EQUAL(introspect::label(e), "4-ary");

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "text/AAAA/314%");

  b = 'B';

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "text/BBBB/314%");

  c = 6;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "text/BBBBBB/314%");

  d = 1.11;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "text/BBBBBB/111%");

  a = "other-text";

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*f, "other-text/BBBBBB/111%");
}

BOOST_AUTO_TEST_CASE(test_LiftUpdater_policy_member_func_no_args)
{
  EngineTest engine;

  struct policy
  {
    static std::string label()
    {
      return "updater-no-args";
    }

    int calculate()
    {
      return 19042020;
    }
  };

  const auto x = core::LiftUpdater<policy>();

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  const auto y = Main(x);

  BOOST_CHECK_EQUAL(*y, 19042020);
}

BOOST_AUTO_TEST_CASE(test_LiftUpdater_binary_func)
{
  EngineTest engine;

  var<char> x = Var<char>('A');
  var<int> y = Var<int>(4);

  class data
  {
  public:
    explicit data(char c, int offset)
    : original_(c)
    , shifted_(static_cast<char>(c + offset))
    , update_count_(0)
    {
    }

    char original() const
    {
      return original_;
    }

    char shifted() const
    {
      return shifted_;
    }

    char shift(int offset)
    {
      ++update_count_;

      return shifted_ = static_cast<char>(original_ + offset);
    }

    int update_count() const
    {
      return update_count_;
    }

  private:
    char original_;
    char shifted_;
    int update_count_;
  };

  struct policy : test_policy_base
  {
    static std::string label()
    {
      return "shift_update";
    }

    static std::shared_ptr<data> calculate(char c, int offset)
    {
      return std::make_shared<data>(c, offset);
    }

    static std::shared_ptr<data>
    update(const std::shared_ptr<data>& value, char c, int offset)
    {
      if (value->original() != c)
        return calculate(c, offset);

      value->shift(offset);

      return value;
    }
  };

  std::vector<std::shared_ptr<data>> ptrs;

  const auto z = core::LiftUpdater<policy>(x, y);

  {
    const auto a = Main(z);

    BOOST_CHECK_EQUAL(introspect::label(z), "shift_update");

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL((*a)->shifted(), 'E');
    BOOST_CHECK_EQUAL((*a)->update_count(), 0);

    ptrs.push_back(*a);

    y = 2;

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL((*a)->shifted(), 'C');
    BOOST_CHECK_EQUAL((*a)->update_count(), 1);
    BOOST_CHECK_EQUAL(*a, ptrs.back());

    y = 3;

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL((*a)->shifted(), 'D');
    BOOST_CHECK_EQUAL((*a)->update_count(), 2);
    BOOST_CHECK_EQUAL(*a, ptrs.back());

    x = 'C';

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL((*a)->shifted(), 'F');
    BOOST_CHECK_EQUAL((*a)->update_count(), 0);
    BOOST_CHECK_NE(*a, ptrs.back());

    ptrs.push_back(*a);

    y = 2;

    BOOST_CHECK(graph_invariant_holds());
    BOOST_CHECK_EQUAL((*a)->shifted(), 'E');
    BOOST_CHECK_EQUAL((*a)->update_count(), 1);
    BOOST_CHECK_EQUAL(*a, ptrs.back());
  }

  for (const auto& ptr : ptrs)
  {
    BOOST_CHECK_EQUAL(ptr.use_count(), 1);
  }
}

BOOST_AUTO_TEST_CASE(test_LiftPuller_n_ary_policy_static_func)
{
  EngineTest engine;

  const auto a = Var(33);
  const auto b = Var(44);
  const auto c = Var(55);

  struct policy : test_policy_base
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

  const auto e = Main([d](dtime) { return d; });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*e, 132);
}

BOOST_AUTO_TEST_CASE(test_Recursion)
{
  EngineTest engine;

  io_fixture io;

  var<char> x = Var<char>('a');

  io.capture_output();

  const auto y = Main([x = x.as_ref()](dtime t0) {
    const auto tf = [=](ref<int> s) {
      struct policy : test_policy_base
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

    const auto s = Recursion(0, tf, t0);

    return introspect::Log(s);
  });

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] recursion = 0;[t=1] recursion = 1;");

  io.capture_output();

  x = 'b'; // t=2

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(
    io.log_string(),
    "[t=0] recursion = 0;[t=1] recursion = 1;[t=3] recursion = 2;");

  io.capture_output();

  x = 'd'; // t=4

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(
    io.log_string(),
    "[t=0] recursion = 0;[t=1] recursion = 1;[t=3] recursion = 2;");

  io.capture_output();

  x = 'c'; // t=5

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] recursion = 0;[t=1] recursion = 1;"
                    "[t=3] recursion = 2;[t=6] recursion = 3;");

  io.capture_output();

  x = 'd'; // t=7

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] recursion = 0;[t=1] recursion = 1;"
                    "[t=3] recursion = 2;[t=6] recursion = 3;"
                    "[t=8] recursion = 4;");
}

BOOST_AUTO_TEST_CASE(
  test_Recursion_recursion_definition_function_requires_initialization)
{
  EngineTest engine;

  auto x = Var<int>(1);

  const auto tf = [x = x.as_ref()](const ref<int>&) {
    return [=](dtime t0) { return x(t0); };
  };

  const auto y = Main([=](dtime t0) { return Recursion(0, tf, t0); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 1);

  x = 33;

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, 1);
}

BOOST_AUTO_TEST_CASE(
  test_Recursion_recursion_definition_function_creates_eager_node)
{
  EngineTest engine;

  const auto const_true = Const<bool>(true);
  const auto const_false = Const<bool>(false);

  const auto tf = [=](const ref<bool>& sp) {
    return [=](dtime t0) {
      return If(
        sp,
        [=](dtime t0) { return const_true; },
        [=](dtime t0) { return const_false; },
        t0);
    };
  };

  const auto z = Main([=](dtime t0) { return Recursion(true, tf, t0); });

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*z, true);
}

BOOST_AUTO_TEST_CASE(test_core_to_string_ref)
{
  EngineTest engine;

  const auto x = Const(0);

  std::stringstream ss;
  ss << std::hex << x.id();

  BOOST_CHECK_EQUAL(core::to_string(x), ss.str());
}

BOOST_AUTO_TEST_CASE(test_core_to_string_flowable)
{
  EngineTest engine;

  BOOST_CHECK_EQUAL(core::to_string(1), "1");
}

BOOST_AUTO_TEST_CASE(test_core_to_string_convertible_to_flowable)
{
  EngineTest engine;

  BOOST_CHECK_EQUAL(core::to_string("str"), "str");
}

BOOST_AUTO_TEST_CASE(test_Timestamp)
{
  EngineTest engine;

  auto x = Var(true);

  const auto m = Main(If(
    x,
    [](dtime t0) { return Timestamp(t0); },
    [](dtime t0) { return Timestamp(t0); }));

  BOOST_CHECK_EQUAL(*m, 0);

  x = false; // t=2

  BOOST_CHECK_EQUAL(*m, 2);

  x = true; // t=4

  BOOST_CHECK_EQUAL(*m, 4);
}

BOOST_AUTO_TEST_CASE(test_Timestamp_main)
{
  EngineTest engine;

  const auto m = Main([](dtime t0) { return Timestamp(t0); });

  BOOST_CHECK_EQUAL(*m, 0);
}

BOOST_AUTO_TEST_CASE(test_Since)
{
  EngineTest engine;

  auto restart = Signal();
  auto use_since = Var(true);

  // t = 0
  const auto m = Main([restart = restart.as_ref(),
                       use_since = use_since.as_ref()](dtime t0) {
    return If(
      use_since,
      Since(restart, [](dtime t0) { return Cast<std::size_t>(Timestamp(t0)); }),
      std::size_t(101010),
      t0);
  });

  BOOST_CHECK_EQUAL(*m, 0);

  // t = 2
  restart();

  BOOST_CHECK_EQUAL(*m, 2);

  // t = 4
  restart();

  BOOST_CHECK_EQUAL(*m, 4);

  // t = 6
  restart();

  BOOST_CHECK_EQUAL(*m, 6);

  // t = 8
  use_since = false;

  BOOST_CHECK_EQUAL(*m, 101010);

  // t = 9
  use_since = true;

  BOOST_CHECK_EQUAL(*m, 9);
}

BOOST_AUTO_TEST_CASE(test_Timeout)
{
  EngineTest engine;

  io_fixture io;

  io.capture_output();

  const auto y =
    Main([](dtime t0) { return introspect::Log(Timeout(1000, t0), "x"); });

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, false);

  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] x = false;[t=1] x = true;[t=2] x = false;");
}

BOOST_AUTO_TEST_CASE(test_Timeout_init_func)
{
  EngineTest engine;

  // Just test that the overloaded version exists
  const auto y = Main(Timeout(1));

  BOOST_CHECK_EQUAL(*y, false);
}

BOOST_AUTO_TEST_CASE(test_unit)
{
  EngineTest engine;

  io_fixture io;

  io.capture_output();

  auto x = Var(unit{});

  const auto y = Main(introspect::Log(x, "x"));

  io.reset_output();

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, unit{});

  BOOST_CHECK_EQUAL(io.log_string(), "[t=0] x = ();");

  io.capture_output();

  x = unit{};

  io.reset_output();

  BOOST_CHECK_EQUAL(*y, unit{});

  BOOST_CHECK_EQUAL(io.log_string(), "[t=0] x = ();");
}

BOOST_AUTO_TEST_SUITE_END()
}
