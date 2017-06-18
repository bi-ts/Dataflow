
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

#ifndef DATAFLOW___CORE_H
#define DATAFLOW___CORE_H

#include "dataflow++_export.h"

#include "internal/ref.h"

#include <string>
#include <type_traits>

namespace dataflow
{

/// \defgroup core
/// \ingroup prelude
/// \{

namespace detail
{
template <typename T, typename U = void>
using enable_if_not_ref_t =
  typename std::enable_if<!std::is_base_of<internal::ref, T>::value>::type;
}

using Time = internal::tick_count;

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

  ref<T> operator()(const Time& t) const;
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

template <typename T> var<T> Var(const T& v = T());
DATAFLOW___EXPORT var<std::string> Var(const char* v);

template <typename T> eager<T> Curr(ref<T> x);

template <typename F, typename T = detail::time_func_result_t<F>>
eager<T> Main(F f);

// Operators

template <typename T> eager<T> operator*(ref<T> x);

// Conditional functions

template <typename T>
ref<T> If(const ref<bool>& x, const ref<T>& y, const ref<T>& z);
template <typename T>
ref<T> If(const ref<bool>& x, const T& y, const ref<T>& z);
template <typename T>
ref<T> If(const ref<bool>& x, const ref<T>& y, const T& z);
template <typename T, typename = detail::enable_if_not_ref_t<T>>
ref<T> If(const ref<bool>& x, const T& y, const T& z);

// Conditional functions

template <typename T>
ref<T> Prev(const Time& t0, const ref<T>& v0, const ref<T>& x);

// Utility functions

namespace core
{
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
}

/// \}
}

#include "core.inl"

#endif // DATAFLOW___CORE_H
