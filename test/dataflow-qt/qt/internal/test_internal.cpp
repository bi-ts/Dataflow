
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
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

#include <dataflow-qt/qt.h>

#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <vector>

namespace dataflow2qt_test
{
namespace
{
template <std::size_t... Is>
std::vector<std::size_t> get_indices(dataflow::std14::index_sequence<Is...> seq)
{
  return {Is...};
}
}

BOOST_AUTO_TEST_SUITE(test_dataflow2qt_internal)

BOOST_AUTO_TEST_CASE(test_qt_internal_make_filtered_index_sequence)
{
  const auto seq_a = get_indices(
    dataflow2qt::internal::
      make_filtered_index_sequence<false, false, true, false, true, true>{});

  BOOST_TEST(seq_a == std::vector<std::size_t>({2, 4, 5}),
             boost::test_tools::per_element());

  const auto seq_b =
    get_indices(dataflow2qt::internal::make_filtered_index_sequence<>{});

  BOOST_TEST(seq_b.empty());
}

BOOST_AUTO_TEST_CASE(test_regression_qt_internal_make_filtered_index_sequence)
{
  const auto seq_a = get_indices(
    dataflow2qt::internal::make_filtered_index_sequence<true, true>{});

  BOOST_TEST(seq_a == std::vector<std::size_t>({0, 1}),
             boost::test_tools::per_element());

  const auto seq_b =
    get_indices(dataflow2qt::internal::make_filtered_index_sequence<>{});

  BOOST_TEST(seq_b.empty());
}

BOOST_AUTO_TEST_CASE(test_qt_internal_for_each_tuple_element)
{
  const std::tuple<int, char, double> t = std::make_tuple(1, 'A', 3.5);

  int accumulator = 0;

  dataflow2qt::internal::for_each_tuple_element(
    t, [&accumulator](const auto& v) { accumulator += static_cast<int>(v); });

  BOOST_CHECK_EQUAL(accumulator, 69);
}

BOOST_AUTO_TEST_CASE(test_qt_internal_for_each_tuple_element_with_indices)
{
  const std::tuple<int, std::string, double, const char*, double> t =
    std::make_tuple(1, std::string{"some string"}, 3.5, "other string", 12.5);

  int accumulator = 0;

  dataflow2qt::internal::for_each_tuple_element(
    t,
    dataflow::std14::index_sequence<0, 2, 4>{},
    [&accumulator](const auto& v) { accumulator += static_cast<int>(v); });

  BOOST_CHECK_EQUAL(accumulator, 16);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow2qt_test
