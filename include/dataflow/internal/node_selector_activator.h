
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

#include "dataflow++_export.h"

#include "node_t.h"
#include "nodes_factory.h"
#include "ref.h"

#include "std_future.h"

namespace dataflow
{
namespace internal
{
DATAFLOW___EXPORT bool is_prev(node_id x);

DATAFLOW___EXPORT update_status
update_node_selector_activator(node_id id, node_id x, bool initialized);

template <typename Policy, typename X, typename... Xs>
class node_selector_activator final : public node_t<X>, Policy
{
  friend class nodes_factory;

public:
  static ref create(const Policy& policy, const ref& x, ref_t<Xs>... xs)
  {
    DATAFLOW___CHECK_PRECONDITION(
      check_all(x.template is_of_type<X>(), xs.template is_of_type<Xs>()...));

    if (is_prev(x.id()))
      throw std::logic_error("Field selection is not allowed on 'prev' values");

    const std::array<node_id, 1 + sizeof...(Xs)> args = {{x.id(), xs.id()...}};

    return nodes_factory::create<node_selector_activator<Policy, X, Xs...>>(
      &args[0], args.size(), node_flags::none, policy);
  }

private:
  explicit node_selector_activator(const Policy& policy)
  : Policy(policy)
  {
  }

  // TODO: move to utilities
  template <typename... Args, std::size_t... Is>
  ref calculate_(const node** p_args, const std14::index_sequence<Is...>&)
  {
    return Policy::calculate(extract_node_value<Args>(p_args[Is])...);
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    DATAFLOW___CHECK_PRECONDITION(p_args != nullptr);
    DATAFLOW___CHECK_PRECONDITION(args_count == 1 + sizeof...(Xs));

    const auto& v = extract_node_value<X>(p_args[0]);

    const auto status = this->set_value_(v);

    const auto x = calculate_<X, Xs...>(
      p_args, std14::make_index_sequence<1 + sizeof...(Xs)>());

    return status | update_node_selector_activator(id, x.id(), initialized);
  }

  virtual std::string label_() const override
  {
    return "selector-activator";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }
};
} // internal
} // dataflow
