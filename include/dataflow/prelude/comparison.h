
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
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<bool> Eq(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<bool> NotEq(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<bool> Gr(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<bool> Less(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<bool> GrEq(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<bool> LessEq(const ArgX& x, const ArgY& y);

// Comparison operators

// Equal to
template <typename T> ref<bool> operator==(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator==(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator==(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<bool> operator==(const ref<std::string>& x,
                                       const char* y);
DATAFLOW___EXPORT ref<bool> operator==(const char* x,
                                       const ref<std::string>& y);

// Not equal to
template <typename T> ref<bool> operator!=(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator!=(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator!=(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<bool> operator!=(const ref<std::string>& x,
                                       const char* y);
DATAFLOW___EXPORT ref<bool> operator!=(const char* x,
                                       const ref<std::string>& y);

// Greater than
template <typename T> ref<bool> operator>(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator>(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator>(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<bool> operator>(const ref<std::string>& x, const char* y);
DATAFLOW___EXPORT ref<bool> operator>(const char* x, const ref<std::string>& y);

// Less than
template <typename T> ref<bool> operator<(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator<(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator<(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<bool> operator<(const ref<std::string>& x, const char* y);
DATAFLOW___EXPORT ref<bool> operator<(const char* x, const ref<std::string>& y);

// Greater than or equal to
template <typename T> ref<bool> operator>=(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator>=(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator>=(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<bool> operator>=(const ref<std::string>& x,
                                       const char* y);
DATAFLOW___EXPORT ref<bool> operator>=(const char* x,
                                       const ref<std::string>& y);

// Less than or equal to
template <typename T> ref<bool> operator<=(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator<=(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator<=(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<bool> operator<=(const ref<std::string>& x,
                                       const char* y);
DATAFLOW___EXPORT ref<bool> operator<=(const char* x,
                                       const ref<std::string>& y);

/// \}
} // dataflow

#include "comparison.inl"

#endif // DATAFLOW___COMPARISON_H
