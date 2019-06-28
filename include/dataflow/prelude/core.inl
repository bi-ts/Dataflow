
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

namespace dataflow
{

// ref

template <typename T>
ref<T>::ref(const internal::ref& r)
: internal::ref(r)
{
  DATAFLOW___CHECK_PRECONDITION(r.is_of_type<T>());
}

template <typename T> ref<T> ref<T>::operator()(const Time& t) const
{
  return ref<T>(internal::node_snapshot<T>::create(
    internal::node_snapshot_activator::create(), *this));
}

// val

template <typename T>
val<T>::val(const internal::ref& r)
: ref<T>(r)
{
}

template <typename T> const T& val<T>::operator()() const
{
  return this->template value<T>();
}

// var

template <typename T>
var<T>::var(const internal::ref& r)
: ref<T>(r)
{
}

template <typename T> const var<T>& var<T>::operator=(const T& v) const
{
  DATAFLOW___CHECK_PRECONDITION(
    dynamic_cast<const internal::node_var<T>*>(this->get_()));

  static_cast<const internal::node_var<T>*>(this->get_())->set_next_value(v);

  this->schedule_();

  return *this;
}

namespace core
{
template <typename T>
generic_patch<T>::generic_patch(const T& curr, const T& prev)
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
    internal::node_n_ary<Policy, T, X, Xs...>::create(policy, false, x, xs...));
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
    internal::node_n_ary<Policy, T, X, Xs...>::create(policy, true, x, xs...));
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPuller(const ref<X>& x,
                                            const ref<Xs>&... xs)
{
  return LiftPuller<Policy>(Policy(), x, xs...);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftSelector(const Policy& policy,
                                              const ref<X>& x,
                                              const ref<Xs>&... xs)
{
  return ref<T>(internal::node_selector<T, X, Policy>::create(
    internal::node_selector_activator<Policy, X, Xs...>::create(
      policy, x, xs...),
    false));
}

template <typename Policy, typename X, typename... Xs, typename T>
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
                                                                   xs...));
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
  return ref<FwT>(internal::node_const<FwT>::create(v));
}

template <typename T, typename... Args, typename>
dataflow::ref<T> dataflow::Const(Args&&... args)
{
  return Const(T(std::forward<Args>(args)...));
}

template <typename T, typename FwT> dataflow::var<FwT> dataflow::Var(const T& v)
{
  return var<FwT>(internal::node_var<FwT>::create(v));
}

template <typename T, typename... Args, typename>
dataflow::var<T> dataflow::Var(Args&&... args)
{
  return Var(T(std::forward<Args>(args)...));
}

template <typename T> dataflow::val<T> dataflow::Curr(ref<T> x)
{
  return val<T>(internal::node_main<T>::create([x](const Time&) { return x; }));
}

template <typename F, typename T> dataflow::val<T> dataflow::Main(F f)
{
  return val<T>(internal::node_main<T>::create(f));
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
    false));
}

template <typename F, typename G, typename T>
dataflow::ref<T>
dataflow::If(const ref<bool>& x, const F& y, const G& z, const Time& t0)
{
  return ref<T>(
    internal::node_if<T>::create(internal::node_if_activator::create(x),
                                 ref<T>(internal::node_compound<T>::create(y)),
                                 ref<T>(internal::node_compound<T>::create(z)),
                                 true));
}

template <typename F, typename G, typename T>
std::function<dataflow::ref<T>(const dataflow::Time&)>
dataflow::If(const ref<bool>& x, const F& y, const G& z)
{
  return [=](const Time& t0) { return If(x, y, z, t0); };
}

// Stateful functions

template <typename T>
dataflow::ref<T>
dataflow::Prev(const ref<T>& v0, const ref<T>& x, const Time& t0)
{
  return ref<T>(internal::node_previous<T>::create(v0(t0), x));
}

template <typename Arg, typename F, typename T>
dataflow::ref<T> dataflow::StateMachine(const Arg& s0, F tf, const Time& t0)
{
  struct helper
  {
    static ref<T> init(const ref<T>& x)
    {
      return x;
    }

    static ref<T> init(const std::function<ref<T>(const Time&)>& f)
    {
      return ref<T>(internal::node_compound<T>::create(f));
    }
  };

  const ref<T> sp = ref<T>(internal::node_state_prev<T>::create());

  const auto s = helper::init(tf(sp));

  return ref<T>(
    internal::node_state<T>::create(sp, core::make_argument(s0), s));
}
