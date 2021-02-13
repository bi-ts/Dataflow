
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

#include "std_future.h"

#include <cstdlib> // std::size_t
#include <tuple>
#include <utility> // std::move

namespace dataflow
{
namespace utility
{
template <std::size_t... Is, typename Tuple, typename F>
void for_each_tuple_element(const Tuple& t,
                            std14::index_sequence<Is...> indices,
                            F f);

template <typename Tuple, typename F>
void for_each_tuple_element(const Tuple& t, F f);
}

template <std::size_t... Is, typename Tuple, typename F>
void utility::for_each_tuple_element(const Tuple& t,
                                     std14::index_sequence<Is...> indices,
                                     F f)
{
  (void)std::make_tuple([&f, &t]() {
    f(std::get<Is>(t));
    return 0;
  }()...);
}

template <typename Tuple, typename F>
void utility::for_each_tuple_element(const Tuple& t, F f)
{
  for_each_tuple_element(
    t,
    std14::make_index_sequence<std::tuple_size<Tuple>::value>{},
    std::move(f));
}
}
