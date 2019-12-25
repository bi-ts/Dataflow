
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

  auto a = Var<listC<std::string>>();

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

  auto xs = Var<listC<int>>();

  xs.insert(0, 2);
  xs.insert(0, 1);

  auto f = *xs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_erase_inactive)
{
  Engine engine;

  auto xs = Var<listC<int>>(1, 2, 3, 4);

  xs.erase(2);
  xs.erase(1);

  auto f = *xs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 4)");
}

BOOST_AUTO_TEST_CASE(test_listA_Var_assign_Get)
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

BOOST_AUTO_TEST_CASE(test_listC_Var_assign_Get)
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

BOOST_AUTO_TEST_CASE(test_listC_Var_ins_erase_Get)
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

BOOST_AUTO_TEST_CASE(test_listC_Var_ins_erase_Get_via_subscript_operator)
{
  Engine engine;

  auto lst = Var<listC<std::string>>();
  auto idx = Var<integer>(0);

  auto x = Main([=](const Time& t0) { return FromMaybe(lst[idx]); });

  lst.insert(0, "first");
  lst.insert(1, "fourth");

  BOOST_CHECK_EQUAL(x(), "first");

  idx = 1;

  BOOST_CHECK_EQUAL(x(), "fourth");

  lst.insert(1, "third");

  BOOST_CHECK_EQUAL(x(), "third");

  lst.insert(1, "second");

  BOOST_CHECK_EQUAL(x(), "second");

  idx = 2;

  BOOST_CHECK_EQUAL(x(), "third");

  lst.erase(1);

  BOOST_CHECK_EQUAL(x(), "fourth");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_assign_Get_via_subscript_operator)
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

  const auto xs = Var<listC<int>>(0, 1, 2);
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

BOOST_AUTO_TEST_CASE(test_listC_Var_ins_Insert_Erase_patch_normalization)
{
  Engine engine;

  auto xs = Var<listC<int>>(1, 3, 4);

  auto as = Insert(xs, 0, 2);
  auto bs = Erase(as, 0);
  auto cs = Insert(bs, 0, 1);
  auto ds = Erase(cs, 0);
  auto es = Insert(ds, 0, 0);

  int calls_count = 0;

  const auto f = *Map(es, [&](int x) {
    ++calls_count;
    return x * 10;
  });

  BOOST_CHECK_EQUAL(calls_count, 4);
  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 10 30 40)");

  xs.insert(1, 2);

  BOOST_CHECK_EQUAL(calls_count, 6); // TODO: must be 5
  BOOST_CHECK_EQUAL(core::to_string(f()), "list(0 10 20 30 40)");
}

BOOST_AUTO_TEST_CASE(test_listC_State)
{
  Engine engine;

  auto x = Var<int>();

  auto xs = Main([=](const Time& t0) {
    auto xp = Prev(0, x, t0);

    return StateMachine(
      ListC(x(t0)),
      [=](const ref<list<int>>& sp) {
        return If(x != xp, Insert(sp, Length(sp), x), sp);
      },
      t0);
  });

  BOOST_CHECK_EQUAL(core::to_string(xs()), "list(0)");

  x = 1;

  BOOST_CHECK_EQUAL(core::to_string(xs()), "list(0 1)");

  x = 5;

  BOOST_CHECK_EQUAL(core::to_string(xs()), "list(0 1 5)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_Concat_operator)
{
  Engine engine;

  auto xs = Var<listC<int>>(1, 2, 3, 4);
  auto ys = Var<listC<int>>(5, 6, 7);
  auto zs = xs + ys;

  auto f = *zs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3 4 5 6 7)");

  xs.insert(2, 22);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 3 4 5 6 7)");

  ys.insert(1, 55);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 3 4 5 55 6 7)");

  xs.erase(1);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 5 55 6 7)");

  ys.erase(0);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 55 6 7)");
}

BOOST_AUTO_TEST_CASE(test_ListC_Concat_operator)
{
  Engine engine;

  auto x1 = Var(1);
  auto x2 = Var(2);
  auto x3 = Var(3);
  auto x4 = Var(4);
  auto xs = ListC(x1, x2, x3, x4);

  auto y1 = Var(5);
  auto y2 = Var(6);
  auto y3 = Var(7);
  auto ys = ListC(y1, y2, y3);

  auto zs = xs + ys;

  auto f = *zs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3 4 5 6 7)");

  x2 = 22;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 5 6 7)");

  y1 = 55;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 55 6 7)");

  x4 = 44;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 44 55 6 7)");

  y2 = 66;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 44 55 66 7)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_Concat_arguments)
{
  Engine engine;

  auto a = Var<listC<int>>(1, 2);
  auto b = Concat(a, a);
  auto c = Concat(make_listC(3, 4), make_listC(3, 4));
  auto d = Concat(b, make_listC(3, 4));
  auto e = Concat(make_listC(-1, 0), d);

  auto f = *Concat(c, e);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(3 4 3 4 -1 0 1 2 1 2 3 4)");

  a.insert(1, 8);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(3 4 3 4 -1 0 1 8 2 1 8 2 3 4)");
}

BOOST_AUTO_TEST_CASE(test_listC_Concat_operator_arguments)
{
  Engine engine;

  auto a = Var<listC<int>>(1, 2);
  auto b = a + a;
  auto c = make_listC(3, 4) + make_listC(3, 4);
  auto d = b + make_listC(3, 4);
  auto e = make_listC(-1, 0) + d;

  auto f = *(c + e);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(3 4 3 4 -1 0 1 2 1 2 3 4)");

  a.insert(1, 8);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(3 4 3 4 -1 0 1 8 2 1 8 2 3 4)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_ListC_concat_Insert_Erase)
{
  Engine engine;

  auto xs = Var<listC<int>>(1, 2, 3);

  auto y1 = Var(5);
  auto y2 = Var(6);
  auto y3 = Var(7);

  auto ys = ListC(y1, y2, y3);

  auto a = Var(4);

  auto zs = Insert(xs + Erase(ys, 1), Length(xs), a);

  auto f = *zs;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3 4 5 7)");

  xs.insert(2, 22);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 3 4 5 7)");

  y1 = 55;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 3 4 55 7)");

  xs.erase(1);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 55 7)");

  y2 = 66;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 55 7)");

  y3 = 77;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 3 4 55 77)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_Take)
{
  Engine engine;

  auto xs = Var<listC<int>>(1, 2, 3, 4, 5);

  auto n = Var(3);

  auto ys = Take(xs, n);

  auto f = *ys;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3)");

  n = 2;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2)");

  n = 4;

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3 4)");

  xs.insert(5, 6);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 3 4)");

  xs.insert(2, 22);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 3)");

  xs.insert(4, 44);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 3)");

  xs.insert(3, 33);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 2 22 33)");

  xs.erase(1);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 33 44)");

  xs.erase(4);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 33 44)");

  xs.erase(3);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 33 5)");

  xs.erase(3);

  BOOST_CHECK_EQUAL(core::to_string(f()), "list(1 22 33 6)");
}

BOOST_AUTO_TEST_CASE(test_listC_Var_Map)
{
  Engine engine;

  auto xs = Var<listC<int>>(0, 1, 2, 3, 4, 5);
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

BOOST_AUTO_TEST_CASE(test_ListC_Map)
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
