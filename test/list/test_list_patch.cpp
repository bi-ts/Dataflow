
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
template <typename T>
bool list_patch_invariant_holds(const list_patch<T>& patch)
{
  std::vector<integer> indices;

  patch.apply([&](integer idx, const T&) { indices.push_back(idx); },
              [&](integer idx) { indices.push_back(idx); });

  return std::is_sorted(indices.begin(), indices.end());
}

BOOST_AUTO_TEST_SUITE(test_list_patch)

BOOST_AUTO_TEST_CASE(test_list_internal_fill_table)
{
  const auto cols = 10;
  const auto rows = 10;
  int table[rows][cols] = {{0}};

  std::string s0 = "";
  std::string s4 = "abcd";
  std::string s5 = "abxde";

  const auto result_s0_s4 = list_internal::fill_table(s0.begin(),
                                                      s0.end(),
                                                      s0.size(),
                                                      s4.begin(),
                                                      s4.end(),
                                                      s4.size(),
                                                      &table[0][0],
                                                      cols);

  BOOST_CHECK_EQUAL(result_s0_s4.skip, 0);
  BOOST_CHECK_EQUAL(result_s0_s4.rows, 1);
  BOOST_CHECK_EQUAL(result_s0_s4.cols, 5);

  const auto result_s4_s0 = list_internal::fill_table(s4.begin(),
                                                      s4.end(),
                                                      s4.size(),
                                                      s0.begin(),
                                                      s0.end(),
                                                      s0.size(),
                                                      &table[0][0],
                                                      cols);

  BOOST_CHECK_EQUAL(result_s4_s0.skip, 0);
  BOOST_CHECK_EQUAL(result_s4_s0.rows, 5);
  BOOST_CHECK_EQUAL(result_s4_s0.cols, 1);

  const auto result_s4_s4 = list_internal::fill_table(s4.begin(),
                                                      s4.end(),
                                                      s4.size(),
                                                      s4.begin(),
                                                      s4.end(),
                                                      s4.size(),
                                                      &table[0][0],
                                                      cols);

  BOOST_CHECK_EQUAL(result_s4_s4.skip, 4);
  BOOST_CHECK_EQUAL(result_s4_s4.rows, 1);
  BOOST_CHECK_EQUAL(result_s4_s4.cols, 1);

  const auto result_s4_s5 = list_internal::fill_table(s4.begin(),
                                                      s4.end(),
                                                      s4.size(),
                                                      s5.begin(),
                                                      s5.end(),
                                                      s5.size(),
                                                      &table[0][0],
                                                      cols);

  BOOST_CHECK_EQUAL(result_s4_s5.skip, 2);
  BOOST_CHECK_EQUAL(result_s4_s5.rows, 3);
  BOOST_CHECK_EQUAL(result_s4_s5.cols, 4);

  const auto result_s5_s4 = list_internal::fill_table(s5.begin(),
                                                      s5.end(),
                                                      s5.size(),
                                                      s4.begin(),
                                                      s4.end(),
                                                      s4.size(),
                                                      &table[0][0],
                                                      cols);

  BOOST_CHECK_EQUAL(result_s5_s4.skip, 2);
  BOOST_CHECK_EQUAL(result_s5_s4.rows, 4);
  BOOST_CHECK_EQUAL(result_s5_s4.cols, 3);
}

BOOST_AUTO_TEST_CASE(test_listC_patch_apply)
{
  Engine engine;

  const auto a = make_listC(1, 2, 3, 4, 5);

  list_patch<int> patch;

  BOOST_CHECK(patch.empty());
  BOOST_CHECK(list_patch_invariant_holds(patch));

  BOOST_CHECK_EQUAL(core::to_string(patch.apply(a)), "list(1 2 3 4 5)");

  patch.insert(2, 0);
  BOOST_CHECK(list_patch_invariant_holds(patch));

  BOOST_CHECK_EQUAL(core::to_string(patch.apply(a)), "list(1 2 0 3 4 5)");

  patch.erase(4);
  BOOST_CHECK(list_patch_invariant_holds(patch));

  BOOST_CHECK_EQUAL(core::to_string(patch.apply(a)), "list(1 2 0 3 5)");
}

BOOST_AUTO_TEST_CASE(test_listC_restore_patch_lengths_8_and_7)
{
  Engine engine;

  const auto xs = make_listC("0", "1", "2", "3", "4", "5", "6", "7");
  const auto ys = make_listC("0", "2", "8", "4", "9", "5", "1");

  list_patch<std::string> patch(ys, xs);

  BOOST_CHECK(list_patch_invariant_holds(patch));

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

  BOOST_CHECK(list_patch_invariant_holds(patch));

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

  BOOST_CHECK(list_patch_invariant_holds(patch));

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

  BOOST_CHECK(list_patch_invariant_holds(patch));

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

  BOOST_CHECK(list_patch_invariant_holds(patch));

  const auto zs =
    make_listC("00", "11", "22", "33", "44", "55", "66", "77", "88", "99");

  const auto ws = patch.apply(zs);

  BOOST_CHECK_EQUAL(core::to_string(ws), "list(a b c d e f g 44 i 77 k l m n)");
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
