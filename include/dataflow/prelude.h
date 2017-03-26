
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

#ifndef DATAFLOW___PRELUDE_H
#define DATAFLOW___PRELUDE_H

#include "dataflow++_export.h"

#include "internal/ref.h"

#include <string>
#include <type_traits>

namespace dataflow
{

/// \defgroup prelude
/// \{

namespace detail
{
template <typename T, typename U = void>
using enable_if_not_ref_t =
  typename std::enable_if<!std::is_base_of<internal::ref, T>::value>::type;
}

class DATAFLOW___EXPORT Engine
{
public:
  Engine();
  ~Engine();
};

template <typename T> class ref : public internal::ref
{
public:
  explicit ref(const internal::ref& r);
};

template <typename T> class eager final : public ref<T>
{
public:
  explicit eager(const internal::ref& r);

  const T& operator()() const;
};

template <typename T> class var final : public ref<T>
{
public:
  explicit var(const internal::ref& r);

  const var& operator=(const T& v) const;
};

using Time = internal::tick_count;

namespace detail
{
template <typename> struct value_type
{
};

template <typename T> struct value_type<ref<T>>
{
  using type = T;
};

template <typename T> struct value_type<var<T>>
{
  using type = T;
};

template <typename T> struct value_type<eager<T>>
{
  using type = T;
};

template <typename F> using value_type_t = typename value_type<F>::type;

template <typename F>
using time_func_result_t =
  value_type_t<typename std::result_of<F(const Time&)>::type>;
}

// Basic functions

template <typename T> ref<T> Const(const T& v = T());
DATAFLOW___EXPORT ref<std::string> Const(const char* v);

template <typename T> var<T> Var(const T& v);

template <typename T> eager<T> Curr(ref<T> x);

template <typename F, typename T = detail::time_func_result_t<F>>
eager<T> Main(F f);

// Operators

template <typename T> eager<T> operator*(ref<T> x);

// Utility functions

template <
  typename Policy,
  typename X,
  typename T = typename std::remove_cv<typename std::remove_reference<
    decltype(std::declval<Policy>().calculate(std::declval<X>()))>::type>::type>
ref<T>
Lift(const ref<X>& x, const Policy& policy = Policy(), bool eager = false);

template <typename F,
          typename X,
          typename T = typename std::result_of<F(const X&)>::type>
ref<T> Lift(const std::string& label, const ref<X>& x, F func);

template <typename Policy,
          typename X,
          typename Y,
          typename T = typename std::remove_cv<typename std::remove_reference<
            decltype(std::declval<Policy>().calculate(
              std::declval<X>(), std::declval<Y>()))>::type>::type>
ref<T> Lift(const ref<X>& x, const ref<Y>& y, const Policy& policy = Policy());

template <typename F,
          typename X,
          typename Y,
          typename T = typename std::result_of<F(const X&, const Y&)>::type>
ref<T> Lift(const std::string& label, const ref<X>& x, const ref<Y>& y, F func);

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

// Operators

// Addition
template <typename T> ref<T> operator+(const ref<T>& x, const ref<T>& y);
template <typename T> ref<T> operator+(const ref<T>& x, const T& y);
template <typename T> ref<T> operator+(const T& x, const ref<T>& y);
ref<std::string> operator+(const ref<std::string>& x, const char* y);
ref<std::string> operator+(const char* x, const ref<std::string>& y);

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

// Comparison functions

template <typename T> ref<bool> Eq(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> NotEq(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> Gr(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> Less(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> GrEq(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> LessEq(const ref<T>& x, const ref<T>& y);

// Operators

// Equal to
template <typename T> ref<bool> operator==(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator==(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator==(const T& x, const ref<T>& y);
ref<bool> operator==(const ref<std::string>& x, const char* y);
ref<bool> operator==(const char* x, const ref<std::string>& y);

// Not equal to
template <typename T> ref<bool> operator!=(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator!=(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator!=(const T& x, const ref<T>& y);
ref<bool> operator!=(const ref<std::string>& x, const char* y);
ref<bool> operator!=(const char* x, const ref<std::string>& y);

// Greater than
template <typename T> ref<bool> operator>(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator>(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator>(const T& x, const ref<T>& y);
ref<bool> operator>(const ref<std::string>& x, const char* y);
ref<bool> operator>(const char* x, const ref<std::string>& y);

// Less than
template <typename T> ref<bool> operator<(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator<(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator<(const T& x, const ref<T>& y);
ref<bool> operator<(const ref<std::string>& x, const char* y);
ref<bool> operator<(const char* x, const ref<std::string>& y);

// Greater than or equal to
template <typename T> ref<bool> operator>=(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator>=(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator>=(const T& x, const ref<T>& y);
ref<bool> operator>=(const ref<std::string>& x, const char* y);
ref<bool> operator>=(const char* x, const ref<std::string>& y);

// Less than or equal to
template <typename T> ref<bool> operator<=(const ref<T>& x, const ref<T>& y);
template <typename T> ref<bool> operator<=(const ref<T>& x, const T& y);
template <typename T> ref<bool> operator<=(const T& x, const ref<T>& y);
ref<bool> operator<=(const ref<std::string>& x, const char* y);
ref<bool> operator<=(const char* x, const ref<std::string>& y);

// Logical functions

ref<bool> Not(const ref<bool>& x);
ref<bool> And(const ref<bool>& x, const ref<bool>& y);
ref<bool> Or(const ref<bool>& x, const ref<bool>& y);
ref<bool> AndE(const ref<bool>& x, const ref<bool>& y);
ref<bool> OrE(const ref<bool>& x, const ref<bool>& y);

// Operators

// Logical negation (`not`)
ref<bool> operator!(const ref<bool>& x);

// Logical `and` (lazy)
ref<bool> operator&&(const ref<bool>& x, const ref<bool>& y);
ref<bool> operator&&(const ref<bool>& x, const bool& y);
ref<bool> operator&&(const bool& x, const ref<bool>& y);

// Logical `or` (lazy)
ref<bool> operator||(const ref<bool>& x, const ref<bool>& y);
ref<bool> operator||(const ref<bool>& x, const bool& y);
ref<bool> operator||(const bool& x, const ref<bool>& y);

// Logical `and` (eager)
ref<bool> operator&(const ref<bool>& x, const ref<bool>& y);
ref<bool> operator&(const ref<bool>& x, const bool& y);
ref<bool> operator&(const bool& x, const ref<bool>& y);

// Logical `or` (eager)
ref<bool> operator|(const ref<bool>& x, const ref<bool>& y);
ref<bool> operator|(const ref<bool>& x, const bool& y);
ref<bool> operator|(const bool& x, const ref<bool>& y);

// Conditional expressions

template <typename T>
ref<T> If(const ref<bool>& x, const ref<T>& y, const ref<T>& z);
template <typename T>
ref<T> If(const ref<bool>& x, const T& y, const ref<T>& z);
template <typename T>
ref<T> If(const ref<bool>& x, const ref<T>& y, const T& z);
template <typename T, typename = detail::enable_if_not_ref_t<T>>
ref<T> If(const ref<bool>& x, const T& y, const T& z);

template <typename T>
ref<T> Switch(const std::pair<ref<bool>, ref<T>>& first_case,
              const ref<T>& default_case);

template <typename T, typename... Cases>
ref<T> Switch(const std::pair<ref<bool>, ref<T>>& first_case,
              const Cases&... other_cases);

template <typename T>
std::pair<ref<bool>, ref<T>> Case(const ref<bool>& x, const ref<T>& y);

template <typename T>
std::pair<ref<bool>, ref<T>> Case(const ref<bool>& x, const T& v);

std::pair<ref<bool>, ref<std::string>> Case(const ref<bool>& x, const char* v);

template <typename T> ref<T> Default(const ref<T>& x);
template <typename T> ref<T> Default(const T& v);
ref<std::string> Default(const char* v);

// String functions

template <typename X> ref<std::string> ToString(const ref<X>& x);

template <> ref<std::string> ToString(const ref<std::string>& x);

template <typename T, typename = detail::enable_if_not_ref_t<T>>
ref<std::string> ToString(const T& v);

template <typename A, typename B, typename... Args>
ref<std::string> ToString(const A& a, const B& b, const Args&... args);

template <typename T>
dataflow::ref<T> FromString(const ref<std::string>& str, const T& d = T());

template <>
ref<std::string> FromString(const ref<std::string>& x, const std::string&);

template <typename T>
dataflow::ref<T> FromString(const char* str, const T& d = T());

/// \}
}

#include "prelude.inl"

#endif // DATAFLOW___PRELUDE_H
