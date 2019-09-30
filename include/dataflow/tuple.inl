
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

#if !defined(DATAFLOW___TUPLE_H)
#error '.inl' file can't be included directly. Use 'tuple.h' instead
#endif

#include "internal/std_future.h"

namespace dataflow
{
namespace detail
{
template <typename U> void print_tuple_elements(std::ostream& out, const U& u)
{
  out << core::to_string(u);
}

template <typename U, typename V, typename... Vs>
void print_tuple_elements(std::ostream& out,
                          const U& u,
                          const V& v,
                          const Vs&... vs)
{
  out << core::to_string(u) << "; ";

  print_tuple_elements(out, v, vs...);
}

template <typename... Us, std::size_t... Is>
void print_tuple(std::ostream& out,
                 const tuple<Us...>& v,
                 const internal::std14::index_sequence<Is...>&)
{
  print_tuple_elements(out, get<Is>(v)...);
}

template <typename T>
typename std::enable_if<!core::is_ref<T>::value, bool>::type
compare_elements(const T& lhs, const T& rhs)
{
  return lhs == rhs;
}

template <typename T>
bool compare_elements(const ref<T>& lhs, const ref<T>& rhs)
{
  return lhs.id() == rhs.id();
}

bool compare_pairs()
{
  return true;
}

template <typename U, typename... Us>
bool compare_pairs(const std::pair<U, U>& p, const std::pair<Us, Us>&... ps)
{
  return compare_elements(p.first, p.second) && compare_pairs(ps...);
}

template <typename... Us, std::size_t... Is>
bool compare_tuples(const tuple<Us...>& lhs,
                    const tuple<Us...>& rhs,
                    const internal::std14::index_sequence<Is...>&)
{
  return compare_pairs(std::make_pair(get<Is>(lhs), get<Is>(rhs))...);
}

template <std::size_t I,
          typename... Us,
          typename T = typename std::tuple_element<I, std::tuple<Us...>>::type>
ref<typename std::enable_if<!core::is_ref<T>::value, T>::type>
tuple_get_element(const ref<tuple<Us...>>& x)
{
  struct policy
  {
    static std::string label()
    {
      std::stringstream ss;
      ss << "get<" << I << ">";
      return ss.str();
    }
    static T calculate(const tuple<Us...>& v)
    {
      return get<I>(v);
    };
  };

  return core::Lift<policy>(x);
}

template <std::size_t I,
          typename... Us,
          typename T = typename std::tuple_element<I, std::tuple<Us...>>::type>
typename std::enable_if<core::is_ref<T>::value, T>::type
tuple_get_element(const ref<tuple<Us...>>& x)
{
  struct policy
  {
    static std::string label()
    {
      std::stringstream ss;
      ss << "get<" << I << ">";
      return ss.str();
    }
    static T calculate(const tuple<Us...>& v)
    {
      return get<I>(v);
    }
  };

  return core::LiftSelector<policy>(x);
}

template <typename T>
T default_value(
  const typename std::enable_if<!core::is_ref<T>::value>::type* = nullptr)
{
  return T();
}

template <typename T>
T default_value(
  const typename std::enable_if<core::is_ref<T>::value>::type* = nullptr)
{
  return Const<core::argument_data_type_t<T>>();
}
}

template <typename T, typename... Ts>
tuple<T, Ts...>::tuple()
: p_data_()
{
}

template <typename T, typename... Ts>
tuple<T, Ts...>::tuple(const T& t, const Ts&... ts)
: p_data_(std::make_shared<data>(t, ts...))
{
}

template <typename T, typename... Ts>
bool tuple<T, Ts...>::operator==(const tuple& other) const
{
  return detail::compare_tuples(
    *this, other, internal::std14::make_index_sequence<sizeof...(Ts) + 1>());
}

template <typename T, typename... Ts>
bool tuple<T, Ts...>::operator!=(const tuple& other) const
{
  return !(*this == other);
}

} // dataflow

template <typename... Ts>
std::ostream& dataflow::operator<<(std::ostream& out, const tuple<Ts...>& value)
{

  out << "tuple(";

  detail::print_tuple(
    out, value, internal::std14::make_index_sequence<sizeof...(Ts)>());

  out << ")";

  return out;
}

template <typename T, typename... Ts>
dataflow::tuple<dataflow::core::argument_type_t<T>,
                dataflow::core::argument_type_t<Ts>...>
dataflow::make_tuple(const T& x, const Ts&... xs)
{
  return tuple<dataflow::core::argument_type_t<T>,
               dataflow::core::argument_type_t<Ts>...>(x, xs...);
}

template <std::size_t I, typename... Us>
typename std::tuple_element<I, std::tuple<Us...>>::type
dataflow::get(const tuple<Us...>& t)
{
  if (t.p_data_)
    return std::get<I>(*t.p_data_);

  return detail::default_value<
    typename std::tuple_element<I, std::tuple<Us...>>::type>();
}

template <typename Arg, typename... Args>
dataflow::ref<dataflow::tuple<dataflow::core::argument_data_type_t<Arg>,
                              dataflow::core::argument_data_type_t<Args>...>>
dataflow::TupleE(const Arg& arg, const Args&... args)
{
  struct policy
  {
    static std::string label()
    {
      return "tuple";
    }
    static tuple<dataflow::core::argument_data_type_t<Arg>,
                 dataflow::core::argument_data_type_t<Args>...>
    calculate(const dataflow::core::argument_data_type_t<Arg>& v,
              const dataflow::core::argument_data_type_t<Args>&... vs)
    {
      return make_tuple(v, vs...);
    };
  };

  return core::Lift<policy>(core::make_argument(arg),
                            core::make_argument(args)...);
}

template <std::size_t I, typename... Us>
dataflow::ref<dataflow::core::argument_data_type_t<
  typename std::tuple_element<I, std::tuple<Us...>>::type>>
dataflow::Get(const ref<tuple<Us...>>& x)
{
  return detail::tuple_get_element<I>(x);
}

template <typename A, typename... Args>
dataflow::ref<dataflow::core::argument_data_type_t<A>>
dataflow::First(const ref<tuple<A, Args...>>& x)
{
  return Get<0>(x);
}

template <typename A, typename B, typename... Args>
dataflow::ref<dataflow::core::argument_data_type_t<B>>
dataflow::Second(const ref<tuple<A, B, Args...>>& x)
{
  return Get<1>(x);
}

template <typename A, typename B, typename C, typename... Args>
dataflow::ref<dataflow::core::argument_data_type_t<C>>
dataflow::Third(const ref<tuple<A, B, C, Args...>>& x)
{
  return Get<2>(x);
}

template <typename A, typename B, typename C, typename D, typename... Args>
dataflow::ref<dataflow::core::argument_data_type_t<D>>
dataflow::Fourth(const ref<tuple<A, B, C, D, Args...>>& x)
{
  return Get<3>(x);
}

template <typename A,
          typename B,
          typename C,
          typename D,
          typename E,
          typename... Args>
dataflow::ref<dataflow::core::argument_data_type_t<E>>
dataflow::Fifth(const ref<tuple<A, B, C, D, E, Args...>>& x)
{
  return Get<4>(x);
}
