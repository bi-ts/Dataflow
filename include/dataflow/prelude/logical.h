
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

DATAFLOW___EXPORT ref<bool> Not(const ref<bool>& x);

template <typename Arg,
          typename... Args,
          typename = core::enable_for_argument_data_type_t<
            bool,
            core::common_argument_data_type_t<Arg, Args...>>>
ref<bool> And(const Arg& arg, const Args&... args);

DATAFLOW___EXPORT ref<bool> Or(const ref<bool>& x, const ref<bool>& y);
DATAFLOW___EXPORT ref<bool> AndE(const ref<bool>& x, const ref<bool>& y);
DATAFLOW___EXPORT ref<bool> OrE(const ref<bool>& x, const ref<bool>& y);

// Logical operators

/// Logical negation (`not`)
///
ref<bool> operator!(const ref<bool>& x);

/// Logical `and` (lazy)
///
ref<bool> operator&&(const ref<bool>& x, const ref<bool>& y);

/// Logical `or` (lazy)
///
ref<bool> operator||(const ref<bool>& x, const ref<bool>& y);

/// Logical `and` (eager)
///
ref<bool> operator&(const ref<bool>& x, const ref<bool>& y);

/// Logical `or` (eager)
///
ref<bool> operator|(const ref<bool>& x, const ref<bool>& y);

/// \}
} // dataflow

#include "logical.inl"

#endif // DATAFLOW___LOGICAL_H
