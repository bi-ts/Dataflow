
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include <dataflow/internal/node_activator.h>
#include <dataflow/internal/nodes_factory.h>

#include "config.h"
#include "engine.h"

namespace dataflow
{
namespace internal
{
ref node_activator::create(const ref& condition)
{
  const auto id = condition.id();

  return nodes_factory::create<node_activator>(&id, 1, false);
}

node_activator::node_activator()
: node_t<bool>(false)
{
}

bool node_activator::update_(node_id id,
                             bool initialized,
                             const node** p_deps,
                             std::size_t deps_count)
{
  CHECK_PRECONDITION(p_deps != nullptr && deps_count == 1);

  const auto new_value = extract_node_value<bool>(p_deps[0]);

  if (engine::instance().update_node_activator(converter::convert(id),
                                               initialized,
                                               std::size_t(new_value),
                                               std::size_t(this->value())))
  {
    this->set_value_(new_value);

    return true;
  }

  return false;
}

std::string node_activator::label_() const
{
  return "activator";
}
} // internal
} // dataflow
