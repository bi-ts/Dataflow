
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

#ifndef DATAFLOW___COMPARISON_H
#define DATAFLOW___COMPARISON_H

#include "dataflow++_export.h"

#include "core.h"

namespace dataflow
{
/// \defgroup comparison
/// \{

// Comparison functions

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> Eq(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> NotEq(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> Gr(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> Less(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> GrEq(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> LessEq(const ArgX& x, const ArgY& y);

// Comparison operators

/// Equal to
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> operator==(const ArgX& x, const ArgY& y);

/// Not equal to
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> operator!=(const ArgX& x, const ArgY& y);

/// Greater than
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> operator>(const ArgX& x, const ArgY& y);

/// Less than
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> operator<(const ArgX& x, const ArgY& y);

/// Greater than or equal to
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> operator>=(const ArgX& x, const ArgY& y);

/// Less than or equal to
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<bool> operator<=(const ArgX& x, const ArgY& y);

/// \}
} // dataflow

#include "comparison.inl"

#endif // DATAFLOW___COMPARISON_H
