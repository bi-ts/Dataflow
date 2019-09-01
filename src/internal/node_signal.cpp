
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

#include <dataflow/internal/node_signal.h>

#include <dataflow/internal/nodes_factory.h>

#include <utility>

namespace dataflow
{
namespace internal
{
ref node_signal::create()
{
  return nodes_factory::create<node_signal>(nullptr, 0, node_flags::none);
}

node_signal::node_signal()
: node_t<bool>()
{
}

update_status node_signal::update_(node_id id,
                                   bool initialized,
                                   const node** p_args,
                                   std::size_t args_count)
{
  const auto value = initialized && !this->value();

  const auto status = this->set_value_(value);

  return value ? status | update_status::updated_next : status;
}

std::string node_signal::label_() const
{
  return "signal";
}

std::pair<std::size_t, std::size_t> node_signal::mem_info_() const
{
  return std::make_pair(sizeof(*this), alignof(decltype(*this)));
}
} // internal
} // dataflow
