
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

#include <dataflow/tuple.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_tuple)

BOOST_AUTO_TEST_CASE(test_tupleE)
{
  Engine engine;

  tupleE<std::string, int, double> x = make_tupleE("text", 1, 3.14);

  // Streaming operator
  std::stringstream ss;

  ss << x;

  BOOST_CHECK_EQUAL(ss.str(), "tupleE(text; 1; 3.14)");

  // Equality comparison
  BOOST_CHECK(x == make_tupleE("text", 1, 3.14));
  BOOST_CHECK(x != (tupleE<std::string, int, double>()));
}

BOOST_AUTO_TEST_CASE(test_TupleE)
{
  Engine engine;

  auto a = Var(2.017);
  auto b = Var("text");

  auto c = TupleE("Friday", 13, a, b, "other text");

  auto d = Get<3>(c);

  BOOST_CHECK_EQUAL(introspect::label(d), "get<3>");

  auto e = *d;

  BOOST_CHECK_EQUAL(e(), "text");

  b = "abc";

  BOOST_CHECK_EQUAL(e(), "abc");
}

BOOST_AUTO_TEST_CASE(test_TupleE_Getters)
{
  Engine engine;

  auto a = TupleE("str", 20, 'c', "text", 34, 0, 1, 2);

  auto b = *TupleE(Fifth(a), Fourth(a), Third(a), Second(a), First(a));

  BOOST_CHECK_EQUAL(b(), make_tupleE(34, "text", 'c', 20, "str"));
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
