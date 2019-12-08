
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

BOOST_AUTO_TEST_CASE(test_listC_Var_Length)
{
  Engine engine;

  auto a = Var<list<std::string>>();

  auto b = Main([=](const Time& t0) { return Length(a); });

  BOOST_CHECK_EQUAL(b(), 0);

  a.insert(0, "0");

  BOOST_CHECK_EQUAL(b(), 1);

  a.insert(1, "2");

  BOOST_CHECK_EQUAL(b(), 2);

  a.insert(2, "3");

  BOOST_CHECK_EQUAL(b(), 3);

  a.erase(0);

  BOOST_CHECK_EQUAL(b(), 2);

  a.erase(0);

  BOOST_CHECK_EQUAL(b(), 1);

  a.erase(0);

  BOOST_CHECK_EQUAL(b(), 0);
}

BOOST_AUTO_TEST_CASE(test_listC_Var_insert_inactive)
{
  Engine engine;

  auto xs = Var<list<int>>();

  xs.insert(0, 2);
  xs.insert(0, 1);

  auto f = *xs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_erase_inactive)
{
  Engine engine;

  auto xs = Var<list<int>>(1, 2, 3, 4);

  xs.erase(2);
  xs.erase(1);

  auto f = *xs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 4)");
}

BOOST_AUTO_TEST_CASE(test_listA_Get)
{
  Engine engine;

  auto lst = Var<listA<std::string>>();
  auto idx = Var<integer>(0);

  auto vd = Var("default");
  auto v1 = Var("first");
  auto v2 = Var("second");
  auto v3 = Var("third");

  auto x = Main([=](const Time& t0) { return FromMaybe(Get(lst, idx), vd); });

  BOOST_CHECK_EQUAL(x(), "default");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), true);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);

  lst = make_listA(v1);

  BOOST_CHECK_EQUAL(x(), "first");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), true);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);

  idx = 1;

  BOOST_CHECK_EQUAL(x(), "default");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), true);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);

  lst = make_listA(v1, v2);

  BOOST_CHECK_EQUAL(x(), "second");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), true);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);

  v2 = "2nd";

  BOOST_CHECK_EQUAL(x(), "2nd");

  lst = make_listA(v1, "second", v3, "fourth");

  BOOST_CHECK_EQUAL(x(), "second");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);

  idx = 2;

  BOOST_CHECK_EQUAL(x(), "third");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), true);

  idx = 3;

  BOOST_CHECK_EQUAL(x(), "fourth");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);

  idx = 4;

  BOOST_CHECK_EQUAL(x(), "default");

  BOOST_CHECK_EQUAL(introspect::active_node(vd), true);
  BOOST_CHECK_EQUAL(introspect::active_node(v1), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v2), false);
  BOOST_CHECK_EQUAL(introspect::active_node(v3), false);
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

BOOST_AUTO_TEST_CASE(test_listC_Var_Get)
{
  Engine engine;

  auto lst = Var<listC<std::string>>();
  auto idx = Var<integer>(0);

  auto x =
    Main([=](const Time& t0) { return FromMaybe(Get(lst, idx), "default"); });

  BOOST_CHECK_EQUAL(x(), "default");

  lst.insert(0, "first");

  BOOST_CHECK_EQUAL(x(), "first");

  idx = 1;

  BOOST_CHECK_EQUAL(x(), "default");

  lst.insert(1, "second");

  BOOST_CHECK_EQUAL(x(), "second");

  lst.insert(1, "third");

  BOOST_CHECK_EQUAL(x(), "third");

  lst.insert(3, "fourth");

  BOOST_CHECK_EQUAL(x(), "third");

  lst.erase(2);

  BOOST_CHECK_EQUAL(x(), "third");

  lst.insert(1, "second");

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

BOOST_AUTO_TEST_CASE(test_ListC_Insert)
{
  Engine engine;

  auto x1 = Var(1);
  auto x2 = Var(2);
  auto x3 = Var(3);

  auto idx = Var(1);

  auto xs = ListC(x1, x2, x3);

  auto ys = Insert(xs, idx, 11);

  auto f = *ys;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 11 2 3)");

  x2 = 0;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 11 0 3)");

  idx = 3;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 0 3 11)");

  x3 = 333;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 0 333 11)");

  x1 = 111;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(111 0 333 11)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_Insert_out_of_range)
{
  Engine engine;

  const auto xs = Var<list<int>>(0, 1, 2);
  auto idx = Var(3);

  const auto ys = Insert(xs, idx, 100);

  const auto f = *ys;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 100)");

  idx = 4;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 100)");

  idx = 5;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 100)");

  idx = 2;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 100 2)");

  idx = 1;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 100 1 2)");

  idx = 0;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(100 0 1 2)");

  idx = -1;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(100 0 1 2)");

  idx = -2;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(100 0 1 2)");
}

