
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

#pragma once

#include <type_traits>

namespace test_tools
{
namespace detail
{
struct is_enabled_for_type_test_type;

template <template <typename, typename> class Enable,
          typename C,
          typename U = typename Enable<C, is_enabled_for_type_test_type>::type,
          typename = typename std::enable_if<
            std::is_same<U, is_enabled_for_type_test_type>::value>::type>
bool is_enabled_for_type(int)
{
  return true;
}

template <template <typename, typename> class Enable, typename C>
bool is_enabled_for_type(...)
{
  return false;
}

template <template <typename, typename> class Enable,
          typename C,
          typename U = Enable<C, is_enabled_for_type_test_type>,
          typename = typename std::enable_if<
            std::is_same<U, is_enabled_for_type_test_type>::value>::type>
bool is_enabled_for_type_t(int)
{
  return true;
}

template <template <typename, typename> class Enable, typename C>
bool is_enabled_for_type_t(...)
{
  return false;
}
}

template <template <typename, typename> class Enable, typename C>
bool is_enabled_for_type()
{
  return detail::is_enabled_for_type<Enable, C>(-1) ||
         detail::is_enabled_for_type_t<Enable, C>(0);
}
}
