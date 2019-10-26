
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

BOOST_AUTO_TEST_CASE(test_tuple_values)
{
  Engine engine;

  const tuple<std::string, int, double> x = make_tuple("text", 1, 3.14);
  const auto y = x;

  // Streaming operator
  std::stringstream ss;

  ss << x;

  BOOST_CHECK_EQUAL(ss.str(), "tuple(text; 1; 3.14)");

  // Equality comparison
  BOOST_CHECK(x == y);
  BOOST_CHECK(x == make_tuple("text", 1, 3.14));
}

BOOST_AUTO_TEST_CASE(test_tuple_mixed)
{
  Engine engine;

  const auto x = Const(1);

  tuple<std::string, ref<int>, double> y = make_tuple("text", x, 3.14);

  BOOST_CHECK_EQUAL(get<0>(y), "text");
  BOOST_CHECK_EQUAL(get<1>(y).id(), x.id());
  BOOST_CHECK_EQUAL(get<2>(y), 3.14);

  // Streaming operator
  std::stringstream ss;

  ss << y;

  BOOST_CHECK_EQUAL(ss.str().substr(0, 11), "tuple(text;");

  // Equality comparison
  BOOST_CHECK(y == make_tuple("text", x, 3.14));

  // Inequality comparison
  BOOST_CHECK(y != make_tuple("txt", x, 3.14));

  BOOST_CHECK(y != make_tuple("text", Const(1), 3.14));
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

  BOOST_CHECK_EQUAL((get<0>(b())), 34);
  BOOST_CHECK_EQUAL((get<1>(b())), "text");
  BOOST_CHECK_EQUAL((get<2>(b())), 'c');
  BOOST_CHECK_EQUAL((get<3>(b())), 20);
  BOOST_CHECK_EQUAL((get<4>(b())), "str");
}

BOOST_AUTO_TEST_CASE(test_Tuple_Getters)
{
  Engine engine;

  const auto a = Const(
    // TODO: why this is confused with `std::make_tuple`?
    dataflow::make_tuple(Const("str"), Const(20), Const('c'), Const("text")));

  const auto b = *Third(a);

  BOOST_CHECK_EQUAL(b(), 'c');
}

BOOST_AUTO_TEST_CASE(test_Tuple_equality_comparison)
{
  Engine engine;

  auto x = Var(10);
  auto y = Var(20.19);

  const auto a = Const(dataflow::make_tuple("13", Const(10), Const(20.19)));
  const auto b = Const(dataflow::make_tuple("13", x, y));

  const auto c = *(a == b);

  BOOST_CHECK_EQUAL(c(), true);

  x = 0;

  BOOST_CHECK_EQUAL(c(), false);
}

BOOST_AUTO_TEST_CASE(test_Tuple_equality_comparison_minimal_tuple)
{
  Engine engine;

  auto x = Var("13");
  const auto y = Const(dataflow::make_tuple(Const("13")));
  const auto z = Const(dataflow::make_tuple(x));

  const auto f = *(z == y);

  BOOST_CHECK_EQUAL(f(), true);

  x = "14";

  BOOST_CHECK_EQUAL(f(), false);
}

BOOST_AUTO_TEST_CASE(test_Tuple_inequality_comparison)
{
  Engine engine;

  auto x = Var(10);
  const auto y = Var(20.19);

  const auto a = Const(dataflow::make_tuple("13", Const(10), Const(20.19)));
  const auto b = Const(dataflow::make_tuple("13", x, y));

  const auto c = *(a != b);

  BOOST_CHECK_EQUAL(c(), false);

  x = 0;

  BOOST_CHECK_EQUAL(c(), true);
}

BOOST_AUTO_TEST_CASE(test_Tuple_inequality_comparison_minimal_tuple)
{
  Engine engine;

  auto x = Var("13");
  const auto y = Const(dataflow::make_tuple(Const("13")));
  const auto z = Const(dataflow::make_tuple(x));

  const auto f = *(z != y);

  BOOST_CHECK_EQUAL(f(), false);

  x = "14";

  BOOST_CHECK_EQUAL(f(), true);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
