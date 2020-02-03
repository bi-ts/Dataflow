
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

#include <dataflow++_export.h>

#include "node_t.h"
#include "nodes_factory.h"
#include "ref.h"

#include <array>
#include <functional>

namespace dataflow
{
namespace internal
{
DATAFLOW___EXPORT void activate_node_compound(node_id x, node_id y);
DATAFLOW___EXPORT void deactivate_node_compound(node_id x);

template <typename T> class node_compound final : public node_t<T>
{
  friend class nodes_factory;

public:
  static ref create(const std::function<ref(const tick_count&)>& f)
  {
    return nodes_factory::create<node_compound<T>>(
      nullptr, 0, node_flags::none, f);
  }

private:
  explicit node_compound(const std::function<ref(const tick_count&)>& f)
  : f_(f)
  {
  }

  virtual void activate_(node_id id, const tick_count& t0) override
  {
    activate_node_compound(id, f_(t0).id());
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    DATAFLOW___CHECK_PRECONDITION(p_args != nullptr);
    DATAFLOW___CHECK_PRECONDITION(args_count == 1);

    return this->set_value_(extract_node_value<T>(p_args[0]));
  }

  virtual void deactivate_(node_id id) override
  {
    deactivate_node_compound(id);

    node_t<T>::perform_deactivation_();
  }

  virtual std::string label_() const override
  {
    return "compound";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }

private:
  const std::function<ref(const tick_count&)> f_;
};
} // internal
} // dataflow
