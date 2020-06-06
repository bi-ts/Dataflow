
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

#include <dataflow/utility/utility.h>

#include <boost/test/unit_test.hpp>

#include <string>

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_dataflow_utility)

BOOST_AUTO_TEST_CASE(test_for_each_tuple_element)
{
  const std::tuple<int, char, double> t = std::make_tuple(1, 'A', 3.5);

  int accumulator = 0;

  dataflow::utility::for_each_tuple_element(
    t, [&accumulator](const auto& v) { accumulator += static_cast<int>(v); });

  BOOST_CHECK_EQUAL(accumulator, 69);
}

BOOST_AUTO_TEST_CASE(test_for_each_tuple_element_with_indices)
{
  const std::tuple<int, std::string, double, const char*, double> t =
    std::make_tuple(1, std::string{"some string"}, 3.5, "other string", 12.5);

  int accumulator = 0;

  dataflow::utility::for_each_tuple_element(
    t,
    dataflow::std14::index_sequence<0, 2, 4>{},
    [&accumulator](const auto& v) { accumulator += static_cast<int>(v); });

  BOOST_CHECK_EQUAL(accumulator, 16);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
