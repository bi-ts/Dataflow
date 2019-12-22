
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

#pragma once

#include "graph.h"
#include "node_time.h"
#include "tick_count.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace dataflow
{
namespace internal
{
class pumpa final
{
public:
  pumpa(const memory_allocator<char>& allocator);

  std::size_t changed_nodes_count() const;

  std::size_t updated_nodes_count() const;

  void schedule_for_next_update(topological_position position);

  void set_metadata(const node* p_node,
                    std::shared_ptr<const metadata> p_metadata);
  const std::shared_ptr<const metadata>& get_metadata(const node* p_node);

  void pump(dependency_graph& graph,
            topological_list& order,
            vertex_descriptor time_node_v,
            tick_count& ticks);

  void pump_(dependency_graph& graph,
             topological_list& order,
             vertex_descriptor time_node_v,
             tick_count& ticks);

private:
  bool pumping_started_;
  std::vector<const node*, memory_allocator<const node*>> args_buffer_;
  std::vector<topological_position, memory_allocator<topological_position>>
    next_update_;

  std::unordered_map<const node*, std::shared_ptr<const metadata>> metadata_;
  const std::shared_ptr<const metadata> p_no_metadata_;

  std::size_t changed_nodes_count_;
  std::size_t updated_nodes_count_;
};
} // internal
} // dataflow
