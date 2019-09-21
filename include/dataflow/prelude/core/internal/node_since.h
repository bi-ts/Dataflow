
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

#include "../../../internal/config.h"
#include "../../../internal/node_t.h"
#include "../../../internal/nodes_factory.h"
#include "../../../internal/ref.h"

#include <dataflow/prelude/core/dtime.h>

#include <array>
#include <utility>

namespace dataflow
{
namespace internal
{
template <typename T> class node_since final : public node_t<T>
{
  friend class nodes_factory;

public:
  static ref create(const ref& basis, const ref& x, bool eager)
  {
    DATAFLOW___CHECK_PRECONDITION(basis.template is_of_type<dtimestamp>());
    DATAFLOW___CHECK_PRECONDITION(x.template is_of_type<T>());

    const std::array<node_id, 2> args = {{basis.id(), x.id()}};

    return nodes_factory::create_conditional<node_since<T>>(
      &args[0], args.size(), eager ? node_flags::eager : node_flags::none);
  }

private:
  explicit node_since()
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
    return "since";
  }

  virtual std::pair<std::size_t, std::size_t> mem_info_() const override final
  {
    return std::make_pair(sizeof(*this), alignof(decltype(*this)));
  }
};
} // internal
} // dataflow
