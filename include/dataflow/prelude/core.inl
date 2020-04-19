
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

#if !defined(DATAFLOW___CORE_H)
#error '.inl' file can't be included directly. Use 'core.h' instead
#endif

#include "core/internal/config.h"
#include "core/internal/node_compound.h"
#include "core/internal/node_const.h"
#include "core/internal/node_if.h"
#include "core/internal/node_if_activator.h"
#include "core/internal/node_main.h"
#include "core/internal/node_n_ary.h"
#include "core/internal/node_patcher_n_ary.h"
#include "core/internal/node_previous.h"
#include "core/internal/node_recursion.h"
#include "core/internal/node_recursion_activator.h"
#include "core/internal/node_selector.h"
#include "core/internal/node_selector_activator.h"
#include "core/internal/node_since.h"
#include "core/internal/node_since_activator.h"
#include "core/internal/node_snapshot.h"
#include "core/internal/node_snapshot_activator.h"
#include "core/internal/node_updater_n_ary.h"
#include "core/internal/node_var.h"

#include <sstream>

namespace dataflow
{
namespace core
{
// ref_base

template <typename T>
ref_base<T>::ref_base(const internal::ref& r, internal::ref::ctor_guard_t)
: internal::ref(r)
{
  DATAFLOW___CHECK_PRECONDITION(r.is_of_type<T>());
}

template <typename T> ref_base<T> ref_base<T>::snapshot_(dtime t) const
{
  return ref_base<T>(internal::node_snapshot<T>::create(
                       internal::node_snapshot_activator::create(), *this),
                     internal::ref::ctor_guard);
}

template <typename T> void ref_base<T>::reset_(const ref_base<T>& other)
{
  DATAFLOW___CHECK_PRECONDITION(other.is_of_type<T>());

  internal::ref::reset_(other);
}
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
: ref<T>(core::ref_base<T>(r, internal::ref::ctor_guard))
{
}

template <typename T> const T& val<T>::operator*() const
{
  return this->template value<T>();
}

namespace core
{
// var

template <typename T>
var_base<T>::var_base(const internal::ref& r, internal::ref::ctor_guard_t)
: ref<T>(core::ref_base<T>(r, internal::ref::ctor_guard))
{
}

template <typename T> var_base<T>::var_base(var_base&& other) = default;

template <typename T> const T& var_base<T>::operator*() const
{
  DATAFLOW___CHECK_PRECONDITION(
    dynamic_cast<const internal::node_var<T>*>(this->get_()));

  const auto p_var = static_cast<const internal::node_var<T>*>(this->get_());

  return p_var->next_value();
}

template <typename T> const ref<T>& var_base<T>::as_ref() const
{
  return *this;
}

template <typename T> var_base<T>::var_base(var_base& other) = default;

template <typename T> void var_base<T>::set_value_(const T& v)
{
  DATAFLOW___CHECK_PRECONDITION(
    dynamic_cast<const internal::node_var<T>*>(this->get_()));

  const auto p_var = static_cast<const internal::node_var<T>*>(this->get_());

  if (p_var->set_next_value(v))
    this->schedule_();
}

template <typename T>
template <typename Patch>
void var_base<T>::set_patch_(const Patch& patch)
{
  DATAFLOW___CHECK_PRECONDITION(
    dynamic_cast<const internal::node_var<T>*>(this->get_()));

  const auto p_var = static_cast<const internal::node_var<T>*>(this->get_());

  if (p_var->set_next_value(patch.apply(p_var->next_value())))
  {

    this->set_metadata(std::unique_ptr<const internal::metadata>(
      new internal::patch_metadata<Patch>{patch}));

    this->schedule_();
  }
}

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
dataflow::init_function<dataflow::core::init_function_type_t<F>>
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

namespace dataflow
{
namespace core
{
namespace detail
{
template <typename T, typename Policy, typename X, typename... Xs>
ref<T> lift(Policy policy, const ref<X>& x, const ref<Xs>&... xs)
{
  return ref_base<T>(internal::node_n_ary<Policy, T, X, Xs...>::create(
                       std::move(policy), false, x, xs...),
                     internal::ref::ctor_guard);
}

template <typename T, typename Policy> ref<T> lift(Policy policy)
{
  return Const<T>(policy.calculate());
}
}
}
template <typename Policy, typename... Xs, typename T>
ref<T> core::Lift(Policy policy, const ref<Xs>&... xs)
{
  return detail::lift<T>(std::move(policy), xs...);
}

template <typename Policy, typename... Xs, typename T>
ref<T> core::Lift(const ref<Xs>&... xs)
{
  return Lift<Policy>(Policy(), xs...);
}

namespace core
{
namespace detail
{
template <typename T, typename Policy, typename X, typename... Xs>
ref<T> lift_updater(Policy policy, const ref<X>& x, const ref<Xs>&... xs)
{
  return ref_base<T>(internal::node_updater_n_ary<Policy, T, X, Xs...>::create(
                       std::move(policy), false, x, xs...),
                     internal::ref::ctor_guard);
}

template <typename T, typename Policy> ref<T> lift_updater(Policy policy)
{
  return Const<T>(policy.calculate());
}
}
}

template <typename Policy, typename... Xs, typename T>
ref<T> core::LiftUpdater(Policy policy, const ref<Xs>&... xs)
{
  return detail::lift_updater<T>(std::move(policy), xs...);
}

template <typename Policy, typename... Xs, typename T>
ref<T> core::LiftUpdater(const ref<Xs>&... xs)
{
  return LiftUpdater(Policy(), xs...);
}
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T>
dataflow::core::LiftPuller(Policy policy, const ref<X>& x, const ref<Xs>&... xs)
{
  return ref_base<T>(internal::node_n_ary<Policy, T, X, Xs...>::create(
                       std::move(policy), true, x, xs...),
                     internal::ref::ctor_guard);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPuller(const ref<X>& x,
                                            const ref<Xs>&... xs)
{
  return LiftPuller<Policy>(Policy(), x, xs...);
}

namespace dataflow
{
namespace core
{
namespace detail
{
template <typename Policy,
          typename X,
          typename... Xs,
          typename T = data_type_t<decltype(std::declval<Policy>().calculate(
            std::declval<X>(), std::declval<Xs>()...))>>
ref<T> lift_selector(Policy policy, const ref<X>& x, const ref<Xs>&... xs)
{
  static_assert(is_aggregate_data_type<X>::value,
                "An aggregate type is required for the selector");

  return ref_base<T>(
    internal::node_selector<T, X, Policy>::create(
      internal::node_selector_activator<Policy, X, Xs...>::create(
        policy, x, xs...),
      false),
    internal::ref::ctor_guard);
}

template <
  typename Policy,
  typename X,
  typename... Xs,
  typename T = std20::remove_cvref_t<decltype(std::declval<Policy>().calculate(
    std::declval<X>(), std::declval<Xs>()...))>>
ref<typename std::enable_if<!core::is_ref<T>::value, T>::type>
lift_selector(Policy policy, const ref<X>& x, const ref<Xs>&... xs)
{
  return Lift(std::move(policy), x, xs...);
}
}
}
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftSelector(Policy policy,
                                              const ref<X>& x,
                                              const ref<Xs>&... xs)
{
  return detail::lift_selector(std::move(policy), x, xs...);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftSelector(const ref<X>& x,
                                              const ref<Xs>&... xs)
{
  return LiftSelector(Policy(), x, xs...);
}

template <typename Policy, typename X, typename... Xs, typename T>
dataflow::ref<T> dataflow::core::LiftPatcher(Policy policy,
                                             const ref<X>& x,
                                             const ref<Xs>&... xs)
{
  return ref_base<T>(
    internal::node_patcher_n_ary<
      Policy,
      core::patch_type_t<T>,
      core::diff_type_t<X>,
      core::diff_type_t<Xs>...>::create(std::move(policy), false, x, xs...),
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
  return core::ref_base<FwT>(internal::node_const<FwT>::create(v),
                             internal::ref::ctor_guard);
}

template <typename T, typename... Args, typename>
dataflow::ref<T> dataflow::Const(Args&&... args)
{
  return Const(T(std::forward<Args>(args)...));
}

template <typename T, typename FwT> dataflow::var<FwT> dataflow::Var(const T& v)
{
  return core::var_base<FwT>(internal::node_var<FwT>::create(v),
                             internal::ref::ctor_guard);
}

template <typename T, typename... Args, typename>
dataflow::var<T> dataflow::Var(Args&&... args)
{
  return Var(T(std::forward<Args>(args)...));
}

template <typename T> dataflow::val<T> dataflow::Curr(ref<T> x)
{
  return val<T>(internal::node_main<T>::create([x](dtime) { return x; }),
                internal::ref::ctor_guard);
}

template <typename F, typename T> dataflow::val<T> dataflow::Main(F f)
{
  return val<T>(internal::node_main<T>::create(f), internal::ref::ctor_guard);
}

template <typename T> dataflow::val<T> dataflow::Main(ref<T> x)
{
  return Main([x](dtime) { return x; });
}

template <typename T, typename U, typename FwT, typename>
dataflow::ref<FwT> dataflow::If(const ref<bool>& x, const T& y, const U& z)
{
  return core::ref_base<FwT>(
    internal::node_if<FwT>::create(
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
ref<core::init_function_type_t<F>> ref_from_init_function_or_ref(const F& f)
{
  using type = core::init_function_type_t<F>;

  return core::ref_base<type>(internal::node_compound<type>::create(f),
                              internal::ref::ctor_guard);
}

template <typename ArgT>
ref<core::argument_data_type_t<ArgT>>
ref_from_init_function_or_ref(const ArgT& x)
{
  return core::make_argument(x);
}
}
}

template <typename FArgT, typename FArgU, typename T, typename, typename>
dataflow::ref<T>
dataflow::If(const ref<bool>& x, const FArgT& y, const FArgU& z, dtime t0)
{
  return core::ref_base<T>(
    internal::node_if<T>::create(internal::node_if_activator::create(x),
                                 detail::ref_from_init_function_or_ref(y),
                                 detail::ref_from_init_function_or_ref(z),
                                 true),
    internal::ref::ctor_guard);
}

template <typename FArgT, typename FArgU, typename T, typename, typename>
dataflow::init_function<T>
dataflow::If(const ref<bool>& x, const FArgT& y, const FArgU& z)
{
  const auto x_ref = core::make_farg(x);
  const auto y_ref = core::make_farg(y);
  const auto z_ref = core::make_farg(z);

  return [=](dtime t0) { return If(x_ref, y_ref, z_ref, t0); };
}

// Stateful functions

template <typename ArgV0, typename ArgX, typename FwT, typename>
dataflow::ref<FwT> dataflow::Prev(const ArgV0& v0, const ArgX& x, dtime t0)
{
  return core::ref_base<FwT>(
    internal::node_previous<FwT>::create(core::make_argument(v0)(t0), x),
    internal::ref::ctor_guard);
}

template <typename Arg, typename F, typename..., typename T, typename>
dataflow::ref<T> dataflow::Recursion(const Arg& s0, F tf, dtime t0)
{
  struct helper
  {
    static ref<T> init(const ref<T>& x)
    {
      return x;
    }

    static ref<T> init(const init_function<T>& f)
    {
      return core::ref_base<T>(internal::node_compound<T>::create(f),
                               internal::ref::ctor_guard);
    }
  };

  const ref<T> sp = core::ref_base<T>(
    internal::node_recursion_activator<T>::create(), internal::ref::ctor_guard);

  const auto s = helper::init(tf(sp));

  return core::ref_base<T>(
    internal::node_recursion<T>::create(sp, core::make_argument(s0), s),
    internal::ref::ctor_guard);
}

template <typename F, typename..., typename T>
dataflow::ref<T>
dataflow::Since(const ref<dtimestamp>& ti, const F& f, dtime t0)
{
  return core::ref_base<T>(
    internal::node_since<T>::create(internal::node_since_activator::create(ti),
                                    detail::ref_from_init_function_or_ref(f),
                                    true),
    internal::ref::ctor_guard);
}

template <typename F, typename..., typename T>
dataflow::init_function<T> dataflow::Since(const ref<dtimestamp>& ti,
                                           const F& f)
{
  return [=](dtime t0) { return Since(ti, f, t0); };
}
