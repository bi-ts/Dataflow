
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

#include <dataflow/prelude/core/internal/node.h>

#include "engine.h"

namespace dataflow
{
namespace internal
{

const discrete_time& node::ticks_()
{
  return engine::instance().ticks();
}

void node::set_metadata(const node* p_node,
                        std::shared_ptr<const metadata> p_metadata)
{
  engine::instance().set_metadata(p_node, std::move(p_metadata));
}

const std::shared_ptr<const metadata>& node::get_metadata(const node* p_node)
{
  return engine::instance().get_metadata(p_node);
}

} // internal
} // dataflow
