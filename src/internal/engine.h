
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

  const node* get_node(node_id id) const;

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

  bool requires_activation(vertex_descriptor v) const;

  void add_ref(vertex_descriptor v);
  void release(vertex_descriptor v);

  vertex_descriptor
  add_node(node* p_node, const node_id* p_args, std::size_t args_count);

  vertex_descriptor add_active_node(node* p_node, vertex_descriptor w);

  vertex_descriptor add_conditional_node(node* p_node,
                                         const node_id* p_args,
                                         std::size_t args_count);

  vertex_descriptor add_persistent_node(node* p_node);

  void pump(vertex_descriptor v);

  // MARKER
public:
  void enable_edge(edge_descriptor e);

  void disable_edge(edge_descriptor e);

  void enable_edge_(edge_descriptor e);
  void disable_edge_(edge_descriptor e);

  void delete_node(vertex_descriptor v)
  {
    assert(graph_[v].p_node);

    const auto p_node = graph_[v].p_node;

    const auto info = p_node->mem_info();

    p_node->~node();

    dst::memory::deallocate_aligned(
      get_allocator(), p_node, info.first, info.second);

    remove_vertex(v, graph_);
  }

  bool update_node_selector(node_id id,
                            bool initialized,
                            std::size_t new_value,
                            std::size_t old_value)
  {
    const auto v = converter::convert(id);

    assert(graph_[v].consumers.size() == 1);

    const auto w = graph_[v].consumers.front();

    assert(w != vertex_descriptor());
    assert(graph_[w].conditional);

    if (!initialized)
    {
      const auto e = *(out_edges(w, graph_).first + 1 + new_value);

      enable_edge(e);

      return true;
    }
    else
    {
      if (new_value != old_value)
      {
        const auto e_prev = *(out_edges(w, graph_).first + 1 + old_value);
        const auto e_curr = *(out_edges(w, graph_).first + 1 + new_value);

        disable_edge(e_prev);

        enable_edge(e_curr);

        return true;
      }
    }

    return false;
  }

  void remove_subgraph_(vertex_descriptor v);

private:
  explicit engine();
  ~engine() noexcept;

  edge_descriptor first_out_edge_(vertex_descriptor v) const;
  edge_descriptor second_out_edge_(vertex_descriptor v) const;
  edge_descriptor last_out_edge_(vertex_descriptor v) const;

  vertex_descriptor activator_(vertex_descriptor v) const;

  vertex_descriptor implied_activator_(vertex_descriptor u,
                                       vertex_descriptor v) const;

  void activate_vertex_(vertex_descriptor v,
                        topological_position pos,
                        vertex_descriptor w);

  void deactivate_vertex_(vertex_descriptor v);

  void reset_activator_(vertex_descriptor v, vertex_descriptor w);

private:
  void pump_();

private:
  allocator_type allocator_;
  dependency_graph graph_;
  topological_list order_;
  bool pumping_started_;
  std::vector<const node*, memory_allocator<const node*>> args_buffer_;
  tick_count ticks_;

private:
  static engine* gp_engine_;
};
} // internal
} // dataflow

#include "engine.inl"

#endif // DATAFLOW___INTERNAL_ENGINE_H
