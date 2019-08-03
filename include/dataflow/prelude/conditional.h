
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

#ifndef DATAFLOW___CONDITIONAL_H
#define DATAFLOW___CONDITIONAL_H

#include "core.h"

namespace dataflow
{
/// \defgroup conditional
/// \{

template <typename T, typename U>
ref<U> Switch(const ref<T>& x,
              const std::pair<ref<T>, ref<U>>& first_case,
              const std::pair<std::true_type, ref<U>>& default_case);

template <typename T, typename U, typename... Cases>
ref<U> Switch(const ref<T>& x,
              const std::pair<ref<T>, ref<U>>& first_case,
              const Cases&... other_cases);

template <typename FArgT, typename FArgU>
core::farg_result_t<core::farg_data_type_t<FArgT>, FArgT, FArgU>
Switch(const std::pair<ref<bool>, FArgT>& first_case,
       const std::pair<std::true_type, FArgU>& default_case);

template <typename FArg, typename... Conds, typename... FArgs>
core::farg_result_t<core::farg_data_type_t<FArg>, FArg, FArgs...>
Switch(const std::pair<ref<bool>, FArg>& first_case,
       const std::pair<Conds, FArgs>&... other_cases);

template <typename ArgT, typename FArgU>
std::pair<ref<core::argument_data_type_t<ArgT>>,
          core::farg_result_t<core::farg_data_type_t<FArgU>, FArgU>>
Case(const ArgT& x, const FArgU& y);

template <typename FArgT>
std::pair<std::true_type,
          core::farg_result_t<core::farg_data_type_t<FArgT>, FArgT>>
Default(const FArgT& x);

template <typename ArgT, typename FArgU>
std::pair<ref<core::argument_data_type_t<ArgT>>,
          core::farg_result_t<core::farg_data_type_t<FArgU>, FArgU>>
operator>>=(const ArgT& x, const FArgU& y);

/// \}
} // dataflow

#include "conditional.inl"

#endif // DATAFLOW___CONDITIONAL_H
