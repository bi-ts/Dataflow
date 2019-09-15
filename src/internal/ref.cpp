
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

#include <dataflow/internal/ref.h>

#include "pumpa.h"

namespace dataflow
{

namespace internal
{
ref::ref(const ref& other)
: id_(other.id_)
{
  pumpa::instance().add_ref(converter::convert(id_));
}

ref::~ref()
{
  pumpa::instance().release(converter::convert(id_));
}

const node* ref::get_() const
{
  return pumpa::instance().get_node(converter::convert(id_));
}

void ref::schedule_() const
{
  pumpa::instance().schedule(converter::convert(id_));
  pumpa::instance().pump();
}

ref::ref(node_id id)
: id_(id)
{
  pumpa::instance().add_ref(converter::convert(id_));
}
}
}
