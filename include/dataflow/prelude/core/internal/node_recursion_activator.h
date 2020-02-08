
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

#include <tuple>
#include <utility>

namespace dataflow
{
namespace internal
{
DATAFLOW___EXPORT
std::pair<const node*, update_status>
update_node_recursion_activator(node_id id, bool initialized);

template <typename T> class node_recursion_activator final : public node_t<T>
{
  friend class nodes_factory;

public:
  static ref create()
  {
    return nodes_factory::create<node_recursion_activator<T>>(
      nullptr, 0, node_flags::none);
  }

private:
  explicit node_recursion_activator()
  : node_t<T>()
  {
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    const node* p_state = nullptr;
    update_status status = update_status::nothing;

    std::tie(p_state, status) =
      update_node_recursion_activator(id, initialized);

    DATAFLOW___CHECK_CONDITION(p_state != nullptr);

    status |= this->set_value_(extract_node_value<T>(p_state));

    return status;
  }

  virtual std::string label_() const override
  {
    return "recursion-activator";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }
};

} // internal
} // dataflow
