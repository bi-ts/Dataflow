
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

#include "../internal/ref.h"

#include <string>
#include <type_traits>

namespace dataflow
{

namespace core
{
template <typename T> struct is_flowable;
}

/// \defgroup core
/// \ingroup prelude
/// \{

using Time = internal::tick_count;

class DATAFLOW___EXPORT Engine
{
public:
  Engine();
  ~Engine();
};

template <typename T> class ref : public internal::ref
{
  static_assert(core::is_flowable<T>::value, "`T` must be flowable");

public:
  explicit ref(const internal::ref& r);

  ref<T> operator()(const Time& t) const;
};

template <typename T> class val final : public ref<T>
{
public:
  explicit val(const internal::ref& r);

  const T& operator()() const;
};

template <typename T> class var final : public ref<T>
{
public:
  explicit var(const internal::ref& r);

  const var& operator=(const T& v) const;
};

namespace core
{

// Type traits

template <typename T>
struct is_flowable
  : std::integral_constant<bool,
                           std::is_default_constructible<T>::value &&
                             std::is_copy_constructible<T>::value &&
                             std::is_copy_assignable<T>::value &&
                             internal::is_streamable<T>::value &&
                             internal::is_equality_comparable<T>::value &&
                             !std::is_base_of<internal::ref, T>::value &&
                             !std::is_pointer<T>::value &&
                             !std::is_reference<T>::value>
{
};

template <typename T, typename U = T>
using enable_if_flowable = std::enable_if<is_flowable<T>::value, U>;

template <typename T, typename U = T>
using enable_if_flowable_t = typename enable_if_flowable<T, U>::type;

namespace detail
{
template <typename T> struct convert_to_flowable
{
private:
  template <typename U, typename = enable_if_flowable_t<U>>
  static U test_(const U&);

  static std::string test_(const char*);

  static void test_(...);

public:
  using type = decltype(test_(std::declval<T>()));
};
}

template <typename T>
struct convert_to_flowable
  : enable_if_flowable<typename detail::convert_to_flowable<T>::type>
{
};

template <typename T>
using convert_to_flowable_t = typename convert_to_flowable<T>::type;

namespace detail
{
template <typename T> struct data_type
{
private:
  template <typename U> static U test_(const ref<U>*);
  static void test_(...);

public:
  using type =
    decltype(test_(std::declval<typename std::remove_reference<T>::type*>()));
};
}

template <typename T>
struct is_ref : is_flowable<typename detail::data_type<T>::type>
{
};

template <typename T>
struct data_type
  : std::enable_if<is_ref<T>::value, typename detail::data_type<T>::type>
{
};

template <typename T> using data_type_t = typename data_type<T>::type;

template <typename F>
using function_of_time_type =
  data_type<typename std::result_of<F(const Time&)>::type>;

template <typename F>
using function_of_time_type_t = typename function_of_time_type<F>::type;

namespace detail
{
template <typename F, typename T> struct transition_function_result
{
private:
  template <typename FF,
            typename = typename std::enable_if<std::is_same<
              data_type_t<typename std::result_of<FF(const ref<T>&)>::type>,
              T>::value>::type>
  static T test_(const FF&);

  static void test_(...);

public:
  using type = decltype(test_(std::declval<F>()));
};
}

template <typename F, typename T>
using is_transition_function =
  is_flowable<typename detail::transition_function_result<F, T>::type>;

template <typename F, typename T, typename U = T>
using enable_if_transition_function =
  std::enable_if<is_transition_function<F, T>::value, U>;

template <typename F, typename T, typename U = T>
using enable_if_transition_function_t =
  typename enable_if_transition_function<F, T, U>::type;

template <typename T>
using argument_data_type =
  typename std::conditional<is_ref<T>::value,
                            data_type<T>,
                            convert_to_flowable<T>>::type;

template <typename T>
using argument_data_type_t = typename argument_data_type<T>::type;

// Utility functions

template <typename T, typename FwT = convert_to_flowable_t<T>>
ref<FwT> make_argument(const T& v);

template <typename T> ref<T> make_argument(const ref<T>& x);

template <typename F,
          typename X,
          typename T = typename std::result_of<F(const X&)>::type>
ref<T> Lift(const std::string& label, const ref<X>& x, F func);

template <typename F,
          typename X,
          typename Y,
          typename T = typename std::result_of<F(const X&, const Y&)>::type>
ref<T> Lift(const std::string& label, const ref<X>& x, const ref<Y>& y, F func);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = typename std::remove_cv<typename std::remove_reference<
            decltype(std::declval<Policy>().calculate(
              std::declval<X>(), std::declval<Xs>()...))>::type>::type>
ref<T> Lift(const Policy& policy, const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = typename std::remove_cv<typename std::remove_reference<
            decltype(std::declval<Policy>().calculate(
              std::declval<X>(), std::declval<Xs>()...))>::type>::type>
ref<T> Lift(const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = typename std::remove_cv<typename std::remove_reference<
            decltype(std::declval<Policy>().calculate(
              std::declval<X>(), std::declval<Xs>()...))>::type>::type>
ref<T> LiftPuller(const Policy& policy, const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = typename std::remove_cv<typename std::remove_reference<
            decltype(std::declval<Policy>().calculate(
              std::declval<X>(), std::declval<Xs>()...))>::type>::type>
ref<T> LiftPuller(const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename T = core::data_type_t<
            decltype(std::declval<Policy>().calculate(std::declval<X>()))>>
ref<T> LiftSelector(const ref<X>& x,
                    const Policy& policy = Policy(),
                    bool eager = false);
}

// Basic functions

DATAFLOW___EXPORT ref<std::size_t> CurrentTime();

template <typename T, typename = core::enable_if_flowable_t<T>>
ref<T> Const(const T& v = T());
DATAFLOW___EXPORT ref<std::string> Const(const char* v);

template <typename T, typename = core::enable_if_flowable_t<T>>
var<T> Var(const T& v = T());
DATAFLOW___EXPORT var<std::string> Var(const char* v);

template <typename T> val<T> Curr(ref<T> x);

template <typename F, typename T = core::function_of_time_type_t<F>>
val<T> Main(F f);

// Operators

template <typename T> val<T> operator*(ref<T> x);

// Conditional functions

template <typename T>
ref<T> If(const ref<bool>& x, const ref<T>& y, const ref<T>& z);
template <typename T>
ref<T> If(const ref<bool>& x, const T& y, const ref<T>& z);
template <typename T>
ref<T> If(const ref<bool>& x, const ref<T>& y, const T& z);
template <typename T, typename = core::convert_to_flowable_t<T>>
ref<T> If(const ref<bool>& x, const T& y, const T& z);

// Stateful functions

template <typename T>
ref<T> Prev(const ref<T>& v0, const ref<T>& x, const Time& t0);

template <
  typename Arg,
  typename F,
  typename T =
    core::enable_if_transition_function_t<F, core::argument_data_type_t<Arg>>>
ref<T> StateMachine(const Arg& s0, F tf, const Time& t0);
/// \}
}

#include "core.inl"

#endif // DATAFLOW___CORE_H
