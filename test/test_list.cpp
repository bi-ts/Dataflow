
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

#include <dataflow/list.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_list)

BOOST_AUTO_TEST_CASE(test_listA_ctor)
{
  Engine engine;

  const auto a = listA<std::string>{
    "first", std::string("second"), Var("third"), Const("fourth")};
}

BOOST_AUTO_TEST_CASE(test_listC_ctor)
{
  Engine engine;

  const auto a = listC<std::string>{"first", std::string("second")};
}

BOOST_AUTO_TEST_CASE(test_listA_make_listA)
{
  Engine engine;

  const listA<std::string> b =
    make_listA("first", std::string("second"), Var("third"), Const("fourth"));
  const listA<std::string> c =
    make_listA(Const("first"), std::string("second"), Var("third"), "fourth");
  const listA<std::string> d =
    make_listA(Var("first"), std::string("second"), "third", Const("fourth"));
  const listA<std::string> e =
    make_listA(std::string("first"), "second", Var("third"), Const("fourth"));
}

BOOST_AUTO_TEST_CASE(test_listC_make_listC)
{
  Engine engine;

  const listC<std::string> b =
    make_listC("first", std::string("second"), "third");
  const listC<std::string> c =
    make_listC(std::string("first"), std::string("second"), "third");
  const listC<std::string> d =
    make_listC("first", std::string("second"), std::string("third"));
  const listC<std::string> e =
    make_listC(std::string("first"), "second", std::string("third"));
}

BOOST_AUTO_TEST_CASE(test_listA_equality_inequality_test)
{
  Engine engine;

  BOOST_CHECK_EQUAL((listA<int>() == listA<int>()), true);
  BOOST_CHECK_EQUAL((listA<int>() == listA<int>(1)), false);
  BOOST_CHECK_EQUAL((listA<int>(2) == listA<int>()), false);

  BOOST_CHECK_EQUAL((listA<int>() != listA<int>()), false);
  BOOST_CHECK_EQUAL((listA<int>() != listA<int>(1)), true);
  BOOST_CHECK_EQUAL((listA<int>(2) != listA<int>()), true);

  const auto c1 = Const(1);
  const auto c2 = Const(2);
  const auto c3 = Const(3);
  const auto c4 = Const(4);

  const auto a = listA<int>(c1, c2, c3, c4);
  const auto b = a;
  const auto c = listA<int>(1, 2, 3, 4);
  const auto d = listA<int>(c1, c2, c3, c4);

  BOOST_CHECK_EQUAL((a == b), true);
  BOOST_CHECK_EQUAL((a != b), false);
  BOOST_CHECK_EQUAL((a == c), false);
  BOOST_CHECK_EQUAL((a != c), true);
  BOOST_CHECK_EQUAL((a == d), true);
  BOOST_CHECK_EQUAL((a != d), false);
}

BOOST_AUTO_TEST_CASE(test_ListA_Length)
{
  Engine engine;

  var<std::string> a = Var("third");
  ref<std::string> b = Var("fifth");

  const auto c = ListA("first", std::string("second"), a, "fourth", b);

  const auto d = Length(c);

  BOOST_CHECK_EQUAL(introspect::label(d), "list-length");

  const auto e = *d;

  BOOST_CHECK_EQUAL(e(), 5);
}

BOOST_AUTO_TEST_CASE(test_listC_Get)
{
  Engine engine;

  auto lst = Var<listC<std::string>>();
  auto idx = Var<integer>(0);

  auto x =
    Main([=](const Time& t0) { return FromMaybe(Get(lst, idx), "default"); });

  BOOST_CHECK_EQUAL(x(), "default");

  lst = make_listC("first");

  BOOST_CHECK_EQUAL(x(), "first");

  idx = 1;

  BOOST_CHECK_EQUAL(x(), "default");

  lst = make_listC("first", "second");

  BOOST_CHECK_EQUAL(x(), "second");

  lst = make_listC("first", "second", "third", "fourth");

  BOOST_CHECK_EQUAL(x(), "second");

  idx = 2;

  BOOST_CHECK_EQUAL(x(), "third");

  idx = 3;

  BOOST_CHECK_EQUAL(x(), "fourth");

  idx = 4;

  BOOST_CHECK_EQUAL(x(), "default");
}

BOOST_AUTO_TEST_CASE(test_ListC_Get_via_subscript_operator)
{
  Engine engine;

  auto lst = Var<listC<std::string>>();
  auto idx = Var<integer>(0);

  auto x = Main([=](const Time& t0) { return FromMaybe(lst[idx], "default"); });

  BOOST_CHECK_EQUAL(x(), "default");

  lst = make_listC("first");

  BOOST_CHECK_EQUAL(x(), "first");

  idx = 1;

  BOOST_CHECK_EQUAL(x(), "default");

  lst = make_listC("first", "second");

  BOOST_CHECK_EQUAL(x(), "second");

  lst = make_listC("first", "second", "third", "fourth");

  BOOST_CHECK_EQUAL(x(), "second");

  idx = 2;

  BOOST_CHECK_EQUAL(x(), "third");

  idx = 3;

  BOOST_CHECK_EQUAL(x(), "fourth");

  idx = 4;

  BOOST_CHECK_EQUAL(x(), "default");
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
