
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

#include <dataflow/prelude/core/internal/node_since_activator.h>
#include <dataflow/prelude/core/internal/nodes_factory.h>

#include "config.h"
#include "engine.h"

namespace dataflow
{
namespace internal
{
ref node_since_activator::create(const ref& x)
{
  const std::array<node_id, 2> args = {
    {x.id(), nodes_factory::get_time().id()}};

  return nodes_factory::create<node_since_activator>(
    &args[0], args.size(), node_flags::none);
}

node_since_activator::node_since_activator()
: node_t<dtimestamp>()
{
}

update_status node_since_activator::update_(node_id id,
                                            bool initialized,
                                            const node** p_deps,
                                            std::size_t deps_count)
{
  CHECK_PRECONDITION(p_deps != nullptr && deps_count == 2);

  const auto ti = extract_node_value<dtimestamp>(p_deps[0]);
  const auto t = extract_node_value<dtimestamp>(p_deps[1]);

  CHECK_PRECONDITION(t >= ti);

  const auto result = engine::instance().update_node_since_activator(
    converter::convert(id), initialized, ti, t);

  return this->set_value_(ti) | result;
}

std::string node_since_activator::label_() const
{
  return "since-activator";
}
} // internal
} // dataflow
