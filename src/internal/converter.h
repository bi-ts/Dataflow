
//  Copyright (c) 2014 - 2016 Maksym V. Bilinets.
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

#include "graph.h"

#include <dataflow/internal/node.h>

#include <type_traits>

namespace dataflow
{
namespace internal
{
class converter final
{
private:
  static_assert(sizeof(vertex_descriptor) == sizeof(node_id),
                "incompatible types");

  static_assert(std::is_trivially_copyable<node_id>::value,
                "node_id must be trivially copyable");

  static_assert(std::is_trivially_copyable<vertex_descriptor>::value,
                "vertex_descriptor must be trivially copyable");

public:
  static node_id convert(vertex_descriptor v)
  {
    return reinterpret_cast<node_id>(v);
  }

  static vertex_descriptor convert(node_id id)
  {
    return reinterpret_cast<vertex_descriptor>(id);
  }
};
}
}
