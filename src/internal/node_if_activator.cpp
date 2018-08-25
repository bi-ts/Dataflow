
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

#include <dataflow/internal/node_if_activator.h>
#include <dataflow/internal/nodes_factory.h>

#include "config.h"
#include "engine.h"

namespace dataflow
{
namespace internal
{
ref node_if_activator::create(const ref& condition)
{
  const auto id = condition.id();

  return nodes_factory::create<node_if_activator>(&id, 1, node_flags::none);
}

node_if_activator::node_if_activator()
: node_t<bool>(false)
{
}

update_status node_if_activator::update_(node_id id,
                                         bool initialized,
                                         const node** p_deps,
                                         std::size_t deps_count)
{
  CHECK_PRECONDITION(p_deps != nullptr && deps_count == 1);

  const auto new_value = extract_node_value<bool>(p_deps[0]);

  const auto result =
    engine::instance().update_node_if_activator(converter::convert(id),
                                                initialized,
                                                std::size_t(new_value),
                                                std::size_t(this->value()));

  return this->set_value_(new_value) | result;
}

std::string node_if_activator::label_() const
{
  return "if-activator";
}
} // internal
} // dataflow
