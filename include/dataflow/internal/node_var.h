
//  Copyright (c) 2014 - 2018 Maksym V. Bilinets.
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

#include "config.h"
#include "node_t.h"
#include "nodes_factory.h"
#include "ref.h"

#include <utility>

namespace dataflow
{
namespace internal
{
template <typename T> class node_var final : public node_t<T>
{
  friend class nodes_factory;

public:
  static ref create(const T& v)
  {
    return nodes_factory::create<node_var<T>>(nullptr, 0, node_flags::none, v);
  }

  void set_next_value(const T& v) const
  {
    next_value_ = v;
  }

private:
  explicit node_var(const T& v)
  : node_t<T>(v)
  , next_value_(v)
  {
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    return this->set_value_(next_value_);
  }

  virtual std::string label_() const override
  {
    return "var";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }

private:
  mutable T next_value_;
};
} // internal
} // dataflow
