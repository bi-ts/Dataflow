
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

namespace dataflow
{
namespace internal
{
namespace std14
{
template <std::size_t... Is> class index_sequence
{
};

namespace detail
{
template <std::size_t N, std::size_t... Is>
struct index_sequence_builder : index_sequence_builder<N - 1, N - 1, Is...>
{
};

template <std::size_t... Is> struct index_sequence_builder<0, Is...>
{
  using type = index_sequence<Is...>;
};
}

template <std::size_t N>
using make_index_sequence = typename detail::index_sequence_builder<N>::type;
}
}
}
