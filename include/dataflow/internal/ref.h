
//  Copyright 2014-2016 Maksym V. Bilinets.
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

#include "dataflow++_export.h"

#include "config.h"
#include "node_t.h"

namespace dataflow
{
namespace internal
{

class DATAFLOW___EXPORT ref
{
  friend class nodes_factory;

public:
  ref(const ref& other);

  ~ref();

  template <typename T> bool is_of_type() const;
  template <typename T> const T& value() const;

  node_id id() const;

protected:
  const node* get_() const;

  void schedule_() const;

private:
  explicit ref(node_id id);

private:
  ref& operator=(const ref&) = delete;

private:
  node_id id_;
};

inline node_id ref::id() const
{
  return id_;
}

template <typename T> bool ref::is_of_type() const
{
  return dynamic_cast<const node_t<T>*>(get_()) != nullptr;
}

template <typename T> const T& ref::value() const
{
  DF_CHECK_PRECONDITION(this->is_of_type<T>());

  return static_cast<const node_t<T>*>(get_())->value();
}
} // internal
} // dataflow
