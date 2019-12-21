
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

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_list_list)

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

BOOST_AUTO_TEST_CASE(test_listA_concat)
{
  Engine engine;

  const auto c0 = Const(0);
  const auto c1 = Const(1);
  const auto c2 = Const(2);
  const auto c3 = Const(3);
  const auto c4 = Const(4);

  const auto xs = make_listA(c0, c1, c2);
  const auto ys = make_listA(c3, c4);
  const auto zs = xs.concat(ys);

  BOOST_CHECK_EQUAL(xs.size(), 3);
  BOOST_CHECK_EQUAL(ys.size(), 2);
  BOOST_CHECK_EQUAL(zs.size(), 5);

  BOOST_CHECK_EQUAL(zs[0].id(), c0.id());
  BOOST_CHECK_EQUAL(zs[1].id(), c1.id());
  BOOST_CHECK_EQUAL(zs[2].id(), c2.id());
  BOOST_CHECK_EQUAL(zs[3].id(), c3.id());
  BOOST_CHECK_EQUAL(zs[4].id(), c4.id());
}

BOOST_AUTO_TEST_CASE(test_listC_concat)
{
  Engine engine;

  const auto xs = make_listC(0, 1, 2, 3, 4, 5);
  const auto ys = make_listC(6, 7, 8, 9);
  const auto zs = xs.concat(ys);

  BOOST_TEST(xs == std::vector<int>({0, 1, 2, 3, 4, 5}),
             boost::test_tools::per_element());

  BOOST_TEST(ys == std::vector<int>({6, 7, 8, 9}),
             boost::test_tools::per_element());

  BOOST_TEST(zs == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}),
             boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(test_listA_take)
{
  Engine engine;

  const auto c0 = Const(0);
  const auto c1 = Const(1);
  const auto c2 = Const(2);
  const auto c3 = Const(3);
  const auto c4 = Const(4);

  const auto xs = make_listA(c0, c1, c2, c3, c4);
  const auto ys = xs.take(3);

  BOOST_CHECK_EQUAL(xs.size(), 5);
  BOOST_CHECK_EQUAL(ys.size(), 3);

  BOOST_CHECK_EQUAL(ys[0].id(), c0.id());
  BOOST_CHECK_EQUAL(ys[1].id(), c1.id());
  BOOST_CHECK_EQUAL(ys[2].id(), c2.id());
}

BOOST_AUTO_TEST_CASE(test_listC_take)
{
  Engine engine;

  const auto xs = make_listC(0, 1, 2, 3, 4, 5);
  const auto ys = xs.take(3);

  BOOST_TEST(xs == std::vector<int>({0, 1, 2, 3, 4, 5}),
             boost::test_tools::per_element());

  BOOST_TEST(ys == std::vector<int>({0, 1, 2}),
             boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(test_listA_skip)
{
  Engine engine;

  const auto c0 = Const(0);
  const auto c1 = Const(1);
  const auto c2 = Const(2);
  const auto c3 = Const(3);
  const auto c4 = Const(4);

  const auto xs = make_listA(c0, c1, c2, c3, c4);
  const auto ys = xs.skip(3);

  BOOST_CHECK_EQUAL(xs.size(), 5);
  BOOST_CHECK_EQUAL(ys.size(), 2);

  BOOST_CHECK_EQUAL(ys[0].id(), c3.id());
  BOOST_CHECK_EQUAL(ys[1].id(), c4.id());
}

BOOST_AUTO_TEST_CASE(test_listC_skip)
{
  Engine engine;

  const auto xs = make_listC(0, 1, 2, 3, 4, 5);
  const auto ys = xs.skip(4);

  BOOST_TEST(xs == std::vector<int>({0, 1, 2, 3, 4, 5}),
             boost::test_tools::per_element());

  BOOST_TEST(ys == std::vector<int>({4, 5}), boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(test_listC_subscript)
{
  Engine engine;

  /*no const*/ auto xs = make_listC("0", "1", "2", "3", "4", "5", "6", "7");

  const auto& cxs = xs;

  BOOST_CHECK_EQUAL(xs[3], "3");
  BOOST_CHECK_EQUAL(xs[5], "5");
  BOOST_CHECK_EQUAL(xs[7], "7");

  BOOST_CHECK_EQUAL(cxs[3], "3");
  BOOST_CHECK_EQUAL(cxs[5], "5");
  BOOST_CHECK_EQUAL(cxs[7], "7");
}

BOOST_AUTO_TEST_CASE(test_listC_begin_end)
{
  Engine engine;

  const auto xs = make_listC(0, 1, 2, 3, 4, 5, 6, 7);

  std::vector<int> tmp;

  std::copy(xs.begin(), xs.end(), std::back_inserter(tmp));

  BOOST_TEST(tmp == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7}),
             boost::test_tools::per_element());
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
