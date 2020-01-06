
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

#include <dataflow/pair.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_pair)

BOOST_AUTO_TEST_CASE(test_pair)
{
  Engine engine;

  auto x = Var(13);
  auto y = Var(31);
  auto z = Pair(x, y);
  auto use_z = Var(true);

  auto a = First(z);

  BOOST_CHECK_EQUAL(introspect::label(a), "first");

  auto b = Second(z);

  BOOST_CHECK_EQUAL(introspect::label(b), "second");

  auto c = If(use_z, z, Pair(1, 2));

  auto d = Main(Second(c));

  BOOST_CHECK_EQUAL(*d, 31);

  y = 43;

  BOOST_CHECK_EQUAL(*d, 43);

  use_z = false;

  BOOST_CHECK_EQUAL(*d, 2);
}

BOOST_AUTO_TEST_CASE(test_pair_First)
{
  Engine engine;

  const auto a = Pair(std::string("1st"), 2);

  const auto b = Main(First(a));

  BOOST_CHECK_EQUAL(*b, std::string("1st"));
}

BOOST_AUTO_TEST_CASE(test_pair_Second)
{
  Engine engine;

  const auto a = Pair(1, std::string("2nd"));

  const auto b = Main(Second(a));

  BOOST_CHECK_EQUAL(*b, std::string("2nd"));
}

BOOST_AUTO_TEST_CASE(test_pair_Pair_overloads)
{
  Engine engine;

  auto x = Var(1);
  auto y = Var("two");

  ref<pair<int, std::string>> a = Pair(x, y);
  ref<pair<int, std::string>> b = Pair(x, std::string("2"));
  ref<pair<int, std::string>> c = Pair(11, y);
  ref<pair<int, std::string>> d = Pair(111, std::string("222"));

  ref<pair<std::string, int>> e = Pair("1", 2);
  ref<pair<std::string, int>> f = Pair("1", x);
  ref<pair<int, std::string>> g = Pair(1, "2");
  ref<pair<int, std::string>> h = Pair(x, "2");
  ref<pair<std::string, std::string>> i = Pair("2", "2");
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
