
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

#pragma once

#ifndef DATAFLOW___LIST_H
#define DATAFLOW___LIST_H

#include "dataflow++_export.h"

#include "maybe.h"
#include "prelude.h"

#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include <vector>

namespace dataflow
{
/// \defgroup list
/// \{

namespace list_internal
{
template <typename T> class assignable_ref;

template <typename T>
using list_data =
  immer::flex_vector<T,
                     immer::memory_policy<immer::heap_policy<immer::cpp_heap>,
                                          immer::default_refcount_policy>>;

template <typename T> struct select_list_data
{
  using type = list_data<T>;
};

template <typename T> struct select_list_data<ref<T>>
{
  using type = list_data<assignable_ref<T>>;
};
}

template <typename T> class list_patch;

template <typename T> class list final : public core::data_type_tag_t<T>
{
private:
  using data_type = typename list_internal::select_list_data<T>::type;

public:
  using patch_type = list_patch<T>;

  using iterator = typename data_type::iterator;
  using const_iterator = iterator;

public:
  list() = default;

  template <typename U, typename... Us>
  explicit list(const U& x, const Us&... xs);

  bool operator==(const list& other) const;
  bool operator!=(const list& other) const;

  list insert(integer idx, const T& v) const;

  list erase(integer idx) const;

  list concat(list other) const;

  list take(integer n) const;

  list skip(integer n) const;

  T operator[](integer idx) const;

  list operator+(list other) const;

  integer size() const;

  const_iterator begin() const;
  const_iterator end() const;

private:
  list(data_type data);

private:
  data_type data_;
};

template <typename T> class list_patch
{
private:
  enum class change_type
  {
    insert,
    erase
  };

  class change_data;

public:
  explicit list_patch();

  explicit list_patch(const list<T>&, const list<T>&);

  void insert(integer idx, const T& v);

  void erase(integer idx);

  template <typename Insert, typename Erase>
  void apply(const Insert& insert, const Erase& erase) const;

  list<T> apply(list<T> v) const;

  bool empty() const;

private:
  std::vector<change_data> changes_;
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

template <typename T> class ref<list<T>> : public core::ref_base<list<T>>
{
public:
  ref(core::ref_base<list<T>> base);

  ref<T> operator()(dtime t) const;

  ref<maybe<T>> operator[](const ref<integer>& idx) const;
};

template <typename T> class var<list<T>> final : public core::var_base<list<T>>
{
public:
  var(core::var_base<list<T>> base);

  var(var<list<T>>& other);

  var& operator=(const list<T>& v);

  void insert(integer idx, const T& v);

  void erase(integer idx);
};

template <typename Arg,
          typename... Args,
          typename T = core::common_argument_data_type_t<Arg, Args...>>
ref<listA<T>> ListA(const Arg& x, const Args&... xs);

template <typename Arg,
          typename... Args,
          typename T = core::common_argument_data_type_t<Arg, Args...>>
ref<listC<T>> ListC(const Arg& x, const Args&... xs);

template <typename T> ref<integer> Length(const ref<list<T>>& x);

template <typename ArgL,
          typename ArgI,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename = core::enable_for_argument_data_type_t<ArgI, integer>>
ref<maybe<T>> Get(const ArgL& lst, const ArgI& idx);

template <typename ArgL,
          typename ArgI,
          typename ArgX,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename = core::enable_for_argument_data_type_t<ArgI, integer>,
          typename = core::enable_for_argument_data_type_t<ArgX, T>>
ref<list<T>> Insert(const ArgL& l, const ArgI& idx, const ArgX& x);

template <typename ArgL,
          typename ArgI,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename = core::enable_for_argument_data_type_t<ArgI, integer>>
ref<list<T>> Erase(const ArgL& l, const ArgI& idx);

template <typename ArgL,
          typename ArgR,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename = core::enable_for_argument_data_type_t<ArgR, list<T>>>
ref<list<T>> Concat(const ArgL& lhs, const ArgR& rhs);

template <typename ArgL,
          typename ArgR,
          typename...,
          typename =
            core::enable_if_any_t<void, core::is_ref<ArgL>, core::is_ref<ArgR>>,
          typename T =
            list_element_type_t<core::common_argument_data_type_t<ArgL, ArgR>>>
ref<list<T>> operator+(const ArgL& lhs, const ArgR& rhs);

template <typename ArgL,
          typename ArgN,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename = core::enable_for_argument_data_type_t<ArgN, integer>>
ref<list<T>> Take(const ArgL& l, const ArgN& num);

template <typename ArgL,
          typename... Args,
          typename F,
          typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
          typename U = core::convert_to_flowable_t<decltype(std::declval<F>()(
            std::declval<const T&>(),
            std::declval<const core::argument_data_type_t<Args>&>()...))>>
ref<list<U>> Map(const ArgL& x, const F& f, const Args&... args);

template <
  typename ArgL,
  typename ArgDelimiter,
  typename...,
  typename T = list_element_type_t<core::argument_data_type_t<ArgL>>,
  typename = core::enable_for_argument_data_type_t<ArgDelimiter, std::string>>
ref<std::string> ToString(const ArgL& l, const ArgDelimiter& delimiter = "");

template <typename ArgL,
          typename...,
          typename = list_element_type_t<core::argument_data_type_t<ArgL>>>
ref<std::string> ToString(const ArgL& l);

/// \}
} // dataflow

#include "list.inl"

#endif // DATAFLOW___LIST_H
