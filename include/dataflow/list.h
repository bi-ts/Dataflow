
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

#include "prelude.h"

namespace dataflow
{
/// \defgroup list
/// \{

namespace list_internal
{
class list_data_impl;

class DATAFLOW___EXPORT list_data
{
public:
  explicit list_data();
  explicit list_data(const std::initializer_list<core::ref_base>& init);
  list_data(const list_data& other);
  ~list_data();

  list_data& operator=(const list_data& other);

  bool operator==(const list_data& other) const;
  bool operator!=(const list_data& other) const;

  std::size_t size() const;

private:
  list_data_impl* p_impl_;
};

}

template <typename T> class list final
{
public:
  list() = default;

  template <typename U, typename... Us>
  explicit list(const U& x, const Us&... xs);

  bool operator==(const list& other) const;
  bool operator!=(const list& other) const;

  friend ref<std::size_t> Length(const ref<list>& x)
  {
    struct policy
    {
      static std::string label()
      {
        return "length";
      }
      static std::size_t calculate(const list& v)
      {
        return v.data_.size();
      }
    };

    return core::Lift<policy>(x);
  }

private:
  list_internal::list_data data_;
};

template <typename T> using listA = list<ref<T>>;

template <typename T>
std::ostream& operator<<(std::ostream& out, const list<T>& value);

template <typename Arg,
          typename... Args,
          typename T = core::common_argument_data_type_t<Arg, Args...>>
listA<T> make_listA(const Arg& x, const Args&... xs);

template <typename Arg,
          typename... Args,
          typename T = core::common_argument_data_type_t<Arg, Args...>>
ref<listA<T>> ListA(const Arg& x, const Args&... xs);

/// \}
} // dataflow

#include "list.inl"

#endif // DATAFLOW___LIST_H
