
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

#if !defined(DATAFLOW___INTERNAL_ENGINE_H)
#error '.inl' file can't be included directly. Use 'engine.h' instead
#endif

#include "config.h"

namespace dataflow
{
namespace internal
{
inline engine::allocator_type engine::get_allocator() const
{
  return allocator_;
}

inline const dependency_graph& engine::graph() const
{
  return graph_;
}

inline const topological_list& engine::order() const
{
  return order_;
}

inline const tick_count& engine::ticks() const
{
  return ticks_;
}

inline const node* engine::get_node(vertex_descriptor v) const
{
  return graph_[v].p_node;
}

inline engine& engine::instance()
{
  CHECK_PRECONDITION(gp_engine_ != nullptr);

  return *gp_engine_;
}

inline bool engine::is_logical_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  if (is_active_node(u))
  {
    return last_out_edge_(u) == e;
  }

  return false;
}

inline bool engine::is_primary_data_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !is_logical_dependency(e) &&
         (!is_conditional_node(u) || first_out_edge_(u) == e);
}

inline bool engine::is_secondary_data_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !is_logical_dependency(e) && is_conditional_node(u) &&
         first_out_edge_(u) != e;
}

inline bool engine::is_active_data_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  return graph_[e] != active_edge_ticket();
}

inline bool engine::is_active_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  const auto position = graph_[v].position;

  return position != topological_position() && *position == v;
}

inline bool engine::is_conditional_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].conditional;
}

inline bool engine::is_eager_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].eager;
}

inline bool engine::is_persistent_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].constant;
}

inline bool engine::requires_activation(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].position == topological_position() && !graph_[v].constant;
}

inline void engine::add_ref(vertex_descriptor v)
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  graph_[v].add_ref();
}

inline void engine::release(vertex_descriptor v)
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  if (graph_[v].release())
  {
    remove_subgraph_(v);
  }
}

inline edge_descriptor engine::first_out_edge_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(out_degree(v, graph_) >= 1);

  auto ei = out_edges(v, graph_).first;

  return *ei;
}

inline edge_descriptor engine::second_out_edge_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(out_degree(v, graph_) >= 2);

  auto ei = out_edges(v, graph_).first;
  std::advance(ei, 1);

  return *ei;
}

inline edge_descriptor engine::last_out_edge_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(out_degree(v, graph_) >= 1);

  return *(out_edges(v, graph_).second - 1);
}

inline vertex_descriptor engine::activator_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(is_active_node(v));

  return target(last_out_edge_(v), graph_);
}

} // internal
} // dataflow
