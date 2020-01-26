
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

#include <dataflow/introspect.h>
#include <dataflow/macro.h>
#include <dataflow/string.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

DATAFLOW_DATA(triple,
              // Some documentation for the first field
              (ref<int>, First),
              // Some documentation for the second field
              (std::string, Second),
              // Some documentation for the third field
              (ref<char>, Third));

BOOST_AUTO_TEST_SUITE(test_macro)

BOOST_AUTO_TEST_CASE(test_DECLARE_DATA_Var)
{
  Engine engine;

  auto a = Var(0);
  auto c = Var('c');
  auto x = Var<triple>(a, "str", c);
  auto y = Main(x);

  BOOST_CHECK_EQUAL(introspect::value(y), "triple");
}

BOOST_AUTO_TEST_CASE(test_DECLARE_DATA_selectors)
{
  Engine engine;

  auto a = Var(0);
  auto c = Var('c');
  auto x = Var<triple>(a, "str", c);

  auto first = First(x);

  BOOST_CHECK_EQUAL(introspect::label(first), "triple-First");

  auto second = Second(x);

  BOOST_CHECK_EQUAL(introspect::label(second), "triple-Second");

  auto third = Third(x);

  BOOST_CHECK_EQUAL(introspect::label(third), "triple-Third");

  auto y = Main(first);

  BOOST_CHECK_EQUAL(*y, 0);

  a = 1;

  BOOST_CHECK_EQUAL(*y, 1);
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
