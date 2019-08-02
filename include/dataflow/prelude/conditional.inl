
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

#if !defined(DATAFLOW___CONDITIONAL_H)
#error '.inl' file can't be included directly. Use 'conditional.h' instead
#endif

#include "comparison.h"

template <typename T>
dataflow::ref<T>
dataflow::Switch(const std::pair<ref<bool>, ref<T>>& first_case,
                 const ref<T>& default_case)
{
  return If(first_case.first, first_case.second, default_case);
}

template <typename T, typename... Cases>
dataflow::ref<T>
dataflow::Switch(const std::pair<ref<bool>, ref<T>>& first_case,
                 const Cases&... other_cases)
{
  return If(first_case.first, first_case.second, Switch(other_cases...));
}

template <typename T, typename U>
dataflow::ref<U> dataflow::Switch(const ref<T>& x,
                                  const std::pair<ref<T>, ref<U>>& first_case,
                                  const ref<U>& default_case)
{
  return If(x == first_case.first, first_case.second, default_case);
}

template <typename T, typename U, typename... Cases>
dataflow::ref<U> dataflow::Switch(const ref<T>& x,
                                  const std::pair<ref<T>, ref<U>>& first_case,
                                  const Cases&... other_cases)
{
  return If(
    x == first_case.first, first_case.second, Switch(x, other_cases...));
}

template <typename ArgT, typename ArgU, typename T, typename U>
std::pair<dataflow::ref<T>, dataflow::ref<U>> dataflow::Case(const ArgT& x,
                                                             const ArgU& y)
{
  return std::make_pair(core::make_argument(x), core::make_argument(y));
}

template <typename ArgT, typename T>
dataflow::ref<T> dataflow::Default(const ArgT& x)
{
  return core::make_argument(x);
}

template <typename ArgT, typename ArgU, typename R>
R dataflow::operator>>=(ArgT&& x, ArgU&& y)
{
  return Case(std::forward<ArgT>(x), std::forward<ArgU>(y));
}
