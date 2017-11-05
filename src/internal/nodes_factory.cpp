
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

#include <dataflow/internal/nodes_factory.h>

#include <dst/allocator/utility.h>

#include "engine.h"

namespace dataflow
{
namespace internal
{

void* nodes_factory::allocate_(std::size_t size, std::size_t alignment)
{
  return dst::memory::allocate_aligned(
    engine::instance().get_allocator(), size, alignment);
}

void nodes_factory::deallocate_(void* p_object,
                                std::size_t size,
                                std::size_t alignment)
{
  return dst::memory::deallocate_aligned(
    engine::instance().get_allocator(), p_object, size, alignment);
}

ref nodes_factory::add_(node* p_node,
                        const node_id* p_args,
                        std::size_t args_count,
                        bool eager)
{
  return ref(converter::convert(
    engine::instance().add_node(p_node, p_args, args_count, eager)));
}

ref nodes_factory::add_conditional_(node* p_node,
                                    const node_id* p_args,
                                    std::size_t args_count,
                                    bool eager)
{
  return ref(converter::convert(
    engine::instance().add_node(p_node, p_args, args_count, eager, true)));
}

ref nodes_factory::add_constant_(node* p_node)
{
  return ref(
    converter::convert(engine::instance().add_persistent_node(p_node)));
}

ref nodes_factory::get_time_()
{
  return ref(converter::convert(engine::instance().get_time_node()));
}

} // internal
} // dataflow
