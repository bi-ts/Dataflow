
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
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

#include "core/dtime.h"
#include "core/engine_options.h"

#include "core/internal/ref.h"

#include <dataflow/utility/std_future.h>

#include <functional>
#include <string>
#include <type_traits>

#ifdef DATAFLOW_CONFIG_VAR_NO_CONST_COPY_CTOR
#define DATAFLOW_VAR_CONST
#else
#define DATAFLOW_VAR_CONST const
#endif

namespace dataflow
{
/// \defgroup core
/// \ingroup prelude
/// \{

using integer = int;

namespace core
{
template <typename T>
struct is_flowable
: std17::conjunction<std::is_default_constructible<T>,
                     std::is_copy_constructible<T>,
                     std::is_copy_assignable<T>,
                     internal::is_streamable<T>,
                     internal::is_equality_comparable<T>,
                     std17::negation<internal::is_callable<T>>,
                     std17::negation<std::is_base_of<internal::ref, T>>,
                     std17::negation<std::is_pointer<T>>,
                     std17::negation<std::is_reference<T>>>
{
};

template <typename T> class ref_base : public internal::ref
{
  static_assert(core::is_flowable<T>::value, "`T` must be flowable");

public:
  explicit ref_base(const internal::ref& r, internal::ref::ctor_guard_t);

protected:
  ref_base<T> snapshot_(dtime t) const;

  void reset_(const ref_base<T>& other);
};
}

class DATAFLOW___EXPORT unit
{
public:
  bool operator==(const unit&) const;
  bool operator!=(const unit&) const;
};

DATAFLOW___EXPORT std::ostream& operator<<(std::ostream& out, const unit& v);

template <typename T> class ref : public core::ref_base<T>
{
public:
  ref(core::ref_base<T> base)
  : core::ref_base<T>(base)
  {
  }

  ref<T> operator()(dtime t) const
  {
    return this->snapshot_(t);
  }
};

template <typename T> class arg : public ref<T>
{
public:
  arg(const ref<T>& x);

  template <
    typename U,
    typename...,
    typename = typename std::enable_if<std::is_convertible<U, T>::value>::type>
  arg(U&& value);
};

class DATAFLOW___EXPORT sig final : public ref<bool>
{
public:
  explicit sig(const internal::ref& r, internal::ref::ctor_guard_t);

  const ref<bool>& as_ref() const;

