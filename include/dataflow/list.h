
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

#include <list>

namespace dataflow
{
/// \defgroup list
/// \{

template <typename T> class list final
{
private:
  using data = std::list<ref<T>>;

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
        return v.p_data_->size();
      }
    };

    return core::Lift<policy>(x);
  }

private:
  std::shared_ptr<data> p_data_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const list<T>& value);

template <typename U,
          typename... Us,
          typename T = core::argument_data_type_t<U>>
list<T> make_list(const U& x, const Us&... xs);

template <typename U,
          typename... Us,
          typename T = core::argument_data_type_t<U>>
ref<list<T>> List(const U& x, const Us&... xs);

/// \}
} // dataflow

#include "list.inl"

#endif // DATAFLOW___LIST_H
