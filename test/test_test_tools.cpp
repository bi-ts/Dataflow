
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

#include "tools/test_tools.h"

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{

BOOST_AUTO_TEST_SUITE(test_test_tools)

template <typename, typename U> struct always_enabled
{
  using type = U;
};

template <typename, typename> struct always_disabled
{
};

template <typename, typename U> using always_enabled_t = U;

template <typename T, typename U>
using always_disabled_t = typename always_disabled<T, U>::type;

BOOST_AUTO_TEST_CASE(test_test_tools_is_enabled_for_type)
{
  BOOST_CHECK_EQUAL((test_tools::is_enabled_for_type<always_enabled, int>()),
                    true);
  BOOST_CHECK_EQUAL((test_tools::is_enabled_for_type<always_enabled_t, int>()),
                    true);
  BOOST_CHECK_EQUAL((test_tools::is_enabled_for_type<always_disabled, int>()),
                    false);
  BOOST_CHECK_EQUAL((test_tools::is_enabled_for_type<always_disabled_t, int>()),
                    false);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace dataflow_test