  void operator()() const;
};

template <typename T> class val final : public ref<T>
{
public:
  explicit val(const internal::ref& r, internal::ref::ctor_guard_t);

  const T& operator*() const;
};

class DATAFLOW___EXPORT Engine
{
  friend ref<bool> Timeout(const arg<integer>& interval_msec, dtime t0);

public:
  Engine(engine_options options = engine_options::fully_optimized);
  virtual ~Engine();

protected:
  static Engine* engine_();

private:
  virtual ref<bool> timeout_(const ref<integer>& interval_msec, dtime t0);
};

namespace core
{
// TODO: move this class out of core namespace, otherwise `Lift*`
//       functions can be called via argument dependent look up.
template <typename T> class var_base : public ref<T>
{
public:
  explicit var_base(const internal::ref& r, internal::ref::ctor_guard_t);
  var_base(var_base&& other);

  const T& operator*() const;

  const ref<T>& as_ref() const;

protected:
#ifdef DATAFLOW_CONFIG_VAR_NO_CONST_COPY_CTOR
  var_base(const var_base& other) = delete;
#endif
  var_base(DATAFLOW_VAR_CONST var_base& other);

  void set_value_(const T& v) DATAFLOW_VAR_CONST;

  template <typename Patch> void set_patch_(const Patch& patch);
};
}

template <typename T> class var final : public core::var_base<T>
{
public:
  var(core::var_base<T> other)
  : core::var_base<T>(std::move(other))
  {
  }

  var(DATAFLOW_VAR_CONST var& other)
  : core::var_base<T>(other)
  {
  }

  DATAFLOW_VAR_CONST var& operator=(const T& v) DATAFLOW_VAR_CONST
  {
    core::var_base<T>::set_value_(v);

    return *this;
  }
};

template <typename T> using init_function = std::function<ref<T>(dtime)>;

namespace core
{
template <typename T> class generic_patch
{
public:
  explicit generic_patch(const T& curr, const T& prev);

  T apply(const T& prev) const;

private:
  T curr_;
};

template <typename T, typename Patch = generic_patch<T>> class diff final
{
public:
  using data_type = T;
  using patch_type = Patch;

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

class aggregate_base
{
};

class composite_base
{
};

// Type traits

template <typename T, typename U = T>
using enable_if_flowable = std::enable_if<is_flowable<T>::value, U>;

template <typename T, typename U = T>
using enable_if_flowable_t = typename enable_if_flowable<T, U>::type;

template <typename T>
using is_aggregate_data_type =
  typename std17::conjunction<is_flowable<T>,
                              std::is_base_of<aggregate_base, T>>;

template <typename T, typename U = T>
using enable_if_aggregate_data_type =
  std::enable_if<is_aggregate_data_type<T>::value, U>;

template <typename T, typename U = T>
using enable_if_aggregate_data_type_t =
  typename enable_if_aggregate_data_type<T, U>::type;

template <typename T>
using is_regular_data_type =
  typename std17::conjunction<is_flowable<T>,
                              std17::negation<is_aggregate_data_type<T>>>;

template <typename T, typename U = T>
using enable_if_regular_data_type =
  std::enable_if<is_regular_data_type<T>::value, U>;

template <typename T, typename U = T>
using enable_if_regular_data_type_t =
  typename enable_if_regular_data_type<T, U>::type;

template <typename T>
struct convert_to_flowable
: enable_if_flowable<typename std::conditional<
    is_flowable<std20::remove_cvref_t<T>>::value,
    std20::remove_cvref_t<T>,
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

template <typename T, typename... Ts>
using data_type_tag = std::conditional<
  std17::disjunction<core::is_ref<T>,
                     core::is_aggregate_data_type<T>,
                     core::is_ref<Ts>...,
                     core::is_aggregate_data_type<Ts>...>::value,
  core::aggregate_base,
  core::composite_base>;

template <typename T, typename... Ts>
using data_type_tag_t = typename data_type_tag<T, Ts...>::type;

namespace detail
{
template <typename T> struct is_var
{
  using type = std::false_type;
};

template <typename T> struct is_var<var<T>>
{
  using type = std::true_type;
};
}

/// A type trait that checks whether type `T` is a variable. It is an alias for
/// `std::true_type`, if type `T` is an instance of `var<T>` class template
/// (possibly cv-qualified) or a reference thereto. Otherwise, it is an alias
/// for `std::false_type`.
///
template <typename T>
using is_var = typename detail::is_var<std20::remove_cvref_t<T>>::type;

namespace detail
{
template <typename T> struct is_sig
{
  using type = std::false_type;
};

template <> struct is_sig<sig>
{
  using type = std::true_type;
};
}

/// A type trait that checks whether type `T` is a signal. It is an alias for
/// `std::true_type`, if type `T` is `sig` (possibly cv-qualified) or a
/// reference thereto. Otherwise, it is an alias for `std::false_type`.
///
template <typename T>
using is_sig = typename detail::is_sig<std20::remove_cvref_t<T>>::type;

namespace detail
{
template <typename T> struct init_function_type
{
private:
  template <typename F,
            typename U =
              data_type_t<decltype(std::declval<F>()(std::declval<dtime>()))>>
  static typename std::enable_if<!is_ref<F>::value, U>::type test_(const F*);
  static void test_(...);

public:
  using type = decltype(test_(std::declval<const T*>()));
};
}

template <typename T>
struct is_init_function
: is_flowable<typename detail::init_function_type<T>::type>
{
};

template <typename F>
struct init_function_type
: std::enable_if<is_init_function<F>::value,
                 typename detail::init_function_type<F>::type>
{
};

template <typename F>
using init_function_type_t = typename init_function_type<F>::type;

namespace detail
{
template <typename F, typename T>
struct type_1_recursion_definition_function_result
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

template <typename F, typename T>
struct type_2_recursion_definition_function_result
{
private:
  template <typename FF,
            typename TT,
            typename FwTT = convert_to_flowable_t<TT>,
            typename = std::enable_if<
              std::is_same<init_function_type_t<decltype(
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
using is_recursion_definition_function = std17::disjunction<
  is_flowable<
    typename detail::type_1_recursion_definition_function_result<F, T>::type>,
  is_flowable<
    typename detail::type_2_recursion_definition_function_result<F, T>::type>>;

template <typename F, typename T, typename U = T>
using enable_if_recursion_definition_function =
  std::enable_if<is_recursion_definition_function<F, T>::value, U>;

template <typename F, typename T, typename U = T>
using enable_if_recursion_definition_function_t =
  typename enable_if_recursion_definition_function<F, T, U>::type;

template <typename T>
using argument_data_type = typename std::
  conditional<is_ref<T>::value, data_type<T>, convert_to_flowable<T>>::type;

template <typename T>
using argument_data_type_t = typename argument_data_type<T>::type;

namespace detail
{
template <typename T> struct ref_type
{
private:
  template <typename U> static ref<U> test_(const ref<U>*);
  static void test_(...);

public:
  using type =
    decltype(test_(std::declval<typename std::remove_reference<T>::type*>()));
};
}

template <typename T>
using argument_type = typename std::conditional<is_ref<T>::value,
                                                detail::ref_type<T>,
                                                convert_to_flowable<T>>::type;

template <typename T> using argument_type_t = typename argument_type<T>::type;

template <typename Arg,
          typename T,
          typename U = typename argument_data_type<Arg>::type>
using enable_for_argument_data_type =
  std::enable_if<std::is_same<typename argument_data_type<Arg>::type, T>::value,
                 U>;

template <typename Arg,
          typename T,
          typename U = typename argument_data_type<Arg>::type>
using enable_for_argument_data_type_t =
  typename enable_for_argument_data_type<Arg, T, U>::type;

template <typename T>
using farg_data_type = typename std::conditional<is_init_function<T>::value,
                                                 init_function_type<T>,
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
using farg_result =
  std::conditional<std17::disjunction<core::is_init_function<FArgs>...>::value,
                   init_function<T>,
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

namespace detail
{
template <typename Patch> struct is_generic_patch
{
  using type = std::false_type;
};

template <typename T> struct is_generic_patch<generic_patch<T>>
{
  using type = std::true_type;
};
}

/// A type trait that checks whether type `Patch` is a generic patch. It is an
/// alias for `std::true_type`, if type `Patch` is an instance of
/// `generic_patch<T>` class template (possibly cv-qualified) or a reference
/// thereto. Otherwise, it is an alias for `std::false_type`.
///
template <typename Patch>
using is_generic_patch =
  typename detail::is_generic_patch<std20::remove_cvref_t<Patch>>::type;

/// A type trait that checks if there is no custom patch type defined for `T`
/// data type. It is an alias for `std::true_type`, if `patch_type_t<T>` is a
/// generic patch. Otherwise, it is an alias for `std::false_type`.
///
template <typename T>
using is_trivially_patcheable = is_generic_patch<typename patch_type<T>::type>;

template <typename T> struct diff_type
{
  using type = diff<T, patch_type_t<T>>;
};

template <typename T> using diff_type_t = typename diff_type<T>::type;

template <typename T, typename... Args>
using enable_if_all = std::enable_if<std17::conjunction<Args...>::value, T>;

template <typename T, typename... Args>
using enable_if_all_t = typename enable_if_all<T, Args...>::type;

template <typename T, typename... Args>
using enable_if_any = std::enable_if<std17::disjunction<Args...>::value, T>;

template <typename T, typename... Args>
using enable_if_any_t = typename enable_if_any<T, Args...>::type;

template <typename T, typename... Args>
using enable_if_none =
  std::enable_if<std17::conjunction<std17::negation<Args>...>::value, T>;

template <typename T = void, typename... Args>
using enable_if_none_t = typename enable_if_none<T, Args...>::type;

template <typename Arg, typename... Args>
struct common_data_type
: enable_if_all<
    convert_to_flowable_t<Arg>,
    std::is_same<convert_to_flowable_t<Arg>, convert_to_flowable_t<Args>>...>
{
};

template <typename Arg, typename... Args>
using common_data_type_t = typename common_data_type<Arg, Args...>::type;

template <typename Arg, typename... Args>
using common_argument_data_type = enable_if_all<
  argument_data_type_t<Arg>,
  std::is_same<argument_data_type_t<Arg>, argument_data_type_t<Args>>...>;

template <typename Arg, typename... Args>
using common_argument_data_type_t =
  typename common_argument_data_type<Arg, Args...>::type;

// Utility functions

template <typename T, typename FwT = convert_to_flowable_t<T>>
ref<FwT> make_argument(const T& v);

template <typename T> ref<T> make_argument(const ref<T>& x);

template <typename F>
init_function<init_function_type_t<F>> make_farg(const F& f);

template <typename T> ref<argument_data_type_t<T>> make_farg(const T& x);

template <typename T> std::string to_string(const ref<T>& x);
template <typename T, typename FwT = convert_to_flowable_t<T>>
std::string to_string(const T& x);

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
          typename... Xs,
          typename T = std20::remove_cvref_t<
            decltype(std::declval<Policy>().calculate(std::declval<Xs>()...))>>
ref<T> Lift(Policy policy, const ref<Xs>&... xs);

template <typename Policy,
          typename... Xs,
          typename T = std20::remove_cvref_t<
            decltype(std::declval<Policy>().calculate(std::declval<Xs>()...))>>
ref<T> Lift(const ref<Xs>&... xs);

template <typename Policy,
          typename... Xs,
          typename T = std20::remove_cvref_t<
            decltype(std::declval<Policy>().calculate(std::declval<Xs>()...))>>
ref<T> LiftUpdater(Policy policy, const ref<Xs>&... xs);

template <typename Policy,
          typename... Xs,
          typename T = std20::remove_cvref_t<
            decltype(std::declval<Policy>().calculate(std::declval<Xs>()...))>>
ref<T> LiftUpdater(const ref<Xs>&... xs);

template <
  typename Policy,
  typename X,
  typename... Xs,
  typename T = std20::remove_cvref_t<decltype(std::declval<Policy>().calculate(
    std::declval<X>(), std::declval<Xs>()...))>>
ref<T> LiftPuller(Policy policy, const ref<X>& x, const ref<Xs>&... xs);

template <
  typename Policy,
  typename X,
  typename... Xs,
  typename T = std20::remove_cvref_t<decltype(std::declval<Policy>().calculate(
    std::declval<X>(), std::declval<Xs>()...))>>
ref<T> LiftPuller(const ref<X>& x, const ref<Xs>&... xs);

template <
  typename Policy,
  typename X,
  typename... Xs,
  typename T = argument_data_type_t<decltype(std::declval<Policy>().calculate(
    std::declval<X>(), std::declval<Xs>()...))>>
ref<T> LiftSelector(Policy policy, const ref<X>& x, const ref<Xs>&... xs);

template <
  typename Policy,
  typename X,
  typename... Xs,
  typename T = argument_data_type_t<decltype(std::declval<Policy>().calculate(
    std::declval<X>(), std::declval<Xs>()...))>>
ref<T> LiftSelector(const ref<X>& x, const ref<Xs>&... xs);

template <typename Policy,
          typename... Xs,
          typename T = std20::remove_cvref_t<
            decltype(std::declval<Policy>().calculate(std::declval<Xs>()...))>>
ref<T> LiftPatcher(Policy policy, const ref<Xs>&... xs);

template <typename Policy,
          typename... Xs,
          typename T = std20::remove_cvref_t<
            decltype(std::declval<Policy>().calculate(std::declval<Xs>()...))>>
ref<T> LiftPatcher(const ref<Xs>&... xs);
}

// Basic functions

DATAFLOW___EXPORT ref<dtimestamp> CurrentTime();

DATAFLOW___EXPORT ref<dtimestamp> Timestamp(dtime t);

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

template <
  typename T,
  typename...,
  typename ArgX,
  typename X = core::argument_data_type_t<ArgX>,
  typename = typename std::enable_if<std::is_convertible<X, T>::value>::type>
ref<T> Cast(const ArgX& x);

template <typename T> val<T> Curr(ref<T> x);

template <typename F, typename T = core::init_function_type_t<F>>
val<T> Main(F f);

template <typename T> val<T> Main(ref<T> x);

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
          typename = core::enable_if_any_t<void,
                                           core::is_init_function<FArgT>,
                                           core::is_init_function<FArgU>>>
ref<T> If(const ref<bool>& x, const FArgT& y, const FArgU& z, dtime t0);

template <typename FArgT,
          typename FArgU,
          typename T = core::farg_data_type_t<FArgT>,
          typename = core::enable_for_farg_data_type_t<FArgU, T>,
          typename = core::enable_if_any_t<void,
                                           core::is_init_function<FArgT>,
                                           core::is_init_function<FArgU>>>
init_function<T> If(const ref<bool>& x, const FArgT& y, const FArgU& z);

DATAFLOW___EXPORT ref<bool> Timeout(const arg<integer>& interval_msec,
                                    dtime t0);

// Stateful functions

template <typename Arg,
          typename F,
          typename...,
          typename T = core::enable_if_recursion_definition_function_t<
            F,
            core::argument_data_type_t<Arg>>,
          typename = core::enable_if_regular_data_type_t<T>>
ref<T> Recursion(const Arg& s0, F tf, dtime t0);

template <typename F, typename..., typename T = core::init_function_type_t<F>>
ref<T> Since(const ref<bool>& ti, const F& f, dtime t0);

template <typename F, typename..., typename T = core::init_function_type_t<F>>
init_function<T> Since(const ref<bool>& ti, const F& f);

/// \}
}

#include "core.inl"

#endif // DATAFLOW___CORE_H
