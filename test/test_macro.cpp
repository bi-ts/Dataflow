
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#include <dataflow/macro.h>

#include <dataflow/introspect.h>
#include <dataflow/string.h>

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{

DATAFLOW_DATA(mixed,
              // Some documentation for the first field
              (dataflow::ref<int>, First),
              // Some documentation for the second field
              (std::string, Second),
              // Some documentation for the third field
              (dataflow::ref<char>, Third));

using namespace dataflow;

BOOST_AUTO_TEST_SUITE(test_macro)

BOOST_AUTO_TEST_CASE(test_DATAFLOW_DATA_mixed_Var)
{
  Engine engine;

  auto a = Var(0);
  auto c = Var('c');
  auto x = Var<mixed>(a, "str", c);
  auto y = Main(x);

  BOOST_CHECK_EQUAL(introspect::value(y), "mixed");
}

BOOST_AUTO_TEST_CASE(test_DATAFLOW_DATA_mixed_selectors)
{
  Engine engine;

  auto a = Var(0);
  auto c = Var('c');
  auto x = Var<mixed>(a, "str", c);

  auto first = First(x);

  BOOST_CHECK_EQUAL(introspect::label(first), "mixed-First");

  auto second = Second(x);

  BOOST_CHECK_EQUAL(introspect::label(second), "mixed-Second");

  auto third = Third(x);

  BOOST_CHECK_EQUAL(introspect::label(third), "mixed-Third");

  auto y = Main(first);

  BOOST_CHECK_EQUAL(*y, 0);

  a = 1;

  BOOST_CHECK_EQUAL(*y, 1);
}

BOOST_AUTO_TEST_CASE(test_DATAFLOW_DATA_mixed_equality_comparison)
{
  Engine engine;

  auto a1 = Var(1);
  auto a2 = Var(1);
  auto c = Var('c');
  const auto m1 = mixed(a1, "b", c);
  const auto m2 = mixed(a2, "b", c);

  BOOST_CHECK_EQUAL(m1 == m2, false);
  BOOST_CHECK_EQUAL(m1 == mixed(a1, "b", c), true);
  BOOST_CHECK_EQUAL(m2 == mixed(a2, "b", c), true);
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
