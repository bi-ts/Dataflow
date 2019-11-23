
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

#ifndef DATAFLOW___LIST_H
#define DATAFLOW___LIST_H

#include "dataflow++_export.h"

#include "maybe.h"
#include "prelude.h"

#include <immer/vector.hpp>
#include <immer/vector_transient.hpp>

namespace dataflow
{
/// \defgroup list
/// \{

namespace list_internal
{
class list_data_impl;

class DATAFLOW___EXPORT listA_data
{
public:
  explicit listA_data();
  explicit listA_data(const std::initializer_list<core::ref_base>& init);
  listA_data(const listA_data& other);
  ~listA_data();

  listA_data& operator=(const listA_data& other);

  bool operator==(const listA_data& other) const;
  bool operator!=(const listA_data& other) const;

  core::ref_base at(std::size_t idx) const;

  std::size_t size() const;

private:
  list_data_impl* p_impl_;
};

template <typename T>
using listC_data =
  immer::vector<T,
                immer::memory_policy<immer::heap_policy<immer::cpp_heap>,
                                     immer::default_refcount_policy>>;

template <typename T> struct select_list_data
{
  using type = listC_data<T>;
};

template <typename T> struct select_list_data<ref<T>>
{
  using type = listA_data;
};
}

template <typename T>
class list final
: public std::conditional<
    internal::std17::disjunction<core::is_ref<T>,
                                 core::is_aggregate_data_type<T>>::value,
    core::aggregate_base,
    core::composite_base>::type
{
public:
  list() = default;

  template <typename U, typename... Us>
  explicit list(const U& x, const Us&... xs);

  bool operator==(const list& other) const;
  bool operator!=(const list& other) const;

  T operator[](integer idx) const;

  integer size() const;

private:
  typename list_internal::select_list_data<T>::type data_;
};

template <typename T> using listA = list<ref<T>>;

template <typename T> using listC = list<T>;

template <typename T>
std::ostream& operator<<(std::ostream& out, const list<T>& value);

template <typename Arg,
          typename... Args,
          typename T = core::common_argument_data_type_t<Arg, Args...>>
listA<T> make_listA(const Arg& x, const Args&... xs);

template <typename Arg,
          typename... Args,
          typename T = core::common_data_type_t<Arg, Args...>>
listC<T> make_listC(const Arg& x, const Args&... xs);

template <typename T> struct list_element_type
{
};

template <typename T> struct list_element_type<list<T>>
{
  using type = T;
};

template <typename T>
using list_element_type_t = typename list_element_type<T>::type;

namespace core
{
template <typename T> class ref_mixin<list<T>>
{
public:
  ref<maybe<T>> operator[](const ref<integer>& idx) const;
};
}

template <typename Arg,
          typename... Args,
          typename T = core::common_argument_data_type_t<Arg, Args...>>
ref<listA<T>> ListA(const Arg& x, const Args&... xs);

template <typename T> ref<integer> Length(const ref<list<T>>& x);

template <typename ArgL,
          typename ArgI,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename = core::enable_for_argument_data_type_t<ArgI, integer>>
ref<maybe<T>> Get(const ArgL& lst, const ArgI& idx);

/// \}
} // dataflow

#include "list.inl"

#endif // DATAFLOW___LIST_H
