
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

template <typename T, typename U>
dataflow::ref<U>
dataflow::Switch(const ref<T>& x,
                 const std::pair<ref<T>, ref<U>>& first_case,
                 const std::pair<std::true_type, ref<U>>& default_case)
{
  return If(x == first_case.first, first_case.second, default_case.second);
}

template <typename T, typename U, typename... Cases>
dataflow::ref<U> dataflow::Switch(const ref<T>& x,
                                  const std::pair<ref<T>, ref<U>>& first_case,
                                  const Cases&... other_cases)
{
  return If(
    x == first_case.first, first_case.second, Switch(x, other_cases...));
}

template <typename FArgT, typename FArgU>
dataflow::core::
  farg_result_t<dataflow::core::farg_data_type_t<FArgT>, FArgT, FArgU>
  dataflow::Switch(const std::pair<dataflow::ref<bool>, FArgT>& first_case,
                   const std::pair<std::true_type, FArgU>& default_case)
{
  return If(first_case.first, first_case.second, default_case.second);
}

template <typename FArg, typename... Conds, typename... FArgs>
dataflow::core::
  farg_result_t<dataflow::core::farg_data_type_t<FArg>, FArg, FArgs...>
  dataflow::Switch(const std::pair<dataflow::ref<bool>, FArg>& first_case,
                   const std::pair<Conds, FArgs>&... other_cases)
{
  return If(first_case.first, first_case.second, Switch(other_cases...));
}

template <typename ArgT, typename FArgU>
std::pair<
  dataflow::ref<dataflow::core::argument_data_type_t<ArgT>>,
  dataflow::core::farg_result_t<dataflow::core::farg_data_type_t<FArgU>, FArgU>>
dataflow::Case(const ArgT& x, const FArgU& y)
{
  return std::make_pair(core::make_argument(x), core::make_farg(y));
}

template <typename FArgT>
std::pair<
  std::true_type,
  dataflow::core::farg_result_t<dataflow::core::farg_data_type_t<FArgT>, FArgT>>
dataflow::Default(const FArgT& x)
{
  return std::make_pair(std::true_type{}, core::make_farg(x));
}

template <typename ArgT, typename FArgU>
std::pair<
  dataflow::ref<dataflow::core::argument_data_type_t<ArgT>>,
  dataflow::core::farg_result_t<dataflow::core::farg_data_type_t<FArgU>, FArgU>>
dataflow::operator>>=(const ArgT& x, const FArgU& y)
{
  return Case(x, y);
}
