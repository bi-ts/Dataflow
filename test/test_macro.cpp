
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

#include <dataflow/macro.h>

#include <dataflow/introspect.h>
#include <dataflow/string.h>

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{

DATAFLOW_DATA(mixed,
              // Some documentation for the first field
              (dataflow::ref<int>, First),
              // Some documentation for the second field
              (std::string, Second),
              // Some documentation for the third field
              (dataflow::ref<char>, Third));

using namespace dataflow;

BOOST_AUTO_TEST_SUITE(test_macro)

// clang-format off

#define TEST_TUPLE                                                             \
  ( 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,             \
   17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,             \
   33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,             \
   49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64)

// clang-format on

#define TEST_TIMES_100(r, data, idx, elem) DATAFLOW___COMMA_IF(idx) elem * 100

BOOST_AUTO_TEST_CASE(test_DATAFLOW___TUPLE_ELEM)
{
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(0, TEST_TUPLE), 1);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(1, TEST_TUPLE), 2);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(2, TEST_TUPLE), 3);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(3, TEST_TUPLE), 4);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(4, TEST_TUPLE), 5);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(5, TEST_TUPLE), 6);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(6, TEST_TUPLE), 7);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(7, TEST_TUPLE), 8);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(8, TEST_TUPLE), 9);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(9, TEST_TUPLE), 10);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(10, TEST_TUPLE), 11);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(11, TEST_TUPLE), 12);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(12, TEST_TUPLE), 13);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(13, TEST_TUPLE), 14);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(14, TEST_TUPLE), 15);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(15, TEST_TUPLE), 16);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(16, TEST_TUPLE), 17);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(17, TEST_TUPLE), 18);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(18, TEST_TUPLE), 19);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(19, TEST_TUPLE), 20);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(20, TEST_TUPLE), 21);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(21, TEST_TUPLE), 22);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(22, TEST_TUPLE), 23);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(23, TEST_TUPLE), 24);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(24, TEST_TUPLE), 25);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(25, TEST_TUPLE), 26);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(26, TEST_TUPLE), 27);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(27, TEST_TUPLE), 28);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(28, TEST_TUPLE), 29);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(29, TEST_TUPLE), 30);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(30, TEST_TUPLE), 31);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(31, TEST_TUPLE), 32);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(32, TEST_TUPLE), 33);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(33, TEST_TUPLE), 34);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(34, TEST_TUPLE), 35);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(35, TEST_TUPLE), 36);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(36, TEST_TUPLE), 37);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(37, TEST_TUPLE), 38);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(38, TEST_TUPLE), 39);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(39, TEST_TUPLE), 40);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(40, TEST_TUPLE), 41);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(41, TEST_TUPLE), 42);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(42, TEST_TUPLE), 43);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(43, TEST_TUPLE), 44);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(44, TEST_TUPLE), 45);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(45, TEST_TUPLE), 46);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(46, TEST_TUPLE), 47);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(47, TEST_TUPLE), 48);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(48, TEST_TUPLE), 49);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(49, TEST_TUPLE), 50);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(50, TEST_TUPLE), 51);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(51, TEST_TUPLE), 52);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(52, TEST_TUPLE), 53);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(53, TEST_TUPLE), 54);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(54, TEST_TUPLE), 55);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(55, TEST_TUPLE), 56);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(56, TEST_TUPLE), 57);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(57, TEST_TUPLE), 58);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(58, TEST_TUPLE), 59);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(59, TEST_TUPLE), 60);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(60, TEST_TUPLE), 61);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(61, TEST_TUPLE), 62);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(62, TEST_TUPLE), 63);
  BOOST_CHECK_EQUAL(DATAFLOW___TUPLE_ELEM(63, TEST_TUPLE), 64);
}

BOOST_AUTO_TEST_CASE(test_DATAFLOW___TUPLE_FOR_EACH_I)
{
  int data[] = {DATAFLOW___TUPLE_FOR_EACH_I(TEST_TIMES_100, _, TEST_TUPLE)};

  BOOST_CHECK_EQUAL(sizeof(data) / sizeof(int), 64);

  for (std::size_t i = 0; i < 64; ++i)
  {
    BOOST_CHECK_EQUAL(data[i], (i + 1) * 100);
  }
}

BOOST_AUTO_TEST_CASE(test_DATAFLOW_DATA_mixed_Var)
{
  Engine engine;

  auto a = Var(0);
  auto c = Var('c');
  auto x = Var<mixed>(a, "str", c);
  auto y = Main(x);

  BOOST_CHECK_EQUAL(introspect::value(y), "mixed");
}

BOOST_AUTO_TEST_CASE(test_DATAFLOW_DATA_mixed_selectors)
{
  Engine engine;

  auto a = Var(0);
  auto c = Var('c');
  auto x = Var<mixed>(a, "str", c);

  auto first = First(x);

  BOOST_CHECK_EQUAL(introspect::label(first), "mixed-First");

  auto second = Second(x);

  BOOST_CHECK_EQUAL(introspect::label(second), "mixed-Second");

  auto third = Third(x);

  BOOST_CHECK_EQUAL(introspect::label(third), "mixed-Third");

  auto y = Main(first);

  BOOST_CHECK_EQUAL(*y, 0);

  a = 1;

  BOOST_CHECK_EQUAL(*y, 1);
}

BOOST_AUTO_TEST_CASE(test_DATAFLOW_DATA_mixed_equality_comparison)
{
  Engine engine;

  auto a1 = Var(1);
  auto a2 = Var(1);
  auto c = Var('c');
  const auto m1 = mixed(a1, "b", c);
  const auto m2 = mixed(a2, "b", c);

  BOOST_CHECK_EQUAL(m1 == m2, false);
  BOOST_CHECK_EQUAL(m1 == mixed(a1, "b", c), true);
  BOOST_CHECK_EQUAL(m2 == mixed(a2, "b", c), true);
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
