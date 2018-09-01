
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

#include <dataflow/internal/node_compound.h>

#include "engine.h"

namespace dataflow
{
void internal::activate_node_compound(node_id x, node_id y)
{
  engine::instance().add_data_edge(converter::convert(x),
                                   converter::convert(y));
}

void internal::deactivate_node_compound(node_id x)
{
  engine::instance().remove_data_edge(converter::convert(x), 0);
}
} // dataflow
