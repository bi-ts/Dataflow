
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

#ifndef DATAFLOW___INTERNAL_PUMPA_H
#define DATAFLOW___INTERNAL_PUMPA_H

#include "converter.h"
#include "graph.h"
#include "tick_count.h"

#include <utility>
#include <vector>

namespace dataflow
{
namespace internal
{
class pumpa final
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
  pumpa(const pumpa&) = delete;
  pumpa& operator=(const pumpa&) = delete;

  static void start();
  static void stop();

  static pumpa& instance();

  bool is_logical_dependency(edge_descriptor e) const;
  bool is_primary_data_dependency(edge_descriptor e) const;
  bool is_secondary_data_dependency(edge_descriptor e) const;
  bool is_active_data_dependency(edge_descriptor e) const;

  bool is_active_node(vertex_descriptor v) const;
  bool is_conditional_node(vertex_descriptor v) const;
  bool is_eager_node(vertex_descriptor v) const;
  bool is_persistent_node(vertex_descriptor v) const;
  bool is_previous_node(vertex_descriptor v) const;

  std::size_t changed_nodes_count() const;
  std::size_t updated_nodes_count() const;

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

  void add_data_edge(vertex_descriptor u, vertex_descriptor v);

  void remove_data_edge(vertex_descriptor u, std::size_t idx);

  void schedule(vertex_descriptor v);

  void pump();

  void set_metadata(const node* p_node,
                    std::shared_ptr<const metadata> p_metadata);
  const std::shared_ptr<const metadata>& get_metadata(const node* p_node);

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

  std::pair<const node*, update_status>
  update_node_state_prev(vertex_descriptor v, bool initialized);

private:
  explicit pumpa();
  ~pumpa() noexcept;

  void schedule_for_next_update_(vertex_descriptor v);

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
  std::unordered_map<const node*, std::shared_ptr<const metadata>> metadata_;
  const std::shared_ptr<const metadata> p_no_metadata_;
  std::size_t changed_nodes_count_;
  std::size_t updated_nodes_count_;

private:
  static pumpa* gp_engine_;
};
} // internal
} // dataflow

#include "pumpa.inl"

#endif // DATAFLOW___INTERNAL_PUMPA_H