BOOST_AUTO_TEST_CASE(test_ListC_Erase)
{
  Engine engine;

  auto x1 = Var(1);
  auto x2 = Var(2);
  auto x3 = Var(3);

  auto idx = Var(1);

  const auto xs = ListC(x1, x2, x3);

  const auto ys = Erase(xs, idx);

  const auto f = *ys;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 3)");

  x1 = 111;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(111 3)");

  x3 = 333;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(111 333)");

  idx = 2;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(111 2)");

  x2 = 222;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(111 222)");
}

BOOST_AUTO_TEST_CASE(test_listC_Erase_out_of_range)
{
  Engine engine;

  const auto xs = Var<listC<int>>(0, 1, 2, 3);
  auto idx = Var(3);

  auto ys = Erase(xs, idx);

  auto f = *ys;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2)");

  idx = 4;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 3)");

  idx = 5;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 3)");

  idx = 2;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 3)");

  idx = 1;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 2 3)");

  idx = 0;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3)");

  idx = -1;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 3)");

  idx = -2;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 1 2 3)");
}

BOOST_AUTO_TEST_CASE(test_listC_Erase_in_empty)
{
  Engine engine;

  const auto xs = Var<listC<int>>();
  auto idx = Var(3);

  auto ys = Erase(xs, idx);

  auto f = *ys;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list()");

  idx = 4;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list()");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_Map)
{
  Engine engine;

  auto xs = Var<list<int>>(0, 1, 2, 3, 4, 5);
  auto y = Var<int>(100);

  int calls_count = 0;

  const auto zs = Map(
    xs,
    [&](int x, int y) {
      ++calls_count;
      return x + y;
    },
    y);

  BOOST_CHECK_EQUAL(calls_count, 0);

  auto f = *zs;

  BOOST_CHECK_EQUAL(calls_count, 6);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(100 101 102 103 104 105)");

  y = 33;

  BOOST_CHECK_EQUAL(calls_count, 12);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(33 34 35 36 37 38)");

  xs.insert(3, 22);

  BOOST_CHECK_EQUAL(calls_count, 13);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(33 34 35 55 36 37 38)");
}

BOOST_AUTO_TEST_CASE(test_listC_ListC_Map)
{
  Engine engine;

  auto x0 = Var(1);
  auto x1 = Var(2);
  auto x2 = Var(3);

  auto y = Var(100);

  auto xs = ListC(x0, x1, x2);

  int calls_count = 0;

  const auto zs = Map(
    xs,
    [&](int x, int y) {
      ++calls_count;
      return x + y;
    },
    y);

  BOOST_CHECK_EQUAL(calls_count, 0);

  auto f = *zs;

  BOOST_CHECK_EQUAL(calls_count, 3);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(101 102 103)");

  y = 33;

  BOOST_CHECK_EQUAL(calls_count, 6);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(34 35 36)");

  x1 = 11;

  BOOST_CHECK_EQUAL(calls_count, 7);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(34 44 36)");
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
