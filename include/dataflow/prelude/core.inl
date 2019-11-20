
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

#if !defined(DATAFLOW___CORE_H)
#error '.inl' file can't be included directly. Use 'core.h' instead
#endif

#include "../internal/config.h"
#include "../internal/node_compound.h"
#include "../internal/node_const.h"
#include "../internal/node_if.h"
#include "../internal/node_if_activator.h"
#include "../internal/node_main.h"
#include "../internal/node_n_ary.h"
#include "../internal/node_patcher_n_ary.h"
#include "../internal/node_previous.h"
#include "../internal/node_selector.h"
#include "../internal/node_selector_activator.h"
#include "../internal/node_snapshot.h"
#include "../internal/node_snapshot_activator.h"
#include "../internal/node_state.h"
#include "../internal/node_state_prev.h"
#include "../internal/node_var.h"

#include "core/internal/node_since.h"
#include "core/internal/node_since_activator.h"

#include <sstream>

namespace dataflow
{

// ref

template <typename T>
ref<T>::ref(const internal::ref& r, internal::ref::ctor_guard_t)
: internal::ref(r)
{
  DATAFLOW___CHECK_PRECONDITION(r.is_of_type<T>());
}

template <typename T> ref<T> ref<T>::operator()(const Time& t) const
{
  return ref<T>(internal::node_snapshot<T>::create(
                  internal::node_snapshot_activator::create(), *this),
                internal::ref::ctor_guard);
}

// arg

template <typename T>
arg<T>::arg(const ref<T>& x)
: ref<T>(x)
{
}

template <typename T>
template <typename U, typename..., typename>
arg<T>::arg(U&& value)
: arg(Const(value))
{
}

// val

template <typename T>
val<T>::val(const internal::ref& r, internal::ref::ctor_guard_t)
: ref<T>(r, internal::ref::ctor_guard)
{
}

template <typename T> const T& val<T>::operator()() const
{
  return this->template value<T>();
}

// var

template <typename T>
var<T>::var(const internal::ref& r, internal::ref::ctor_guard_t)
: ref<T>(r, internal::ref::ctor_guard)
, readonly_(false)
{
}

template <typename T>
var<T>::var(const var& other)
: ref<T>(other)
, readonly_(true)
{
}

template <typename T> var<T>::var(var& other) = default;

template <typename T> var<T>::var(var&& other) = default;

template <typename T> const var<T>& var<T>::operator=(const T& v)
{
  if (readonly_)
    throw std::logic_error("variable is readonly");

  DATAFLOW___CHECK_PRECONDITION(
    dynamic_cast<const internal::node_var<T>*>(this->get_()));

  static_cast<const internal::node_var<T>*>(this->get_())->set_next_value(v);

  this->schedule_();

  return *this;
}

namespace core
{
template <typename T>
generic_patch<T>::generic_patch(const T& curr, const T&)
: curr_(curr)
{
}

template <typename T> T generic_patch<T>::apply(const T&) const
{
  return curr_;
}

template <typename T, typename Patch>
diff<T, Patch>::diff(const T& curr, const T& prev, const Patch& patch)
: curr_(curr)
, prev_(prev)
, patch_(patch)
{
}

template <typename T, typename Patch>
diff<T, Patch>::diff(const T& curr, const T& prev)
: curr_(curr)
, prev_(prev)
, patch_(curr, prev)
{
}

template <typename T, typename Patch> const T& diff<T, Patch>::curr() const
{
  return curr_;
}

template <typename T, typename Patch> const T& diff<T, Patch>::prev() const
{
  return prev_;
}

template <typename T, typename Patch> const Patch& diff<T, Patch>::patch() const
{
  return patch_;
}
}
}

// Utility functions

template <typename T, typename FwT>
dataflow::ref<FwT> dataflow::core::make_argument(const T& v)
{
  return Const(v);
}

template <typename T>
dataflow::ref<T> dataflow::core::make_argument(const ref<T>& x)
{
  return x;
}

template <typename F>
dataflow::function_of_time<dataflow::core::function_of_time_type_t<F>>
dataflow::core::make_farg(const F& f)
{
  return f;
}

