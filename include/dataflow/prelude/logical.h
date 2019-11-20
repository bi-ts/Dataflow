
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

#ifndef DATAFLOW___LOGICAL_H
#define DATAFLOW___LOGICAL_H

#include "dataflow++_export.h"

#include "core.h"

namespace dataflow
{
/// \defgroup logical
/// \{

// Logical functions

DATAFLOW___EXPORT ref<bool> Not(const arg<bool>& x);

template <typename Arg,
          typename... Args,
          typename = core::enable_for_argument_data_type_t<
            bool,
            core::common_argument_data_type_t<Arg, Args...>>>
ref<bool> And(const Arg& x, const Args&... xs);

template <typename Arg,
          typename... Args,
          typename = core::enable_for_argument_data_type_t<
            bool,
            core::common_argument_data_type_t<Arg, Args...>>>
ref<bool> Or(const Arg& x, const Args&... xs);

template <typename Arg,
          typename... Args,
          typename = core::enable_for_argument_data_type_t<
            bool,
            core::common_argument_data_type_t<Arg, Args...>>>
ref<bool> AndE(const Arg& x, const Args&... xs);

template <typename Arg,
          typename... Args,
          typename = core::enable_for_argument_data_type_t<
            bool,
            core::common_argument_data_type_t<Arg, Args...>>>
ref<bool> OrE(const Arg& x, const Args&... xs);

// Logical operators

/// Logical negation (`not`)
///
ref<bool> operator!(const ref<bool>& x);

/// Logical `and` (lazy)
///
template <
  typename ArgX,
  typename ArgY,
  typename...,
  typename =
    core::enable_if_some_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>>
ref<bool> operator&&(const ArgX& x, const ArgY& y);

/// Logical `or` (lazy)
///
template <
  typename ArgX,
  typename ArgY,
  typename...,
  typename =
    core::enable_if_some_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>>
ref<bool> operator||(const ArgX& x, const ArgY& y);

/// Logical `and` (eager)
///
template <
  typename ArgX,
  typename ArgY,
  typename...,
  typename =
    core::enable_if_some_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>>
ref<bool> operator&(const ArgX& x, const ArgY& y);

/// Logical `or` (eager)
///
template <
  typename ArgX,
  typename ArgY,
  typename...,
  typename =
    core::enable_if_some_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>>
ref<bool> operator|(const ArgX& x, const ArgY& y);

/// \}
} // dataflow

#include "logical.inl"

#endif // DATAFLOW___LOGICAL_H
