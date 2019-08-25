
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

#if !defined(DATAFLOW___STATEFUL_H)
#error '.inl' file can't be included directly. Use 'stateful.h' instead
#endif

#include "conditional.h"

namespace dataflow
{
namespace detail
{
namespace stateful
{
template <typename... Trs, std::size_t... Is>
static ref<integer> make1(const std::tuple<Trs...> transitions,
                          ref<integer> def,
                          const internal::std14::index_sequence<Is...>&)
{
  return Switch(
    Case(std::get<Is>(transitions).first, static_cast<integer>(Is) + 1)...,
    Default(def));
}

template <typename T, typename... Trs, std::size_t... Is>
static std::function<ref<T>(const Time&)>
make2(const std::tuple<Trs...> transitions,
      ref<T> def,
      ref<integer> tr_idx,
      const internal::std14::index_sequence<Is...>& seq)
{
  return Switch(
    Case(tr_idx == Const<integer>(Is + 1), std::get<Is>(transitions).second)...,
    Default(def));
}
};
}
}

template <typename T, typename F, typename>
dataflow::ref<T>
dataflow::StateMachine(const ref<T>& initial, const F& f, const Time& t0)
{
  return StateMachine(
    initial,
    [=](const ref<T>& sp) {
      return [=](const Time& t0) {
        const auto transitions = f(sp);

        const auto tr_idx = StateMachine(
          0,
          [=](ref<integer> sp) {
            return detail::stateful::make1(
              transitions,
              sp,
              internal::std14::make_index_sequence<
                std::tuple_size<decltype(transitions)>::value>());
          },
          t0);

        return detail::stateful::make2(
          transitions,
          sp,
          tr_idx,
          internal::std14::make_index_sequence<
            std::tuple_size<decltype(transitions)>::value>())(t0);
      };
    },
    t0);
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
