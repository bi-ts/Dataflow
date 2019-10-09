
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

#if !defined(DATAFLOW___INTERNAL_PUMPA_H)
#error '.inl' file can't be included directly. Use 'pumpa.h' instead
#endif

#include "config.h"

namespace dataflow
{
namespace internal
{
inline pumpa::allocator_type pumpa::get_allocator() const
{
  return allocator_;
}

inline const dependency_graph& pumpa::graph() const
{
  return graph_;
}

inline const topological_list& pumpa::order() const
{
  return order_;
}

inline const tick_count& pumpa::ticks() const
{
  return ticks_;
}

inline const node* pumpa::get_node(vertex_descriptor v) const
{
  return graph_[v].p_node;
}

inline pumpa& pumpa::instance()
{
  CHECK_PRECONDITION(gp_pumpa_ != nullptr);

  return *gp_pumpa_;
}

inline bool pumpa::is_logical_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  if (is_active_node(u))
  {
    return last_out_edge_(u) == e;
  }

  return false;
}

inline bool pumpa::is_primary_data_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !is_logical_dependency(e) &&
         (!is_conditional_node(u) || out_edge_at_(u, 0) == e);
}

inline bool pumpa::is_secondary_data_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !is_logical_dependency(e) && is_conditional_node(u) &&
         out_edge_at_(u, 0) != e;
}

inline bool pumpa::is_active_data_dependency(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  return graph_[e] != active_edge_ticket();
}

inline bool pumpa::is_active_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  const auto position = graph_[v].position;

  return position != topological_position() && *position == v;
}

inline bool pumpa::is_conditional_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].conditional;
}

inline bool pumpa::is_eager_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].eager;
}

inline bool pumpa::is_persistent_node(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].constant;
}

inline std::size_t pumpa::changed_nodes_count() const
{
  CHECK_PRECONDITION(!pumping_started_);

  return changed_nodes_count_;
}

inline std::size_t pumpa::updated_nodes_count() const
{
  CHECK_PRECONDITION(!pumping_started_);

  return updated_nodes_count_;
}

inline bool pumpa::requires_activation(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].position == topological_position() && !graph_[v].constant;
}

inline void pumpa::add_ref(vertex_descriptor v)
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  graph_[v].add_ref();
}

inline void pumpa::release(vertex_descriptor v)
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  if (graph_[v].release())
  {
    remove_subgraph_(v);
  }
}

inline edge_descriptor pumpa::out_edge_at_(vertex_descriptor v,
                                           std::size_t idx) const
{
  CHECK_PRECONDITION(out_degree(v, graph_) > idx);

  auto ei = out_edges(v, graph_).first;

  std::advance(ei, idx);

  return *ei;
}

inline edge_descriptor pumpa::last_out_edge_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(out_degree(v, graph_) >= 1);

  return *(out_edges(v, graph_).second - 1);
}

inline vertex_descriptor pumpa::main_consumer_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(!graph_[v].consumers.empty());

  const auto u = graph_[v].consumers.back();

  CHECK_POSTCONDITION(is_conditional_node(u));

  return u;
}

inline vertex_descriptor pumpa::activator_(vertex_descriptor v) const
{
  CHECK_PRECONDITION(is_active_node(v));

  return target(last_out_edge_(v), graph_);
}

} // internal
} // dataflow
