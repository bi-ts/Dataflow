
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

#pragma once

#ifndef DATAFLOW___INTERNAL_ENGINE_H
#define DATAFLOW___INTERNAL_ENGINE_H

#include "converter.h"
#include "graph.h"
#include "tick_count.h"

#include <vector>

namespace dataflow
{
namespace internal
{
class engine final
{
public:
  using allocator_type = memory_allocator<char>;

public:
  allocator_type get_allocator() const;

  const dependency_graph& graph() const;

  const topological_list& order() const;

  const tick_count& ticks() const;

  const node* get_node(vertex_descriptor v) const;

public:
  engine(const engine&) = delete;
  engine& operator=(const engine&) = delete;

  static void start();
  static void stop();

  static engine& instance();

  bool is_logical_dependency(edge_descriptor e) const;
  bool is_primary_data_dependency(edge_descriptor e) const;
  bool is_secondary_data_dependency(edge_descriptor e) const;
  bool is_active_data_dependency(edge_descriptor e) const;

  bool is_active_node(vertex_descriptor v) const;
  bool is_conditional_node(vertex_descriptor v) const;
  bool is_eager_node(vertex_descriptor v) const;
  bool is_persistent_node(vertex_descriptor v) const;
  bool is_previous_node(vertex_descriptor v) const;

  bool requires_activation(vertex_descriptor v) const;

  void add_ref(vertex_descriptor v);
  void release(vertex_descriptor v);

  vertex_descriptor get_time_node() const;

  vertex_descriptor add_node(node* p_node,
                             const node_id* p_args,
                             std::size_t args_count,
                             bool eager,
                             bool conditional = false,
                             bool previous = false);

  vertex_descriptor add_persistent_node(node* p_node);

  void schedule(vertex_descriptor v);

  void schedule_for_next_update(vertex_descriptor v);

  void pump();

  update_status update_node_if_activator(vertex_descriptor v,
                                         bool initialized,
                                         std::size_t new_value,
                                         std::size_t old_value);

  update_status update_node_selector_activator(vertex_descriptor v,
                                               vertex_descriptor x,
                                               bool initialized);

  update_status update_node_snapshot_activator(vertex_descriptor v,
                                               bool initialized);

  void update_node_state(vertex_descriptor v);

  const node* update_node_state_prev(vertex_descriptor v, bool initialized);

private:
  explicit engine();
  ~engine() noexcept;

  edge_descriptor out_edge_at_(vertex_descriptor v, std::size_t idx) const;
  edge_descriptor last_out_edge_(vertex_descriptor v) const;

  vertex_descriptor main_consumer_(vertex_descriptor v) const;

  vertex_descriptor activator_(vertex_descriptor v) const;

  vertex_descriptor implied_activator_(vertex_descriptor u,
                                       vertex_descriptor v) const;

  edge_descriptor add_logical_edge_(vertex_descriptor u, vertex_descriptor v);
  edge_descriptor add_data_edge_(vertex_descriptor u, vertex_descriptor v);

  void delete_node_(vertex_descriptor v);

  void activate_vertex_(vertex_descriptor v,
                        topological_position pos,
                        vertex_descriptor w);

  void deactivate_vertex_partially_(vertex_descriptor v);

  void deactivate_vertex_(vertex_descriptor v);

  void reset_activator_(vertex_descriptor v, vertex_descriptor w);

  void activate_edge_(edge_descriptor e);

  void deactivate_edge_(edge_descriptor e);

  void activate_subgraph_(edge_descriptor e);

  void deactivate_subgraph_(edge_descriptor e);

  void remove_subgraph_(vertex_descriptor v);

  void pump_();

private:
  allocator_type allocator_;
  dependency_graph graph_;
  topological_list order_;
  std::vector<topological_position, memory_allocator<topological_position>>
    next_update_;
  bool pumping_started_;
  std::vector<const node*, memory_allocator<const node*>> args_buffer_;
  tick_count ticks_;
  vertex_descriptor time_node_v_;

private:
  static engine* gp_engine_;
};
} // internal
} // dataflow

#include "engine.inl"

#endif // DATAFLOW___INTERNAL_ENGINE_H
