
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

#include <dataflow/internal/node_snapshot_activator.h>
#include <dataflow/internal/nodes_factory.h>

#include "pumpa.h"

namespace dataflow
{
namespace internal
{
ref node_snapshot_activator::create()
{
  return nodes_factory::create<node_snapshot_activator>(
    nullptr, 0, node_flags::none);
}

node_snapshot_activator::node_snapshot_activator()
: node_t<bool>(false)
{
}

update_status node_snapshot_activator::update_(node_id id,
                                               bool initialized,
                                               const node** p_deps,
                                               std::size_t deps_count)
{
  CHECK_PRECONDITION(deps_count == 0);

  return pumpa::instance().update_node_snapshot_activator(
    converter::convert(id), initialized);
}

std::string node_snapshot_activator::label_() const
{
  return "snapshot-activator";
}
} // internal
} // dataflow
