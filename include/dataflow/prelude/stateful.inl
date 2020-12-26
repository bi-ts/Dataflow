
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

#if !defined(DATAFLOW___STATEFUL_H)
#error '.inl' file can't be included directly. Use 'stateful.h' instead
#endif

#include "stateful/internal/curr_prev.h"
#include "stateful/internal/transition.h"

#include "conditional.h"

#include <tuple>

namespace dataflow
{
namespace stateful
{

namespace detail
{
template <typename F, typename T> struct is_sm_definition_function
{
private:
  template <typename... FArgs>
  static std17::conjunction<
    std::is_same<typename core::farg_data_type<FArgs>::type, T>...>
  test_f_(const std::tuple<std::pair<ref<bool>, FArgs>...>&);
  static std::false_type test_f_(...);

  template <typename FF, typename TT>
  static decltype(test_f_(
    std::declval<FF>()(std::declval<ref<core::enable_if_flowable_t<TT>>>())))
  test_(const FF*, const TT*);
  static std::false_type test_(...);

public:
  using type =
    decltype(test_(std::declval<const F*>(), std::declval<const T*>()));
};

template <typename T, typename... Trs, std::size_t... Is>
ref<T> make_state_machine(
  const std::tuple<Trs...>& transitions,
  const std14::index_sequence<Is...>& seq,
  const ref<T>& initial,
  dtime t0,
  const core::enable_if_any_t<
    void,
    core::is_init_function<decltype(
      std::get<Is>(std::declval<std::tuple<Trs...>>()).second)>...>* = nullptr)
{
  struct helper
  {
    static ref<internal::transition> make_case(integer idx, std::false_type)
    {
      return Const<internal::transition>(idx, dtimestamp());
    }

    static init_function<internal::transition> make_case(integer idx,
                                                         std::true_type)
    {
      return [=](dtime t0) { return internal::Transition(idx, t0); };
    }
  };

  const auto tr = Recursion(
    internal::Transition(0, t0),
    [=](ref<internal::transition> sp) {
      return Switch(
        Case(std::get<Is>(transitions).first,
             helper::make_case(static_cast<integer>(Is) + 1,
                               core::is_init_function<decltype(
                                 std::get<Is>(transitions).second)>()))...,
        Default(sp));
    },
    t0);

  const auto tr_idx = Index(tr);

  return Since(When(tr) == CurrentTime(),
               Switch(Case(tr_idx == Const(static_cast<integer>(Is) + 1),
                           std::get<Is>(transitions).second)...,
                      Default(initial)),
               t0);
}

template <typename T, typename... Trs, std::size_t... Is>
ref<T> make_state_machine(
  const std::tuple<Trs...>& transitions,
  const std14::index_sequence<Is...>& seq,
  const ref<T>& initial,
  dtime t0,
  const core::enable_if_none_t<
    void,
    core::is_init_function<decltype(
      std::get<Is>(std::declval<std::tuple<Trs...>>()).second)>...>* = nullptr)
{
  const auto tr_idx = Recursion(
    0,
    [=](ref<integer> sp) {
      return Switch(Case(std::get<Is>(transitions).first,
                         Const(static_cast<integer>(Is) + 1))...,
                    Default(sp));
    },
    t0);

  return Switch(Case(tr_idx == Const(static_cast<integer>(Is) + 1),
                     std::get<Is>(transitions).second)...,
                Default(initial));
}

}
}
}

template <typename ArgT, typename T, typename F, typename>
dataflow::ref<T>
dataflow::StateMachine(const ArgT& initial, const F& f, dtime t0)
{
  const auto initial_arg = core::make_argument(initial);

  return Recursion(
    initial_arg,
    [=](const ref<T>& sp) {
      return [=](dtime t0) {
        const auto transitions = f(sp);

        return stateful::detail::make_state_machine(
          transitions,
          std14::make_index_sequence<
            std::tuple_size<decltype(transitions)>::value>(),
          initial_arg,
          t0);
      };
    },
    t0);
}

template <typename ArgT, typename T, typename F, typename>
dataflow::init_function<T> dataflow::StateMachine(const ArgT& initial,
                                                  const F& f)
{
  const auto initial_ref = core::make_argument(initial);
  return [=](dtime t0) { return StateMachine(initial_ref, f, t0); };
}

template <typename... Trs>
std::tuple<Trs...> dataflow::Transitions(const Trs&... transitions)
{
  return {transitions...};
}

template <typename ArgT, typename FArgU>
std::pair<
  dataflow::ref<dataflow::core::enable_for_argument_data_type_t<ArgT, bool>>,
  dataflow::core::farg_result_t<dataflow::core::farg_data_type_t<FArgU>, FArgU>>
dataflow::On(const ArgT& x, const FArgU& y)
{
  return std::make_pair(core::make_argument(x), core::make_farg(y));
}

template <typename ArgV0, typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::Prev(const ArgV0& v0, const ArgX& x, dtime t0)
{
  return stateful::internal::Prev(Recursion(
    stateful::internal::CurrPrev(x, v0),
    [=](const ref<stateful::internal::curr_prev<T>>& pv) {
      return stateful::internal::CurrPrev(x, Curr(pv));
    },
    t0));
}
