
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

#ifndef DATAFLOW___CONDITIONAL_H
#define DATAFLOW___CONDITIONAL_H

#include "core.h"

namespace dataflow
{
/// \defgroup conditional
/// \{

template <typename T>
ref<T> Switch(const std::pair<ref<bool>, ref<T>>& first_case,
              const ref<T>& default_case);

template <typename T, typename... Cases>
ref<T> Switch(const std::pair<ref<bool>, ref<T>>& first_case,
              const Cases&... other_cases);

template <typename T, typename U>
ref<U> Switch(const ref<T>& x,
              const std::pair<ref<T>, ref<U>>& first_case,
              const ref<U>& default_case);

template <typename T, typename U, typename... Cases>
ref<U> Switch(const ref<T>& x,
              const std::pair<ref<T>, ref<U>>& first_case,
              const Cases&... other_cases);

template <typename T, typename U>
std::pair<ref<T>, ref<U>> Case(const ref<T>& x, const ref<U>& y);

template <typename T, typename U, typename FwU = core::convert_to_flowable_t<U>>
std::pair<ref<T>, ref<FwU>> Case(const ref<T>& x, const U& y);

template <typename T, typename U, typename FwT = core::convert_to_flowable_t<T>>
std::pair<ref<FwT>, ref<U>> Case(const T& x, const ref<U>& y);

template <typename T,
          typename U,
          typename FwT = core::convert_to_flowable_t<T>,
          typename FwU = core::convert_to_flowable_t<U>>
std::pair<ref<FwT>, ref<FwU>> Case(const T& x, const U& y);

template <typename T> ref<T> Default(const ref<T>& x);

template <typename T, typename FwT = core::convert_to_flowable_t<T>>
ref<FwT> Default(const T& v);

template <typename T>
std::pair<ref<bool>, ref<T>> operator>>=(const ref<bool>& x, const ref<T>& y);

template <typename T, typename FwT = core::convert_to_flowable_t<T>>
std::pair<ref<bool>, ref<FwT>> operator>>=(const ref<bool>& x, const T& y);

/// \}
} // dataflow

#include "conditional.inl"

#endif // DATAFLOW___CONDITIONAL_H
