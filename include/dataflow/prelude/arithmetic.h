
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

#ifndef DATAFLOW___ARITHMETIC_H
#define DATAFLOW___ARITHMETIC_H

#include "dataflow++_export.h"

#include "core.h"

namespace dataflow
{
/// \defgroup arithmetic
/// \{

// Arithmetic functions

template <typename T> ref<T> Abs(const ref<T>& x);
template <typename T> ref<T> Add(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> Sub(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> Plus(const ref<T>& x);
template <typename T> ref<T> Inv(const ref<T>& x);
template <typename T> ref<T> Mult(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> Div(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> Mod(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> Incr(const ref<T>& x);
template <typename T> ref<T> Decr(const ref<T>& x);

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
