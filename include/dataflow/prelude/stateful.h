
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

#ifndef DATAFLOW___STATEFUL_H
#define DATAFLOW___STATEFUL_H

#include "../internal/std_future.h"

#include "core.h"

#include <cstdint>
#include <tuple>
#include <utility>

namespace dataflow
{
/// \defgroup stateful
/// \{

namespace stateful
{
namespace detail
{
template <typename F, typename T> struct is_sm_definition_function;
}

template <typename F, typename T>
using is_sm_definition_function =
  typename detail::is_sm_definition_function<F, T>::type;
}

template <typename ArgT,
          typename T = core::argument_data_type_t<ArgT>,
          typename F,
          typename = typename std::enable_if<
            stateful::is_sm_definition_function<F, T>::value>::type>
ref<T> StateMachine(const ArgT& initial, const F& f, const Time& t0);

template <typename ArgT,
          typename T = core::argument_data_type_t<ArgT>,
          typename F,
          typename = typename std::enable_if<
            stateful::is_sm_definition_function<F, T>::value>::type>
function_of_time<T> StateMachine(const ArgT& initial, const F& f);

template <typename... Trs>
std::tuple<Trs...> Transitions(const Trs&... transitions);

template <typename ArgT, typename FArgU>
std::pair<ref<core::enable_for_argument_data_type_t<ArgT, bool>>,
          core::farg_result_t<core::farg_data_type_t<FArgU>, FArgU>>
On(const ArgT& x, const FArgU& y);

/// \}
} // dataflow

#include "stateful.inl"

#endif // DATAFLOW___STATEFUL_H
