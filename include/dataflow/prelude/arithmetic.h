
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

#ifndef DATAFLOW___ARITHMETIC_H
#define DATAFLOW___ARITHMETIC_H

#include "dataflow++_export.h"

#include "core.h"

namespace dataflow
{
/// \defgroup arithmetic
/// \{

// Arithmetic functions

template <typename ArgT,
          typename...,
          typename T = core::argument_data_type_t<ArgT>>
ref<T> Abs(const ArgT& x);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<T> Add(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<T> Sub(const ArgX& x, const ArgY& y);

template <typename ArgT,
          typename...,
          typename T = core::argument_data_type_t<ArgT>>
ref<T> Plus(const ArgT& x);

template <typename ArgT,
          typename...,
          typename T = core::argument_data_type_t<ArgT>>
ref<T> Inv(const ArgT& x);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<T> Mult(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<T> Div(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_for_argument_data_type_t<ArgY, T>>
ref<T> Mod(const ArgX& x, const ArgY& y);

template <typename ArgT,
          typename...,
          typename T = core::argument_data_type_t<ArgT>>
ref<T> Incr(const ArgT& x);

template <typename ArgT,
          typename...,
          typename T = core::argument_data_type_t<ArgT>>
ref<T> Decr(const ArgT& x);

// Arithmetic operators

// Addition
template <typename T> ref<T> operator+(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> operator+(const ref<T>& x, const T& y);
template <typename T> ref<T> operator+(const T& x, const ref<T>& y);
DATAFLOW___EXPORT ref<std::string> operator+(const ref<std::string>& x,
                                             const char* y);
DATAFLOW___EXPORT ref<std::string> operator+(const char* x,
                                             const ref<std::string>& y);

// Subtraction
template <typename T> ref<T> operator-(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> operator-(const ref<T>& x, const T& y);
template <typename T> ref<T> operator-(const T& x, const ref<T>& y);

// Unary plus
template <typename T> ref<T> operator+(const ref<T>& x);

// Unary minus (additive inverse)
template <typename T> ref<T> operator-(const ref<T>& x);

// Multiplication
template <typename T> ref<T> operator*(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> operator*(const ref<T>& x, const T& y);
template <typename T> ref<T> operator*(const T& x, const ref<T>& y);

// Division
template <typename T> ref<T> operator/(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> operator/(const ref<T>& x, const T& y);
template <typename T> ref<T> operator/(const T& x, const ref<T>& y);

// Modulo (integer remainder)
template <typename T> ref<T> operator%(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> operator%(const ref<T>& x, const T& y);
template <typename T> ref<T> operator%(const T& x, const ref<T>& y);

// Increment
template <typename T> ref<T> operator++(const ref<T>& x);
template <typename T> ref<T> operator++(const ref<T>& x, int);

// Decrement
template <typename T> ref<T> operator--(const ref<T>& x);
template <typename T> ref<T> operator--(const ref<T>& x, int);

/// \}
} // dataflow

#include "arithmetic.inl"

#endif // DATAFLOW___ARITHMETIC_H
