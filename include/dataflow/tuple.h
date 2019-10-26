
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

#ifndef DATAFLOW___TUPLE_H
#define DATAFLOW___TUPLE_H

#include "prelude.h"

#include <memory>
#include <tuple>

namespace dataflow
{
/// \defgroup tuple
/// \{

template <typename T, typename... Ts>
class tuple final
: public std::conditional<
    internal::std17::disjunction<core::is_ref<T>,
                                 core::is_aggregate_data_type<T>,
                                 core::is_ref<Ts>...,
                                 core::is_aggregate_data_type<Ts>...>::value,
    core::aggregate_base,
    core::composite_base>::type
{
private:
  using data = std::tuple<T, Ts...>;

public:
  tuple();

  explicit tuple(const T& t, const Ts&... ts);

  bool operator==(const tuple& other) const;
  bool operator!=(const tuple& other) const;

  template <std::size_t I, typename... Us>
  friend typename std::tuple_element<I, std::tuple<Us...>>::type
  get(const tuple<Us...>&);

private:
  std::shared_ptr<data> p_data_;
};

template <typename... Ts>
std::ostream& operator<<(std::ostream& out, const tuple<Ts...>& value);

template <typename T, typename... Ts>
tuple<core::argument_type_t<T>, core::argument_type_t<Ts>...>
make_tuple(const T& x, const Ts&... xs);

template <std::size_t I, typename... Us>
typename std::tuple_element<I, std::tuple<Us...>>::type
get(const tuple<Us...>& t);

template <typename Arg, typename... Args>
ref<tuple<core::argument_data_type_t<Arg>, core::argument_data_type_t<Args>...>>
TupleC(const Arg& arg, const Args&... args);

template <std::size_t I, typename... Us>
ref<core::argument_data_type_t<
  typename std::tuple_element<I, std::tuple<Us...>>::type>>
Get(const ref<tuple<Us...>>& x);

template <typename A, typename... Args>
ref<core::argument_data_type_t<A>> First(const ref<tuple<A, Args...>>& x);

template <typename A, typename B, typename... Args>
ref<core::argument_data_type_t<B>> Second(const ref<tuple<A, B, Args...>>& x);

template <typename A, typename B, typename C, typename... Args>
ref<core::argument_data_type_t<C>> Third(const ref<tuple<A, B, C, Args...>>& x);

template <typename A, typename B, typename C, typename D, typename... Args>
ref<core::argument_data_type_t<D>>
Fourth(const ref<tuple<A, B, C, D, Args...>>& x);

template <typename A,
          typename B,
          typename C,
          typename D,
          typename E,
          typename... Args>
ref<core::argument_data_type_t<E>>
Fifth(const ref<tuple<A, B, C, D, E, Args...>>& x);

template <typename T, typename... Ts>
ref<bool> operator==(const ref<tuple<T, Ts...>>& x,
                     const ref<tuple<T, Ts...>>& y);

template <typename T, typename... Ts>
ref<bool> operator!=(const ref<tuple<T, Ts...>>& x,
                     const ref<tuple<T, Ts...>>& y);
/// \}
} // dataflow

#include "tuple.inl"

#endif // DATAFLOW___TUPLE_H