template <typename T>
dataflow::ref<dataflow::core::argument_data_type_t<T>>
dataflow::core::make_farg(const T& x)
{
  return make_argument(x);
}

template <typename T>
std::string dataflow::core::to_string(const dataflow::ref<T>& x)
{
  std::stringstream ss;
  ss << std::hex << x.id();
  return ss.str();
}

template <typename T, typename FwT>
std::string dataflow::core::to_string(const T& x)
{
  std::stringstream ss;
  ss << static_cast<const FwT&>(x);
  return ss.str();
}

template <typename F, typename X, typename T>
dataflow::ref<T>
dataflow::core::Lift(const std::string& label, const ref<X>& x, F func)
{
  class policy
  {
  public:
    policy(const std::string& label, const F& func)
    : label_(label)
    , func_(func)
    {
    }

    std::string label() const
    {
      return label_;
    }

    T calculate(const X& v) const
    {
      return func_(v);
    };

  private:
    std::string label_;
    F func_;
  };

  return Lift(policy(label, func), x);
}

template <typename F, typename X, typename Y, typename T>
dataflow::ref<T> dataflow::core::Lift(const std::string& label,
                                      const ref<X>& x,
                                      const ref<Y>& y,
                                      F func)
{
  class policy
  {
  public:
    policy(const std::string& label, const F& func)
    : label_(label)
    , func_(func)
    {
    }

    const std::string& label() const
    {
      return label_;
    }

    T calculate(const X& x, const Y& y)
    {
      return func_(x, y);
    };

  private:
    std::string label_;
    F func_;
  };

  return Lift(policy(label, func), x, y);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::Lift(const Policy& policy,
                                      const ref<X>& x,
                                      const ref<Xs>&... xs)
{
  return ref<T>(
    internal::node_n_ary<Policy, T, X, Xs...>::create(policy, false, x, xs...),
    internal::ref::ctor_guard);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::Lift(const ref<X>& x, const ref<Xs>&... xs)
{
  return Lift<Policy>(Policy(), x, xs...);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPuller(const Policy& policy,
                                            const ref<X>& x,
                                            const ref<Xs>&... xs)
{
  return ref<T>(
    internal::node_n_ary<Policy, T, X, Xs...>::create(policy, true, x, xs...),
    internal::ref::ctor_guard);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPuller(const ref<X>& x,
                                            const ref<Xs>&... xs)
{
  return LiftPuller<Policy>(Policy(), x, xs...);
}

template <typename Policy, typename X, typename... Xs, typename T, typename>
dataflow::ref<T> dataflow::core::LiftSelector(const Policy& policy,
                                              const ref<X>& x,
                                              const ref<Xs>&... xs)
{
  return ref<T>(internal::node_selector<T, X, Policy>::create(
                  internal::node_selector_activator<Policy, X, Xs...>::create(
                    policy, x, xs...),
                  false),
                internal::ref::ctor_guard);
}

template <typename Policy, typename X, typename... Xs, typename T, typename>
dataflow::ref<T> dataflow::core::LiftSelector(const ref<X>& x,
                                              const ref<Xs>&... xs)
{
  return LiftSelector(Policy(), x, xs...);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPatcher(const Policy& policy,
                                             const ref<X>& x,
                                             const ref<Xs>&... xs)
{
  return ref<T>(
    internal::node_patcher_n_ary<Policy,
                                 core::patch_type_t<T>,
                                 core::diff_type_t<X>,
                                 core::diff_type_t<Xs>...>::create(policy,
                                                                   false,
                                                                   x,
                                                                   xs...),
    internal::ref::ctor_guard);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPatcher(const ref<X>& x,
                                             const ref<Xs>&... xs)
{
  return LiftPatcher(Policy(), x, xs...);
}

// Basic functions

template <typename T, typename FwT>
dataflow::ref<FwT> dataflow::Const(const T& v)
{
  return ref<FwT>(internal::node_const<FwT>::create(v),
                  internal::ref::ctor_guard);
}

template <typename T, typename... Args, typename>
dataflow::ref<T> dataflow::Const(Args&&... args)
{
  return Const(T(std::forward<Args>(args)...));
}

template <typename T, typename FwT> dataflow::var<FwT> dataflow::Var(const T& v)
{
  return var<FwT>(internal::node_var<FwT>::create(v),
                  internal::ref::ctor_guard);
}

template <typename T, typename... Args, typename>
dataflow::var<T> dataflow::Var(Args&&... args)
{
  return Var(T(std::forward<Args>(args)...));
}

template <typename T> dataflow::val<T> dataflow::Curr(ref<T> x)
{
  return val<T>(internal::node_main<T>::create([x](const Time&) { return x; }),
                internal::ref::ctor_guard);
}

template <typename F, typename T> dataflow::val<T> dataflow::Main(F f)
{
  return val<T>(internal::node_main<T>::create(f), internal::ref::ctor_guard);
}

// Operators

template <typename T> dataflow::val<T> dataflow::operator*(ref<T> x)
{
  return Curr(x);
}

template <typename T, typename U, typename FwT, typename>
dataflow::ref<FwT> dataflow::If(const ref<bool>& x, const T& y, const U& z)
{
  return ref<FwT>(internal::node_if<FwT>::create(
                    internal::node_if_activator::create(core::make_argument(x)),
                    core::make_argument(y),
                    core::make_argument(z),
                    false),
                  internal::ref::ctor_guard);
}

namespace dataflow
{
namespace detail
{
template <typename F>
ref<core::function_of_time_type_t<F>>
ref_from_function_of_time_or_ref(const F& f)
{
  using type = core::function_of_time_type_t<F>;

  return ref<type>(internal::node_compound<type>::create(f),
                   internal::ref::ctor_guard);
}

template <typename ArgT>
ref<core::argument_data_type_t<ArgT>>
ref_from_function_of_time_or_ref(const ArgT& x)
{
  return core::make_argument(x);
}
}
}

template <typename FArgT, typename FArgU, typename T, typename, typename>
dataflow::ref<T>
dataflow::If(const ref<bool>& x, const FArgT& y, const FArgU& z, const Time& t0)
{
  return ref<T>(
    internal::node_if<T>::create(internal::node_if_activator::create(x),
                                 detail::ref_from_function_of_time_or_ref(y),
                                 detail::ref_from_function_of_time_or_ref(z),
                                 true),
    internal::ref::ctor_guard);
}

template <typename FArgT, typename FArgU, typename T, typename, typename>
dataflow::function_of_time<T>
dataflow::If(const ref<bool>& x, const FArgT& y, const FArgU& z)
{
  return [=](const Time& t0) { return If(x, y, z, t0); };
}

// Stateful functions

template <typename ArgV0, typename ArgX, typename FwT, typename>
dataflow::ref<FwT>
dataflow::Prev(const ArgV0& v0, const ArgX& x, const Time& t0)
{
  return ref<FwT>(
    internal::node_previous<FwT>::create(core::make_argument(v0)(t0), x),
    internal::ref::ctor_guard);
}

template <typename Arg, typename F, typename..., typename T, typename>
dataflow::ref<T> dataflow::StateMachine(const Arg& s0, F tf, const Time& t0)
{
  struct helper
  {
    static ref<T> init(const ref<T>& x)
    {
      return x;
    }

    static ref<T> init(const function_of_time<T>& f)
    {
      return ref<T>(internal::node_compound<T>::create(f),
                    internal::ref::ctor_guard);
    }
  };

  const ref<T> sp =
    ref<T>(internal::node_state_prev<T>::create(), internal::ref::ctor_guard);

  const auto s = helper::init(tf(sp));

  return ref<T>(internal::node_state<T>::create(sp, core::make_argument(s0), s),
                internal::ref::ctor_guard);
}

template <typename F, typename..., typename T>
dataflow::ref<T>
dataflow::Since(const ref<dtimestamp>& ti, const F& f, const Time& t0)
{
  return ref<T>(
    internal::node_since<T>::create(internal::node_since_activator::create(ti),
                                    detail::ref_from_function_of_time_or_ref(f),
                                    true),
    internal::ref::ctor_guard);
}

template <typename F, typename..., typename T>
dataflow::function_of_time<T> dataflow::Since(const ref<dtimestamp>& ti,
                                              const F& f)
{
  return [=](const Time& t0) { return Since(ti, f, t0); };
}
