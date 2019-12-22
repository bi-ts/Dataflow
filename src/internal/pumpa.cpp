
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

#include "pumpa.h"

#include "converter.h"

namespace dataflow
{
namespace internal
{
pumpa::pumpa(const memory_allocator<char>& allocator)
: pumping_started_(false)
, args_buffer_(allocator)
, changed_nodes_count_()
, updated_nodes_count_()
{
}

std::size_t pumpa::changed_nodes_count() const
{
  CHECK_PRECONDITION(!pumping_started_);

  return changed_nodes_count_;
}

std::size_t pumpa::updated_nodes_count() const
{
  CHECK_PRECONDITION(!pumping_started_);

  return updated_nodes_count_;
}

void pumpa::schedule_for_next_update(topological_position position)
{
  CHECK_PRECONDITION(pumping_started_);

  if (position != topological_position())
    next_update_.push_back(position);
}

void pumpa::set_metadata(const node* p_node,
                         std::shared_ptr<const metadata> p_metadata)
{
  CHECK_CONDITION(metadata_.find(p_node) == metadata_.end());

  metadata_[p_node] = std::move(p_metadata);
}

const std::shared_ptr<const metadata>& pumpa::get_metadata(const node* p_node)
{
  const auto it = metadata_.find(p_node);

  if (it == metadata_.end())
    return p_no_metadata_;

  return it->second;
}

void pumpa::pump(dependency_graph& graph,
                 topological_list& order,
                 vertex_descriptor time_node_v,
                 tick_count& ticks)
{
  CHECK_PRECONDITION(!pumping_started_);

  pumping_started_ = true;

  try
  {
    pump_(graph, order, time_node_v, ticks);

    while (!next_update_.empty())
    {
      for (auto pos : next_update_)
        order.mark(pos);

      next_update_.clear();

      pump_(graph, order, time_node_v, ticks);
    }
  }
  catch (...)
  {
    pumping_started_ = false;
    args_buffer_.clear();
    metadata_.clear();
    throw;
  }

  pumping_started_ = false;

  CHECK_POSTCONDITION(!pumping_started_);
  CHECK_POSTCONDITION(metadata_.empty());
}

void pumpa::pump_(dependency_graph& graph,
                  topological_list& order,
                  vertex_descriptor time_node_v,
                  tick_count& ticks)
{
  ++ticks;
  changed_nodes_count_ = 0;
  updated_nodes_count_ = 0;

  CHECK_CONDITION(dynamic_cast<node_time*>(graph[time_node_v].p_node));

  const auto p_node_time = static_cast<node_time*>(graph[time_node_v].p_node);

  p_node_time->increment();

  order.mark(graph[time_node_v].position);

  const auto to = order.end_marked();
  for (auto it = order.begin_marked(); it != to; it = order.begin_marked())
  {
    const auto v = *it;

    order.unmark(it.base());

    const auto p_node = graph[v].p_node;

    CHECK_CONDITION(p_node);

    CHECK_CONDITION(args_buffer_.size() == 0);

    for (auto es = out_edges(v, graph); es.first != es.second; ++es.first)
    {
      const auto e = *es.first;

      // Only active data dependencies get to the arguments list
      if (graph[e] != active_edge_ticket())
        args_buffer_.push_back(graph[target(e, graph)].p_node);
    }

    const auto status = p_node->update(converter::convert(v),
                                       graph[v].initialized,
                                       &args_buffer_.front(),
                                       args_buffer_.size());

    ++updated_nodes_count_;

    if ((status & update_status::updated_next) != update_status::nothing)
    {
      schedule_for_next_update(graph[v].position);
    }

    if ((status & update_status::updated) != update_status::nothing)
    {
      ++changed_nodes_count_;

      for (auto u : graph[v].consumers)
      {
        order.mark(graph[u].position);
      }
    }

    graph[v].initialized = true;

    args_buffer_.clear();
  }

  CHECK_CONDITION(order.begin_marked() == order.end_marked());

  metadata_.clear();
}
} // internal
} // dataflow
