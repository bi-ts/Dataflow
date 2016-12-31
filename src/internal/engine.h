
//  Copyright 2014-2016 Maksym V. Bilinets.
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

private:
  explicit engine();
  ~engine() noexcept;

public:
  static void start();
  static void stop();

  static engine& instance();

  void pump();

  const tick_count& ticks() const
  {
    return ticks_;
  }

  const dependency_graph& graph() const;

  const topological_list& order() const;

  bool base(edge_descriptor e) const;

  bool primary(edge_descriptor e) const;

  bool secondary(edge_descriptor e) const;

  bool enabled(edge_descriptor e) const;

  bool active(edge_descriptor e) const;

  bool active(vertex_descriptor v) const;

  bool conditional(vertex_descriptor v) const;

  bool ordered(vertex_descriptor v) const;

  vertex_descriptor
  add_node(node* p_node, const node_id* p_args, std::size_t args_count);

  vertex_descriptor add_active_node(node* p_node, vertex_descriptor w);

  vertex_descriptor add_conditional_node(node* p_node,
                                         const node_id* p_args,
                                         std::size_t args_count);

  vertex_descriptor add_constant_node(node* p_node);

  void add_ref(vertex_descriptor v);
  void release(vertex_descriptor v);

  void schedule(vertex_descriptor v)
  {
    if (graph_[v].position != topological_position())
      order_.mark(graph_[v].position);

    pump();
  }

  const node* get_node(node_id id)
  {
    return graph_[converter::convert(id)].p_node;
  }

  vertex_descriptor base(vertex_descriptor v) const
  {
    assert(active(v));
    assert(out_degree(v, graph_) > 0);

    return target(*(out_edges(v, graph_).second - 1), graph_);
  }

  void reset_base(vertex_descriptor v, vertex_descriptor b)
  {
    assert(active(v));
    assert(out_degree(v, graph_) > 0);

    const auto e = *(out_edges(v, graph_).second - 1);

    assert(graph_[e] == active_edge_ticket());

    remove_edge(e, graph_);

    add_edge(v, b, graph_);
  }

  void activate_vertex(vertex_descriptor v,
                       topological_position pos,
                       vertex_descriptor b)
  {
    assert(!active(v));
    assert(*pos == v);

    graph_[v].position = pos;

    add_edge(v, b, graph_);

    assert(!graph_[v].initialized);
    assert(active(v));
  }

  void deactivate_vertex(vertex_descriptor v)
  {
    assert(active(v));
    assert(graph_[v].position != topological_position());
    assert(*graph_[v].position == v);
    assert(graph_[v].initialized);
    assert(graph_[v].consumers.size() == 0);
    assert(out_degree(v, graph_) >= 1);

    order_.erase(graph_[v].position);
    graph_[v].position = topological_position();

    remove_edge(out_edges(v, graph_).second - 1, graph_);

    graph_[v].initialized = false;

    assert(!active(v));
    assert(graph_[v].position == topological_position());
    assert(!graph_[v].initialized);
  }

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

inline engine::allocator_type engine::get_allocator() const
{
  return allocator_;
}

inline engine& engine::instance()
{
  assert(gp_engine_ != nullptr);

  return *gp_engine_;
}

inline const dependency_graph& engine::graph() const
{
  return graph_;
}

inline const topological_list& engine::order() const
{
  return order_;
}

inline bool engine::base(edge_descriptor e) const
{
  assert(e != edge_descriptor());

  const auto u = source(e, graph_);

  if (active(u))
  {
    return *(out_edges(u, graph_).second - 1) == e;
  }

  return false;
}

inline bool engine::primary(edge_descriptor e) const
{
  assert(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !base(e) &&
         (!graph_[u].conditional || *out_edges(u, graph_).first == e);
}

inline bool engine::secondary(edge_descriptor e) const
{
  assert(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !base(e) && graph_[u].conditional && *out_edges(u, graph_).first != e;
}

inline bool engine::enabled(edge_descriptor e) const
{
  assert(e != edge_descriptor());

  return graph_[e] != active_edge_ticket();
}

inline bool engine::active(edge_descriptor e) const
{
  return enabled(e) || base(e);
}

inline bool engine::active(vertex_descriptor v) const
{
  assert(v != vertex_descriptor());

  return graph_[v].position != topological_position();
}

inline bool engine::conditional(vertex_descriptor v) const
{
  assert(v != vertex_descriptor());

  return graph_[v].conditional;
}

inline bool engine::ordered(vertex_descriptor v) const
{
  assert(v != vertex_descriptor());

  const auto pos = graph_[v].position;

  return pos != topological_position() && *pos == v;
}

inline void engine::add_ref(vertex_descriptor v)
{
  graph_[v].add_ref();
}

inline void engine::release(vertex_descriptor v)
{
  if (graph_[v].release())
  {
    remove_subgraph_(v);
  }
}
}
}
