
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

#include <type_traits>

namespace dataflow
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

template <bool B, class T, class F>
using conditional_t = typename std::conditional<B, T, F>::type;
}

namespace std17
{
template <bool B> using bool_constant = std::integral_constant<bool, B>;

template <typename...> struct conjunction : std::true_type
{
};

template <typename Arg> struct conjunction<Arg> : Arg
{
};

template <typename Arg, typename... Args>
struct conjunction<Arg, Args...>
: std14::conditional_t<static_cast<bool>(Arg::value), conjunction<Args...>, Arg>
{
};

template <typename...> struct disjunction : std::false_type
{
};

template <typename Arg> struct disjunction<Arg> : Arg
{
};

template <typename Arg, typename... Args>
struct disjunction<Arg, Args...>
: std14::conditional_t<static_cast<bool>(Arg::value), Arg, disjunction<Args...>>
{
};

template <typename B>
struct negation : bool_constant<!static_cast<bool>(B::value)>
{
};
}

namespace std20
{
template <typename T>
using remove_cvref = std::remove_cv<typename std::remove_reference<T>::type>;

template <typename T> using remove_cvref_t = typename remove_cvref<T>::type;
}
}
