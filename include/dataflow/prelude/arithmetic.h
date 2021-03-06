
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

template <typename ArgX,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Abs(const ArgX& x);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() +
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Add(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() -
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Sub(const ArgX& x, const ArgY& y);

template <
  typename ArgX,
  typename...,
  typename X = core::argument_data_type_t<ArgX>,
  typename T = core::convert_to_flowable_t<decltype(+std::declval<X>())>,
  typename = core::enable_if_all_t<void,
                                   core::is_regular_data_type<X>,
                                   core::is_trivially_patcheable<X>,
                                   core::is_regular_data_type<T>,
                                   core::is_trivially_patcheable<T>>>
ref<T> Plus(const ArgX& x);

template <
  typename ArgX,
  typename...,
  typename X = core::argument_data_type_t<ArgX>,
  typename T = core::convert_to_flowable_t<decltype(-std::declval<X>())>,
  typename = core::enable_if_all_t<void,
                                   core::is_regular_data_type<X>,
                                   core::is_trivially_patcheable<X>,
                                   core::is_regular_data_type<T>,
                                   core::is_trivially_patcheable<T>>>
ref<T> Inv(const ArgX& x);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() *
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Mult(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() /
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Div(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename ArgY,
          typename...,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() %
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Mod(const ArgX& x, const ArgY& y);

template <typename ArgX,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Incr(const ArgX& x);

template <typename ArgX,
          typename...,
          typename T = core::argument_data_type_t<ArgX>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> Decr(const ArgX& x);

// Arithmetic operators

/// Addition
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() +
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator+(const ArgX& x, const ArgY& y);

/// Subtraction
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() -
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator-(const ArgX& x, const ArgY& y);

/// Unary plus
///
template <
  typename X,
  typename...,
  typename T = core::convert_to_flowable_t<decltype(+std::declval<X>())>,
  typename = core::enable_if_all_t<void,
                                   core::is_regular_data_type<X>,
                                   core::is_trivially_patcheable<X>,
                                   core::is_regular_data_type<T>,
                                   core::is_trivially_patcheable<T>>>
ref<T> operator+(const ref<X>& x);

/// Unary minus (additive inverse)
///
template <
  typename X,
  typename...,
  typename T = core::convert_to_flowable_t<decltype(-std::declval<X>())>,
  typename = core::enable_if_all_t<void,
                                   core::is_regular_data_type<X>,
                                   core::is_trivially_patcheable<X>,
                                   core::is_regular_data_type<T>,
                                   core::is_trivially_patcheable<T>>>
ref<T> operator-(const ref<X>& x);

/// Multiplication
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() *
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator*(const ArgX& x, const ArgY& y);

/// Division
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() /
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator/(const ArgX& x, const ArgY& y);

/// Modulo (integer remainder)
///
template <typename ArgX,
          typename ArgY,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgX>, core::is_ref<ArgY>>,
          typename X = core::argument_data_type_t<ArgX>,
          typename Y = core::argument_data_type_t<ArgY>,
          typename T = core::convert_to_flowable_t<decltype(std::declval<X>() %
                                                            std::declval<Y>())>,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<X>,
                                           core::is_trivially_patcheable<X>,
                                           core::is_regular_data_type<Y>,
                                           core::is_trivially_patcheable<Y>,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator%(const ArgX& x, const ArgY& y);

/// Increment
///
template <typename T,
          typename...,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator++(const ref<T>& x);

template <typename T,
          typename...,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator++(const ref<T>& x, int);

/// Decrement
///
template <typename T,
          typename...,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator--(const ref<T>& x);

template <typename T,
          typename...,
          typename = core::enable_if_all_t<void,
                                           core::is_regular_data_type<T>,
                                           core::is_trivially_patcheable<T>>>
ref<T> operator--(const ref<T>& x, int);

/// \}
} // dataflow

#include "arithmetic.inl"

#endif // DATAFLOW___ARITHMETIC_H
