
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

#ifndef DATAFLOW___TUPLE_H
#define DATAFLOW___TUPLE_H

#include "prelude.h"

#include <memory>
#include <tuple>

namespace dataflow
{
/// \defgroup tuple
/// \{

template <typename T, typename... Ts> class tupleE
{
private:
  using data = std::tuple<T, Ts...>;

public:
  tupleE();

  explicit tupleE(const T& t, const Ts&... ts);

  bool operator==(const tupleE& other) const;
  bool operator!=(const tupleE& other) const;

  template <std::size_t I, typename... Us>
  friend const typename std::tuple_element<I, std::tuple<Us...>>::type&
  get(const tupleE<Us...>&);

private:
  std::shared_ptr<data> p_data_;
};

template <typename... Ts>
std::ostream& operator<<(std::ostream& out, const tupleE<Ts...>& value);

template <typename T, typename... Ts>
tupleE<core::convert_to_flowable_t<T>, core::convert_to_flowable_t<Ts>...>
make_tupleE(const T& t, const Ts&... ts);

template <std::size_t I, typename... Us>
const typename std::tuple_element<I, std::tuple<Us...>>::type&
get(const tupleE<Us...>& t);

template <typename... Arg>
ref<tupleE<core::argument_data_type_t<Arg>...>> TupleE(const Arg&... arguments);

template <std::size_t I,
          typename... Us,
          typename T = core::convert_to_flowable_t<
            decltype(get<I>(std::declval<tupleE<Us...>>()))>>
ref<T> Get(const ref<tupleE<Us...>>& x);

/// \}
} // dataflow

#include "tuple.inl"

#endif // DATAFLOW___TUPLE_H
