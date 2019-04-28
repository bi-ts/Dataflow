
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include <ostream>
#include <type_traits>

namespace dataflow
{
namespace internal
{

template <typename T> struct is_streamable
{
private:
  template <typename U>
  static decltype(std::declval<std::ostream&>() << std::declval<U>(),
                  std::true_type())
  test_(int);

  template <typename> static std::false_type test_(...);

public:
  static constexpr const bool value = decltype(test_<T>(0))::value;
};

template <typename T> constexpr const bool is_streamable<T>::value;

template <typename T> struct is_equality_comparable
{
private:
  template <typename U>
  static auto test_(int) -> decltype(
    std::is_same<bool, decltype(std::declval<U>() == std::declval<U>())>::value,
    std::is_same<bool, decltype(std::declval<U>() != std::declval<U>())>::value,
    std::true_type());

  template <typename> static std::false_type test_(...);

public:
  static constexpr const bool value = decltype(test_<T>(0))::value;
};

template <typename T> constexpr const bool is_equality_comparable<T>::value;
}
}
