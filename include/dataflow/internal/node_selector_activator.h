
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

#include "dataflow++_export.h"

#include "node_t.h"
#include "nodes_factory.h"
#include "ref.h"

namespace dataflow
{
namespace internal
{
DATAFLOW___EXPORT update_status
update_node_selector_activator(node_id id, node_id x, bool initialized);

template <typename T, typename Policy>
class node_selector_activator final : public node_t<T>, Policy
{
  friend class nodes_factory;

public:
  static ref create(const ref& x, const Policy& policy = Policy())
  {
    DATAFLOW___CHECK_PRECONDITION(x.template is_of_type<T>());

    const auto id = x.id();

    return nodes_factory::create<node_selector_activator<T, Policy>>(
      &id, 1, node_flags::none, policy);
  }

private:
  explicit node_selector_activator(const Policy& policy)
  : Policy(policy)
  {
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    DATAFLOW___CHECK_PRECONDITION(p_args != nullptr);
    DATAFLOW___CHECK_PRECONDITION(args_count == 1);

    const auto& v = extract_node_value<T>(p_args[0]);

    if (this->set_value_(v) == update_status::nothing)
      return update_status::nothing;

    const ref x = Policy::calculate(v);

    return update_node_selector_activator(id, x.id(), initialized);
  }

  virtual std::string label_() const override
  {
    return "selector_activator";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }
};
} // internal
} // dataflow
