
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

  const tuple<std::string, int, double> x = make_tupleB("text", 1, 3.14);
  const auto y = x;

  // Streaming operator
  std::stringstream ss;

  ss << x;

  BOOST_CHECK_EQUAL(ss.str(), "tuple(text; 1; 3.14)");

  // Equality comparison
  BOOST_CHECK(x == y);
  BOOST_CHECK(x == make_tupleB("text", 1, 3.14));
}

BOOST_AUTO_TEST_CASE(test_tuple_mixed)
{
  Engine engine;

  const auto x = Const(1);

  tuple<std::string, ref<int>, double> y = make_tupleB("text", x, 3.14);

  BOOST_CHECK_EQUAL(get<0>(y), "text");
  BOOST_CHECK_EQUAL(get<1>(y).id(), x.id());
  BOOST_CHECK_EQUAL(get<2>(y), 3.14);

  // Streaming operator
  std::stringstream ss;

  ss << y;

  BOOST_CHECK_EQUAL(ss.str().substr(0, 11), "tuple(text;");

  // Equality comparison
  BOOST_CHECK(y == make_tupleB("text", x, 3.14));

  // Inequality comparison
  BOOST_CHECK(y != make_tupleB("txt", x, 3.14));

  BOOST_CHECK(y != make_tupleB("text", Const(1), 3.14));
}

BOOST_AUTO_TEST_CASE(test_TupleA_Get)
{
  Engine engine;

  auto a = Var(2.019);
  auto b = Var("26");
  auto c = Var("text");

  auto d = TupleA("Saturday", 13, a, b, c);

  BOOST_CHECK_EQUAL(introspect::label(d), "const");

  auto e = Get<3>(d);

  BOOST_CHECK_EQUAL(introspect::label(e), "get<3>");

  const auto f = Get<0>(d) + ", " + e;

  auto g = Main(f);

  BOOST_CHECK_EQUAL(*g, "Saturday, 26");
  BOOST_CHECK_EQUAL(introspect::active_node(a), false);
  BOOST_CHECK_EQUAL(introspect::active_node(b), true);
  BOOST_CHECK_EQUAL(introspect::active_node(c), false);

  b = "27";

  BOOST_CHECK_EQUAL(*g, "Saturday, 27");
}

BOOST_AUTO_TEST_CASE(test_TupleC_Get)
{
  Engine engine;

  auto a = Var(2.017);
  auto b = Var("13");
  auto c = Var("text");

  auto d = TupleC("Friday", 13, a, b, c);

  BOOST_CHECK_EQUAL(introspect::label(d), "tuple");

  auto e = Get<3>(d);

  BOOST_CHECK_EQUAL(introspect::label(e), "get<3>");

  const auto f = Get<0>(d) + ", " + e;

  auto g = Main(f);

  BOOST_CHECK_EQUAL(*g, "Friday, 13");
  BOOST_CHECK_EQUAL(introspect::active_node(a), true);
  BOOST_CHECK_EQUAL(introspect::active_node(b), true);
  BOOST_CHECK_EQUAL(introspect::active_node(c), true);

  b = "15";

  BOOST_CHECK_EQUAL(*g, "Friday, 15");
}

BOOST_AUTO_TEST_CASE(test_TupleC_Getters)
{
  Engine engine;

  auto a = TupleC("str", 20, 'c', "text", 34, 0, 1, 2);

  auto b = Main(TupleC(Fifth(a), Fourth(a), Third(a), Second(a), First(a)));

  BOOST_CHECK_EQUAL((get<0>(*b)), 34);
  BOOST_CHECK_EQUAL((get<1>(*b)), "text");
  BOOST_CHECK_EQUAL((get<2>(*b)), 'c');
  BOOST_CHECK_EQUAL((get<3>(*b)), 20);
  BOOST_CHECK_EQUAL((get<4>(*b)), "str");
}

BOOST_AUTO_TEST_CASE(test_Tuple_Getters)
{
  Engine engine;

  const auto a =
    Const(make_tupleB(Const("str"), Const(20), Const('c'), Const("text")));

  const auto b = Main(Third(a));

  BOOST_CHECK_EQUAL(*b, 'c');
}

BOOST_AUTO_TEST_CASE(test_Tuple_equality_comparison)
{
  Engine engine;

  auto x = Var(10);
  auto y = Var(20.19);

  const auto a = Const(make_tupleB("13", Const(10), Const(20.19)));
  const auto b = Const(make_tupleB("13", x, y));

  const auto c = Main((a == b));

  BOOST_CHECK_EQUAL(*c, true);

  x = 0;

  BOOST_CHECK_EQUAL(*c, false);
}

BOOST_AUTO_TEST_CASE(test_Tuple_equality_comparison_minimal_tuple)
{
  Engine engine;

  auto x = Var("13");
  const auto y = Const(make_tupleB(Const("13")));
  const auto z = Const(make_tupleB(x));

  const auto f = Main((z == y));

  BOOST_CHECK_EQUAL(*f, true);

  x = "14";

  BOOST_CHECK_EQUAL(*f, false);
}

BOOST_AUTO_TEST_CASE(test_Tuple_inequality_comparison)
{
  Engine engine;

  auto x = Var(10);
  const auto y = Var(20.19);

  const auto a = Const(make_tupleB("13", Const(10), Const(20.19)));
  const auto b = Const(make_tupleB("13", x, y));

  const auto c = Main((a != b));

  BOOST_CHECK_EQUAL(*c, false);

  x = 0;

  BOOST_CHECK_EQUAL(*c, true);
}

BOOST_AUTO_TEST_CASE(test_Tuple_inequality_comparison_minimal_tuple)
{
  Engine engine;

  auto x = Var("13");
  const auto y = Const(make_tupleB(Const("13")));
  const auto z = Const(make_tupleB(x));

  const auto f = Main((z != y));

  BOOST_CHECK_EQUAL(*f, false);

  x = "14";

  BOOST_CHECK_EQUAL(*f, true);
}

BOOST_AUTO_TEST_CASE(test_tupleA_def_initialized_eq_comparison)
{
  Engine engine;

  const tuple<ref<int>> a;
  const tuple<ref<int>> b;

  BOOST_CHECK_EQUAL(a, b);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
