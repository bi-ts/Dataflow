
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

#include <array>
#include <utility>

namespace dataflow
{
namespace internal
{
template <typename T> class node_if final : public node_t<T>
{
  friend class nodes_factory;

public:
  static ref
  create(const ref& basis, const ref& consequence, const ref& alternative)
  {
    DATAFLOW___CHECK_PRECONDITION(basis.template is_of_type<bool>());
    DATAFLOW___CHECK_PRECONDITION(consequence.template is_of_type<T>());
    DATAFLOW___CHECK_PRECONDITION(alternative.template is_of_type<T>());

    const std::array<node_id, 3> args = {
      {basis.id(), alternative.id(), consequence.id()}};

    return nodes_factory::create_conditional<node_if<T>>(
      &args[0], args.size(), false);
  }

private:
  explicit node_if()
  {
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) override
  {
    DATAFLOW___CHECK_PRECONDITION(p_args != nullptr);
    DATAFLOW___CHECK_PRECONDITION(args_count == 2);

    return this->set_value_(extract_node_value<T>(p_args[1]));
  }

  virtual std::string label_() const override
  {
    return "if";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }
};
} // internal
} // dataflow
