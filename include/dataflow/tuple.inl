
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

#if !defined(DATAFLOW___TUPLE_H)
#error '.inl' file can't be included directly. Use 'tuple.h' instead
#endif

#include "internal/std_future.h"

namespace dataflow
{
namespace detail
{
template <typename U> void print_tupleE_elements(std::ostream& out, const U& u)
{
  out << u;
}

template <typename U, typename V, typename... Vs>
void print_tupleE_elements(std::ostream& out,
                           const U& u,
                           const V& v,
                           const Vs&... vs)
{
  out << u << "; ";

  print_tupleE_elements(out, v, vs...);
}

template <typename... Us, std::size_t... Is>
void print_tupleE(std::ostream& out,
                  const tupleE<Us...>& v,
                  const internal::std14::index_sequence<Is...>&)
{
  print_tupleE_elements(out, get<Is>(v)...);
}
};

template <typename T, typename... Ts>
tupleE<T, Ts...>::tupleE()
: p_data_(std::make_shared<data>())
{
}

template <typename T, typename... Ts>
tupleE<T, Ts...>::tupleE(const T& t, const Ts&... ts)
: p_data_(std::make_shared<data>(t, ts...))
{
}

template <typename T, typename... Ts>
bool tupleE<T, Ts...>::operator==(const tupleE& other) const
{
  return *p_data_ == *other.p_data_;
}

template <typename T, typename... Ts>
bool tupleE<T, Ts...>::operator!=(const tupleE& other) const
{
  return !(*this == other);
}

} // dataflow

template <typename... Ts>
std::ostream& dataflow::operator<<(std::ostream& out,
                                   const tupleE<Ts...>& value)
{

  out << "tupleE(";

  detail::print_tupleE(
    out, value, internal::std14::make_index_sequence<sizeof...(Ts)>());

  out << ")";

  return out;
}

template <typename T, typename... Ts>
dataflow::tupleE<dataflow::core::convert_to_flowable_t<T>,
                 dataflow::core::convert_to_flowable_t<Ts>...>
dataflow::make_tupleE(const T& t, const Ts&... ts)
{
  return tupleE<core::convert_to_flowable_t<T>,
                core::convert_to_flowable_t<Ts>...>(t, ts...);
}

template <std::size_t I, typename... Us>
const typename std::tuple_element<I, std::tuple<Us...>>::type&
dataflow::get(const tupleE<Us...>& t)
{
  return std::get<I>(*t.p_data_);
}

template <typename... Args>
dataflow::ref<dataflow::tupleE<dataflow::core::argument_data_type_t<Args>...>>
dataflow::TupleE(const Args&... arguments)
{
  struct policy
  {
    static std::string label()
    {
      return "tupleE";
    }
    static tupleE<core::argument_data_type_t<Args>...>
    calculate(const core::argument_data_type_t<Args>&... vs)
    {
      return make_tupleE(vs...);
    };
  };

  return core::Lift<policy>(core::make_argument(arguments)...);
}

template <std::size_t I, typename... Us, typename T>
dataflow::ref<T> dataflow::Get(const ref<tupleE<Us...>>& x)
{
  struct policy
  {
    static std::string label()
    {
      std::stringstream ss;
      ss << "get<" << I << ">";
      return ss.str();
    }
    static const T& calculate(const tupleE<Us...>& v)
    {
      return get<I>(v);
    };
  };

  return core::Lift<policy>(x);
}

template <typename A, typename... Args>
dataflow::ref<A> dataflow::First(const ref<tupleE<A, Args...>>& x)
{
  return Get<0>(x);
}

template <typename A, typename B, typename... Args>
dataflow::ref<B> dataflow::Second(const ref<tupleE<A, B, Args...>>& x)
{
  return Get<1>(x);
}

template <typename A, typename B, typename C, typename... Args>
dataflow::ref<C> dataflow::Third(const ref<tupleE<A, B, C, Args...>>& x)
{
  return Get<2>(x);
}

template <typename A, typename B, typename C, typename D, typename... Args>
dataflow::ref<D> dataflow::Fourth(const ref<tupleE<A, B, C, D, Args...>>& x)
{
  return Get<3>(x);
}

template <typename A,
          typename B,
          typename C,
          typename D,
          typename E,
          typename... Args>
dataflow::ref<E> dataflow::Fifth(const ref<tupleE<A, B, C, D, E, Args...>>& x)
{
  return Get<4>(x);
}
