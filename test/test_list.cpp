
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

BOOST_AUTO_TEST_CASE(test_list)
{
  BOOST_CHECK_EQUAL(core::is_trivially_patcheable<listA<int>>::value, false);
  BOOST_CHECK_EQUAL(core::is_trivially_patcheable<listC<int>>::value, false);
}

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

BOOST_AUTO_TEST_CASE(test_listC_insert_into_empty)
{
  Engine engine;

  auto xs = listC<std::string>{};

  BOOST_CHECK_EQUAL(xs.size(), 0);

  xs = xs.insert(0, "first");

  BOOST_CHECK_EQUAL(xs.size(), 1);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(first)");
}

BOOST_AUTO_TEST_CASE(test_listC_insert)
{
  Engine engine;

  auto xs = list<std::string>{"1", "2", "3"};

  BOOST_CHECK_EQUAL(xs.size(), 3);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 2 3)");

  xs = xs.insert(2, ",");

  BOOST_CHECK_EQUAL(xs.size(), 4);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 2 , 3)");

  xs = xs.insert(1, ",");

  BOOST_CHECK_EQUAL(xs.size(), 5);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 , 2 , 3)");
}

BOOST_AUTO_TEST_CASE(test_listC_erase)
{
  Engine engine;

  auto xs = make_listC("1", "2", "3", "4", "5");

  BOOST_CHECK_EQUAL(xs.size(), 5);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 2 3 4 5)");

  xs = xs.erase(1);

  BOOST_CHECK_EQUAL(xs.size(), 4);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 3 4 5)");

  xs = xs.erase(3);

  BOOST_CHECK_EQUAL(xs.size(), 3);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 3 4)");

  xs = xs.erase(3);

  BOOST_CHECK_EQUAL(xs.size(), 3);
  BOOST_CHECK_EQUAL(core::to_string(xs), "list(1 3 4)");
}

BOOST_AUTO_TEST_CASE(test_listC_subscript)
{
  Engine engine;

  auto xs = make_listC("0", "1", "2", "3", "4", "5", "6", "7");

  const auto& cxs = xs;

  BOOST_CHECK_EQUAL(xs[3], "3");
  BOOST_CHECK_EQUAL(xs[5], "5");
  BOOST_CHECK_EQUAL(xs[7], "7");

  BOOST_CHECK_EQUAL(cxs[3], "3");
  BOOST_CHECK_EQUAL(cxs[5], "5");
  BOOST_CHECK_EQUAL(cxs[7], "7");
}

BOOST_AUTO_TEST_CASE(test_listC_patch_apply)
{
  Engine engine;

  const auto a = make_listC(1, 2, 3, 4, 5);

  list_patch<int> patch;

  BOOST_CHECK(patch.empty());

  BOOST_CHECK_EQUAL(core::to_string(patch.apply(a)), "list(1 2 3 4 5)");

  patch.insert(2, 0);

  BOOST_CHECK_EQUAL(core::to_string(patch.apply(a)), "list(1 2 0 3 4 5)");

  patch.erase(4);

  BOOST_CHECK_EQUAL(core::to_string(patch.apply(a)), "list(1 2 0 3 5)");
}

BOOST_AUTO_TEST_CASE(test_listC_restore_patch_lengths_8_and_7)
{
  Engine engine;

  const auto xs = make_listC("0", "1", "2", "3", "4", "5", "6", "7");
  const auto ys = make_listC("0", "2", "8", "4", "9", "5", "1");

  list_patch<std::string> patch(ys, xs);

  const auto zs = make_listC("a", "b", "c", "d", "e", "f", "g", "h");

  const auto ws = patch.apply(zs);

  BOOST_CHECK_EQUAL(core::to_string(ws), "list(a c 8 e 9 f 1)");
}

BOOST_AUTO_TEST_CASE(test_listC_restore_patch_lengths_5_and_0)
{
  Engine engine;

  const auto xs = make_listC("0", "1", "2", "3", "4");
  const auto ys = list<std::string>{};

  list_patch<std::string> patch(ys, xs);

  const auto zs = make_listC("a", "b", "c", "d", "e", "f", "g", "h");

  const auto ws = patch.apply(zs);

  BOOST_CHECK_EQUAL(core::to_string(ws), "list(f g h)");
}

BOOST_AUTO_TEST_CASE(test_listC_restore_patch_lengths_0_and_5)
{
  Engine engine;

  const auto xs = list<std::string>{};
  const auto ys = make_listC("0", "1", "2", "3", "4");

  list_patch<std::string> patch(ys, xs);

  const auto zs = list<std::string>{};

  const auto ws = patch.apply(zs);

  BOOST_CHECK_EQUAL(core::to_string(ws), "list(0 1 2 3 4)");
}

BOOST_AUTO_TEST_CASE(test_listC_restore_patch_completely_different_lists)
{
  Engine engine;

  const auto xs = make_listC("0", "1", "2", "3", "4", "5", "6", "7", "8", "9");
  const auto ys = make_listC(
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n");

  list_patch<std::string> patch(ys, xs);

  const auto zs =
    make_listC("00", "11", "22", "33", "44", "55", "66", "77", "88", "99");

  const auto ws = patch.apply(zs);

  BOOST_CHECK_EQUAL(core::to_string(ws), "list(a b c d e f g h i j k l m n)");
}

BOOST_AUTO_TEST_CASE(test_listC_restore_patch_very_different_lists)
{
  Engine engine;

  const auto xs = make_listC("0", "1", "2", "3", "h", "5", "6", "j", "8", "9");
  const auto ys = make_listC(
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n");

  list_patch<std::string> patch(ys, xs);

  const auto zs =
    make_listC("00", "11", "22", "33", "44", "55", "66", "77", "88", "99");

  const auto ws = patch.apply(zs);

  BOOST_CHECK_EQUAL(core::to_string(ws), "list(a b c d e f g 44 i 77 k l m n)");
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
