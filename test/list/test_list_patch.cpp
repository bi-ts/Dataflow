
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
  enum op_type
  {
    op_insert,
    op_erase
  };

  std::vector<std::pair<integer, op_type>> edits;

  patch.apply(
    [&](integer idx, const T&) {
      edits.push_back({idx, op_insert});
    },
    [&](integer idx) {
      edits.push_back({idx, op_erase});
    });

  if (!std::is_sorted(
        edits.begin(), edits.end(), [](const auto& lhs, const auto& rhs) {
          return lhs.first < rhs.first;
        }))

    return false;

  if (edits.size() > 1)
  {
    for (auto it1 = edits.begin(); it1 != edits.end(); ++it1)
    {
      for (auto it2 = it1; ++it2 != edits.end();)
      {
        if (it1->first == it2->first && it1->second == op_insert &&
            it2->second == op_erase)
        {
          std::cout << "Patch is not normalized: the element inserted at "
                    << it1->first << ", immediately erased" << std::endl;

          return false;
        }
      }
    }
  }

  return true;
}

BOOST_AUTO_TEST_SUITE(test_list_patch)

BOOST_AUTO_TEST_CASE(test_list_internal_table)
{
  const auto rows = 3;
  const auto cols = 2;
  std::array<std::array<int, cols>, 3> data = {{{1, 2}, {3, 4}, {5, 6}}};

  BOOST_CHECK_EQUAL(data[0][0], 1);
  BOOST_CHECK_EQUAL(data[0][1], 2);
  BOOST_CHECK_EQUAL(data[1][0], 3);
  BOOST_CHECK_EQUAL(data[1][1], 4);
  BOOST_CHECK_EQUAL(data[2][0], 5);
  BOOST_CHECK_EQUAL(data[2][1], 6);

  list_internal::table<int, int> table(&data[0][0], cols, rows * cols);

  BOOST_CHECK_EQUAL(data[0][0], 0);
  BOOST_CHECK_EQUAL(data[0][1], 0);
  BOOST_CHECK_EQUAL(data[1][0], 0);
  BOOST_CHECK_EQUAL(data[1][1], 4);
  BOOST_CHECK_EQUAL(data[2][0], 0);
  BOOST_CHECK_EQUAL(data[2][1], 6);

  BOOST_CHECK_EQUAL(table[0][0], 0);
  BOOST_CHECK_EQUAL(table[0][1], 0);
  BOOST_CHECK_EQUAL(table[1][0], 0);
  BOOST_CHECK_EQUAL(table[1][1], 4);
  BOOST_CHECK_EQUAL(table[2][0], 0);
  BOOST_CHECK_EQUAL(table[2][1], 6);
}

BOOST_AUTO_TEST_CASE(test_list_internal_fill_table)
{
  const auto cols = 7;
  const auto rows = 5;
  int data[rows][cols] = {{1, 2, 3, 4, 5, 6, 7},
                          {2, 8, 8, 8, 8, 8, 8},
                          {3, 8, 9, 9, 9, 9, 9},
                          {4, 8, 9, 1, 1, 1, 1},
                          {5, 8, 9, 1, 1, 1, 1}};

  list_internal::table<int, std::size_t> table(&data[0][0], cols, rows * cols);

  std::string s0 = "";
  std::string s4 = "abcd";
  std::string s5 = "abxde";

  BOOST_CHECK_EQUAL(table.rows(), 1);
  BOOST_CHECK_EQUAL(table.cols(), 1);

  const auto skip_s0_s4 = table.fill(
    s0.begin(), s0.end(), s4.begin(), s4.end(), s0.size(), s4.size());

  BOOST_CHECK_EQUAL(skip_s0_s4, 0);
  BOOST_CHECK_EQUAL(table.rows(), 1);
  BOOST_CHECK_EQUAL(table.cols(), 5);

  const auto skip_s4_s0 = table.fill(
    s4.begin(), s4.end(), s0.begin(), s0.end(), s4.size(), s0.size());

  BOOST_CHECK_EQUAL(skip_s4_s0, 0);
  BOOST_CHECK_EQUAL(table.rows(), 5);
  BOOST_CHECK_EQUAL(table.cols(), 1);

  const auto skip_s4_s4 = table.fill(
    s4.begin(), s4.end(), s4.begin(), s4.end(), s4.size(), s4.size());

  BOOST_CHECK_EQUAL(skip_s4_s4, 4);
  BOOST_CHECK_EQUAL(table.rows(), 1);
  BOOST_CHECK_EQUAL(table.cols(), 1);

  const auto skip_s4_s5 = table.fill(
    s4.begin(), s4.end(), s5.begin(), s5.end(), s4.size(), s5.size());

  BOOST_CHECK_EQUAL(skip_s4_s5, 2);
  BOOST_CHECK_EQUAL(table.rows(), 3);
  BOOST_CHECK_EQUAL(table.cols(), 4);

  const auto skip_s5_s4 = table.fill(
    s5.begin(), s5.end(), s4.begin(), s4.end(), s5.size(), s4.size());

  BOOST_CHECK_EQUAL(skip_s5_s4, 2);
  BOOST_CHECK_EQUAL(table.rows(), 4);
  BOOST_CHECK_EQUAL(table.cols(), 3);
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

BOOST_AUTO_TEST_CASE(test_listC_erase_insert)
{
  Engine engine;

  list_patch<std::string> patch;

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.erase(0);

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.insert(0, "0");

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.insert(0, "1");

  BOOST_CHECK(list_patch_invariant_holds(patch));

  const auto xs = make_listC("a", "b", "c");

  const auto ys = patch.apply(xs);

  BOOST_CHECK_EQUAL(core::to_string(ys), "list(1 0 b c)");
}

BOOST_AUTO_TEST_CASE(test_listC_insert_erase_normalization)
{
  Engine engine;

  list_patch<std::string> patch;

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.insert(0, "0");

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.insert(0, "1");

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.erase(0);

  BOOST_CHECK(list_patch_invariant_holds(patch));

  const auto xs = make_listC("a", "b", "c");

  const auto ys = patch.apply(xs);

  BOOST_CHECK_EQUAL(core::to_string(ys), "list(0 a b c)");
}

BOOST_AUTO_TEST_CASE(test_listC_erase_insert_normalization)
{
  Engine engine;

  list_patch<std::string> patch;

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.insert(0, "0");

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.insert(0, "1");

  BOOST_CHECK(list_patch_invariant_holds(patch));

  patch.erase(0);

  BOOST_CHECK(list_patch_invariant_holds(patch));

  const auto xs = make_listC("a", "b", "c");

  const auto ys = patch.apply(xs);

  BOOST_CHECK_EQUAL(core::to_string(ys), "list(0 a b c)");
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
