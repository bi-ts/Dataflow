
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

#ifndef DATAFLOW___CORE_H
#define DATAFLOW___CORE_H

#include "dataflow++_export.h"

#include "../internal/ref.h"
#include "../internal/std_future.h"

#include <functional>
#include <string>
#include <type_traits>

namespace dataflow
{

namespace core
{
template <typename T>
struct is_flowable
: internal::std17::conjunction<
    std::is_default_constructible<T>,
    std::is_copy_constructible<T>,
    std::is_copy_assignable<T>,
    internal::is_streamable<T>,
    internal::is_equality_comparable<T>,
    internal::std17::negation<std::is_base_of<internal::ref, T>>,
    internal::std17::negation<std::is_pointer<T>>,
    internal::std17::negation<std::is_reference<T>>>
{
};
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

namespace internal
{
template <typename Ref> Ref make_ref(const ref& x);
}

template <typename T> class ref : public internal::ref
{
  static_assert(core::is_flowable<T>::value, "`T` must be flowable");

  template <typename Ref> friend Ref internal::make_ref(const internal::ref&);

public:
  ref<T> operator()(const Time& t) const;

protected:
  explicit ref(const internal::ref& r);
};

class DATAFLOW___EXPORT sig final : public ref<bool>
{
  template <typename Ref> friend Ref internal::make_ref(const internal::ref&);

public:
  void emit() const;

private:
  explicit sig(const internal::ref& r);
};

template <typename T> class val final : public ref<T>
{
  template <typename Ref> friend Ref internal::make_ref(const internal::ref&);

public:
  const T& operator()() const;

private:
  explicit val(const internal::ref& r);
};

template <typename T> class var final : public ref<T>
{
  template <typename Ref> friend Ref internal::make_ref(const internal::ref&);

public:
  const var& operator=(const T& v) const;

private:
  explicit var(const internal::ref& r);
};

template <typename T>
using function_of_time = std::function<ref<T>(const Time&)>;

namespace core
{
template <typename T> class generic_patch
{
public:
  using data_type = T;

public:
  explicit generic_patch(const T& curr, const T& prev);

  T apply(const T&) const;

private:
  T curr_;
};

template <typename T, typename Patch = generic_patch<T>> class diff final
{
public:
  using data_type = T;
  using patch_type = Patch;

  static_assert(std::is_same<typename Patch::data_type, T>::value,
                "Wrong patch data type");

public:
  explicit diff(const T& curr, const T& prev, const Patch& patch);

  explicit diff(const T& curr, const T& prev);

  const T& curr() const;
  const T& prev() const;
  const Patch& patch() const;

private:
  T curr_;
  T prev_;
  Patch patch_;
};

// Type traits

template <typename T, typename U = T>
using enable_if_flowable = std::enable_if<is_flowable<T>::value, U>;

template <typename T, typename U = T>
using enable_if_flowable_t = typename enable_if_flowable<T, U>::type;

template <typename T>
struct convert_to_flowable
: enable_if_flowable<typename std::conditional<
    is_flowable<internal::std20::remove_cvref_t<T>>::value,
    internal::std20::remove_cvref_t<T>,
    typename std::conditional<std::is_convertible<T, std::string>::value,
                              std::string,
                              void>::type>::type>
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

namespace detail
{
template <typename T> struct function_of_time_type
{
private:
  template <typename F,
            typename U = data_type_t<
              decltype(std::declval<F>()(std::declval<const Time&>()))>>
  static typename std::enable_if<!is_ref<F>::value, U>::type test_(const F*);
  static void test_(...);

public:
  using type = decltype(test_(std::declval<const T*>()));
};
}

template <typename T>
struct is_function_of_time
: is_flowable<typename detail::function_of_time_type<T>::type>
{
};

template <typename F>
struct function_of_time_type
: std::enable_if<is_function_of_time<F>::value,
                 typename detail::function_of_time_type<F>::type>
{
};

template <typename F>
using function_of_time_type_t = typename function_of_time_type<F>::type;

namespace detail
{
template <typename F, typename T> struct type_1_transition_function_result
{
private:
  template <
    typename FF,
    typename TT,
    typename FwTT = convert_to_flowable_t<TT>,
    typename = typename std::enable_if<std::is_same<
      data_type_t<decltype(std::declval<FF>()(std::declval<ref<FwTT>>()))>,
      T>::value>::type>
  static T test_(const FF*, const TT*);

  static void test_(...);

public:
  using type =
    decltype(test_(std::declval<const F*>(), std::declval<const T*>()));
};

template <typename F, typename T> struct type_2_transition_function_result
{
private:
  template <typename FF,
            typename TT,
            typename FwTT = convert_to_flowable_t<TT>,
            typename = std::enable_if<
              std::is_same<function_of_time_type_t<decltype(
                             std::declval<FF>()(std::declval<ref<FwTT>>()))>,
                           T>::value>>
  static T test_(const FF*, const TT*);

  static void test_(...);

public:
  using type =
    decltype(test_(std::declval<const F*>(), std::declval<const T*>()));
};
}

template <typename F, typename T>
using is_transition_function = internal::std17::disjunction<
  is_flowable<typename detail::type_1_transition_function_result<F, T>::type>,
  is_flowable<typename detail::type_2_transition_function_result<F, T>::type>>;

template <typename F, typename T, typename U = T>
using enable_if_transition_function =
  std::enable_if<is_transition_function<F, T>::value, U>;

template <typename F, typename T, typename U = T>
using enable_if_transition_function_t =
  typename enable_if_transition_function<F, T, U>::type;

template <typename T>
using argument_data_type = typename std::
  conditional<is_ref<T>::value, data_type<T>, convert_to_flowable<T>>::type;

template <typename T>
using argument_data_type_t = typename argument_data_type<T>::type;

template <typename T,
          typename U,
          typename V = typename argument_data_type<T>::type>
using enable_for_argument_data_type =
  std::enable_if<std::is_same<typename argument_data_type<T>::type, U>::value,
                 V>;

template <typename T,
          typename U,
          typename V = typename argument_data_type<T>::type>
using enable_for_argument_data_type_t =
  typename enable_for_argument_data_type<T, U, V>::type;

template <typename T>
using farg_data_type = typename std::conditional<is_function_of_time<T>::value,
                                                 function_of_time_type<T>,
                                                 argument_data_type<T>>::type;

template <typename T> using farg_data_type_t = typename farg_data_type<T>::type;

template <typename Arg,
          typename T,
          typename U = typename farg_data_type<Arg>::type>
using enable_for_farg_data_type =
  std::enable_if<std::is_same<typename farg_data_type<Arg>::type, T>::value, U>;

template <typename Arg,
          typename T,
          typename U = typename farg_data_type<Arg>::type>
using enable_for_farg_data_type_t =
  typename enable_for_farg_data_type<Arg, T, U>::type;

template <typename T, typename... FArgs>
using farg_result = std::conditional<
  internal::std17::disjunction<core::is_function_of_time<FArgs>...>::value,
  function_of_time<T>,
  ref<T>>;

template <typename T, typename... FArgs>
using farg_result_t = typename farg_result<T, FArgs...>::type;

template <typename T> struct patch_type
{
private:
  template <typename TT, typename Patch = typename TT::patch_type>
  static Patch test_(const TT&);

  static generic_patch<T> test_(...);

public:
  using type = decltype(test_(std::declval<T>()));
};

template <typename T> using patch_type_t = typename patch_type<T>::type;

template <typename T> struct diff_type
{
  using type = diff<T, patch_type_t<T>>;
};

template <typename T> using diff_type_t = typename diff_type<T>::type;

template <typename T, typename... Args>
using enable_if_all =
  std::enable_if<internal::std17::conjunction<Args...>::value, T>;

template <typename T, typename... Args>
using enable_if_all_t = typename enable_if_all<T, Args...>::type;

template <typename T, typename... Args>
using enable_if_some =
  std::enable_if<internal::std17::disjunction<Args...>::value, T>;

template <typename T = void, typename... Args>
using enable_if_some_t = typename enable_if_some<T, Args...>::type;

template <typename T, typename... Args>
using enable_if_none = std::enable_if<
  internal::std17::conjunction<internal::std17::negation<Args>...>::value,
  T>;

template <typename T = void, typename... Args>
using enable_if_none_t = typename enable_if_none<T, Args...>::type;

// Utility functions

template <typename T, typename FwT = convert_to_flowable_t<T>>
ref<FwT> make_argument(const T& v);

template <typename T> ref<T> make_argument(const ref<T>& x);

template <typename F>
function_of_time<function_of_time_type_t<F>> make_farg(const F& f);

template <typename T> ref<argument_data_type_t<T>> make_farg(const T& x);

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
          typename... Xs,
          typename T = data_type_t<decltype(std::declval<Policy>().calculate(
            std::declval<X>(), std::declval<Xs>()...))>>
ref<T>
LiftSelector(const Policy& policy, const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = data_type_t<decltype(std::declval<Policy>().calculate(
            std::declval<X>(), std::declval<Xs>()...))>>
ref<T> LiftSelector(const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = decltype(std::declval<Policy>().calculate(
            std::declval<X>(), std::declval<Xs>()...))>
ref<T> LiftPatcher(const Policy& policy, const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename X,
          typename... Xs,
          typename T = decltype(std::declval<Policy>().calculate(
            std::declval<X>(), std::declval<Xs>()...))>
ref<T> LiftPatcher(const ref<X>& x, const ref<Xs>&... xs);
}

// Basic functions

DATAFLOW___EXPORT ref<std::size_t> CurrentTime();

template <typename T, typename FwT = core::convert_to_flowable_t<T>>
ref<FwT> Const(const T& v);

template <typename T,
          typename... Args,
          typename = core::enable_if_flowable_t<T>>
ref<T> Const(Args&&... args);

DATAFLOW___EXPORT sig Signal();

template <typename T, typename FwT = core::convert_to_flowable_t<T>>
var<FwT> Var(const T& v);

template <typename T,
          typename... Args,
          typename = core::enable_if_flowable_t<T>>
var<T> Var(Args&&... args);

template <typename T> val<T> Curr(ref<T> x);

template <typename F, typename T = core::function_of_time_type_t<F>>
val<T> Main(F f);

// Operators

template <typename T> val<T> operator*(ref<T> x);

// Conditional functions

template <typename T,
          typename U,
          typename FwT = core::argument_data_type_t<T>,
          typename = core::enable_for_argument_data_type_t<U, FwT>>
ref<FwT> If(const ref<bool>& x, const T& y, const U& z);

template <typename FArgT,
          typename FArgU,
          typename T = core::farg_data_type_t<FArgT>,
          typename = core::enable_for_farg_data_type_t<FArgU, T>,
          typename = core::enable_if_some_t<void,
                                            core::is_function_of_time<FArgT>,
                                            core::is_function_of_time<FArgU>>>
ref<T> If(const ref<bool>& x, const FArgT& y, const FArgU& z, const Time& t0);

template <typename FArgT,
          typename FArgU,
          typename T = core::farg_data_type_t<FArgT>,
          typename = core::enable_for_farg_data_type_t<FArgU, T>,
          typename = core::enable_if_some_t<void,
                                            core::is_function_of_time<FArgT>,
                                            core::is_function_of_time<FArgU>>>
function_of_time<T> If(const ref<bool>& x, const FArgT& y, const FArgU& z);

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
