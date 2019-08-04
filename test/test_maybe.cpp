
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
  BOOST_CHECK(core::is_flowable<maybe<int>>::value);
}

BOOST_AUTO_TEST_CASE(test_maybe_default_ctr)
{
  Engine engine;

  const auto x = Const<int>();

  const maybe<int> a;

  BOOST_CHECK_EQUAL(a.engaged(), false);
  BOOST_CHECK_EQUAL(a.value_or(x).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_ctr)
{
  Engine engine;

  const auto x = Const<int>();
  const auto y = Const<int>();

  const maybe<int> a{x};

  BOOST_CHECK_EQUAL(a.engaged(), true);
  BOOST_CHECK_EQUAL(a.value_or(y).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_copy_ctr)
{
  Engine engine;

  const auto x = Const<int>();
  const auto y = Const<int>();

  const maybe<int> a{};
  const maybe<int> b{x};

  const maybe<int> ca{a};

  BOOST_CHECK_EQUAL(ca.engaged(), false);
  BOOST_CHECK_EQUAL(ca.value_or(x).id(), x.id());

  const maybe<int> cb{b};

  BOOST_CHECK_EQUAL(cb.engaged(), true);
  BOOST_CHECK_EQUAL(cb.value_or(y).id(), x.id());
}

BOOST_AUTO_TEST_CASE(test_maybe_copy_assignment)
{
  Engine engine;

  const auto x = Const<int>();
  const auto y = Const<int>();

  const maybe<int> n{};
  const maybe<int> e{x};

  maybe<int> nn{};

  BOOST_CHECK_EQUAL(nn.engaged(), false);
  BOOST_CHECK_EQUAL(nn.value_or(y).id(), y.id());

  nn = n;

  BOOST_CHECK_EQUAL(nn.engaged(), false);
  BOOST_CHECK_EQUAL(nn.value_or(y).id(), y.id());

  maybe<int> ne{};

  BOOST_CHECK_EQUAL(ne.engaged(), false);
  BOOST_CHECK_EQUAL(ne.value_or(y).id(), y.id());

  ne = e;

  BOOST_CHECK_EQUAL(ne.engaged(), true);
  BOOST_CHECK_EQUAL(ne.value_or(y).id(), x.id());

  maybe<int> en{x};

  BOOST_CHECK_EQUAL(en.engaged(), true);
  BOOST_CHECK_EQUAL(en.value_or(y).id(), x.id());

  en = n;

  BOOST_CHECK_EQUAL(en.engaged(), false);
  BOOST_CHECK_EQUAL(en.value_or(y).id(), y.id());

  maybe<int> ee{x};

  BOOST_CHECK_EQUAL(ee.engaged(), true);
  BOOST_CHECK_EQUAL(ee.value_or(y).id(), x.id());

  ee = e;

  BOOST_CHECK_EQUAL(ee.engaged(), true);
  BOOST_CHECK_EQUAL(ee.value_or(y).id(), x.id());
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
