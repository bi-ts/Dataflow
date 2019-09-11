
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

#include <dataflow/introspect.h>
#include <dataflow/maybe.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_maybe)

BOOST_AUTO_TEST_CASE(test_maybe_flowable)
{
  BOOST_CHECK(core::is_flowable<maybe<ref<int>>>::value);
}

BOOST_AUTO_TEST_CASE(test_maybe_default_ctr)
{
  Engine engine;

  const auto x = Const<int>();

  const maybe<ref<int>> a;

  BOOST_CHECK_EQUAL(a.engaged(), false);
  BOOST_CHECK_EQUAL(a.value_or(x).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_ctr)
{
  Engine engine;

  const auto x = Const<int>();
  const auto y = Const<int>();

  const maybe<ref<int>> a{x};

  BOOST_CHECK_EQUAL(a.engaged(), true);
  BOOST_CHECK_EQUAL(a.value_or(y).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_copy_ctr)
{
  Engine engine;

  const auto x = Const<int>();
  const auto y = Const<int>();
  const auto z = Const<int>();

  const maybe<ref<int>> a{};
  const maybe<ref<int>> b{x};

  const maybe<ref<int>> ca{a};

  BOOST_CHECK_EQUAL(ca.engaged(), false);
  BOOST_CHECK_EQUAL(ca.value_or(z).id(), z.id());

  const maybe<ref<int>> cb{b};

  BOOST_CHECK_EQUAL(cb.engaged(), true);
  BOOST_CHECK_EQUAL(cb.value_or(z).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_copy_assignment)
{
  Engine engine;

  const auto x = Const<int>();
  const auto y = Const<int>();

  const maybe<ref<int>> n{};
  const maybe<ref<int>> e{x};

  maybe<ref<int>> nn{};

  BOOST_CHECK_EQUAL(nn.engaged(), false);
  BOOST_CHECK_EQUAL(nn.value_or(y).id(), y.id());

  nn = n;

  BOOST_CHECK_EQUAL(nn.engaged(), false);
  BOOST_CHECK_EQUAL(nn.value_or(y).id(), y.id());

  maybe<ref<int>> ne{};

  BOOST_CHECK_EQUAL(ne.engaged(), false);
  BOOST_CHECK_EQUAL(ne.value_or(y).id(), y.id());

  ne = e;

  BOOST_CHECK_EQUAL(ne.engaged(), true);
  BOOST_CHECK_EQUAL(ne.value_or(y).id(), x.id());

  maybe<ref<int>> en{x};

  BOOST_CHECK_EQUAL(en.engaged(), true);
  BOOST_CHECK_EQUAL(en.value_or(y).id(), x.id());

  en = n;

  BOOST_CHECK_EQUAL(en.engaged(), false);
  BOOST_CHECK_EQUAL(en.value_or(y).id(), y.id());

  maybe<ref<int>> ee{x};

  BOOST_CHECK_EQUAL(ee.engaged(), true);
  BOOST_CHECK_EQUAL(ee.value_or(y).id(), x.id());

  ee = e;

  BOOST_CHECK_EQUAL(ee.engaged(), true);
  BOOST_CHECK_EQUAL(ee.value_or(y).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_Nothing_to_string)
{
  Engine engine;

  const auto x = Nothing<int>();
  const auto f = *x;

  BOOST_CHECK_EQUAL(introspect::value(x), "nothing");
}

BOOST_AUTO_TEST_CASE(test_maybe_Just_to_string)
{
  Engine engine;

  const auto x = Var(11);
  const auto y = Just(x);
  const auto f = *y;

  std::stringstream ss;
  ss << std::hex << x.id();

  BOOST_CHECK_EQUAL(introspect::value(y), "just(" + ss.str() + ")");
}

BOOST_AUTO_TEST_CASE(test_maybe_Just_FromMaybe)
{
  Engine engine;

  const auto x = Var(11);
  const auto y = Just(x);
  const auto z = FromMaybe(y);
  const auto f = *z;

  BOOST_CHECK_EQUAL(introspect::label(z), "from-maybe");
  BOOST_CHECK_EQUAL(f(), 11);

  x = 22;

  BOOST_CHECK_EQUAL(f(), 22);
}

BOOST_AUTO_TEST_CASE(test_maybe_Nothing_FromMaybe_1_argument)
{
  Engine engine;

  const auto x = Nothing<ref<int>>();
  const auto y = FromMaybe(x);
  const auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "from-maybe");
  BOOST_CHECK_EQUAL(f(), 0);
}

BOOST_AUTO_TEST_CASE(test_maybe_Nothing_FromMaybe_2_arguments)
{
  Engine engine;

  const auto x = Nothing<ref<int>>();
  const auto y = Var(23);
  const auto z = FromMaybe(x, y);
  const auto f = *z;

  BOOST_CHECK_EQUAL(introspect::label(z), "from-maybe");
  BOOST_CHECK_EQUAL(f(), 23);

  y = 34;

  BOOST_CHECK_EQUAL(f(), 34);
}

BOOST_AUTO_TEST_CASE(test_maybe_Nothing_FromMaybe_2nd_argument_literal)
{
  Engine engine;

  const auto x = Nothing<ref<std::string>>();
  const auto y = FromMaybe(x, "empty");
  const auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "from-maybe");
  BOOST_CHECK_EQUAL(f(), "empty");
}

BOOST_AUTO_TEST_CASE(test_maybeE_flowable)
{
  BOOST_CHECK(core::is_flowable<maybe<int>>::value);
}

BOOST_AUTO_TEST_CASE(test_maybeE_default_ctr)
{
  const maybe<int> a;

  BOOST_CHECK_EQUAL(a.engaged(), false);
  BOOST_CHECK_EQUAL(a.value_or(34), 34);
}

BOOST_AUTO_TEST_CASE(test_maybeE_ctr)
{
  Engine engine;

  const maybe<int> a{56};

  BOOST_CHECK_EQUAL(a.engaged(), true);
  BOOST_CHECK_EQUAL(a.value_or(78), 56);
}

BOOST_AUTO_TEST_CASE(test_maybeE_copy_ctr)
{
  const maybe<int> a{};
  const maybe<int> b{11};

  const maybe<int> ca{a};

  BOOST_CHECK_EQUAL(ca.engaged(), false);
  BOOST_CHECK_EQUAL(ca.value_or(22), 22);

  const maybe<int> cb{b};

  BOOST_CHECK_EQUAL(cb.engaged(), true);
  BOOST_CHECK_EQUAL(cb.value_or(22), 11);
}

BOOST_AUTO_TEST_CASE(test_maybeE_copy_assignment)
{
  const maybe<int> n{};
  const maybe<int> e{11};

  maybe<int> nn{};

  BOOST_CHECK_EQUAL(nn.engaged(), false);
  BOOST_CHECK_EQUAL(nn.value_or(22), 22);

  nn = n;

  BOOST_CHECK_EQUAL(nn.engaged(), false);
  BOOST_CHECK_EQUAL(nn.value_or(22), 22);

  maybe<int> ne{};

  BOOST_CHECK_EQUAL(ne.engaged(), false);
  BOOST_CHECK_EQUAL(ne.value_or(22), 22);

  ne = e;

  BOOST_CHECK_EQUAL(ne.engaged(), true);
  BOOST_CHECK_EQUAL(ne.value_or(22), 11);

  maybe<int> en{33};

  BOOST_CHECK_EQUAL(en.engaged(), true);
  BOOST_CHECK_EQUAL(en.value_or(22), 33);

  en = n;

  BOOST_CHECK_EQUAL(en.engaged(), false);
  BOOST_CHECK_EQUAL(en.value_or(22), 22);

  maybe<int> ee{44};

  BOOST_CHECK_EQUAL(ee.engaged(), true);
  BOOST_CHECK_EQUAL(ee.value_or(22), 44);

  ee = e;

  BOOST_CHECK_EQUAL(ee.engaged(), true);
  BOOST_CHECK_EQUAL(ee.value_or(22), 11);
}

BOOST_AUTO_TEST_CASE(test_maybeE_Just_FromMaybe)
{
  Engine engine;

  const auto x = Var(11);
  const auto y = JustE(x);
  const auto z = FromMaybe(y);
  const auto f = *z;

  BOOST_CHECK_EQUAL(introspect::label(z), "from-maybe");
  BOOST_CHECK_EQUAL(f(), 11);

  x = 22;

  BOOST_CHECK_EQUAL(f(), 22);
}

BOOST_AUTO_TEST_CASE(test_maybeE_Nothing_FromMaybe_1_argument)
{
  Engine engine;

  const auto x = Nothing<int>();
  const auto y = FromMaybe(x);
  const auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "from-maybe");
  BOOST_CHECK_EQUAL(f(), 0);
}

BOOST_AUTO_TEST_CASE(test_maybeE_Nothing_FromMaybe_2_arguments)
{
  Engine engine;

  const auto x = Nothing<int>();
  const auto y = Var(23);
  const auto z = FromMaybe(x, y);
  const auto f = *z;

  BOOST_CHECK_EQUAL(introspect::label(z), "from-maybe");
  BOOST_CHECK_EQUAL(f(), 23);

  y = 34;

  BOOST_CHECK_EQUAL(f(), 34);
}

BOOST_AUTO_TEST_CASE(test_maybeE_Nothing_FromMaybe_2nd_argument_literal)
{
  Engine engine;

  const auto x = Nothing<std::string>();
  const auto y = FromMaybe(x, "empty");
  const auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "from-maybe");
  BOOST_CHECK_EQUAL(f(), "empty");
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
