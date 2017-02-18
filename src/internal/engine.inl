
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

inline engine& engine::instance()
{
  CHECK_PRECONDITION(gp_engine_ != nullptr);

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
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  if (active(u))
  {
    return *(out_edges(u, graph_).second - 1) == e;
  }

  return false;
}

inline bool engine::primary(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !base(e) &&
         (!graph_[u].conditional || *out_edges(u, graph_).first == e);
}

inline bool engine::secondary(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  const auto u = source(e, graph_);

  return !base(e) && graph_[u].conditional && *out_edges(u, graph_).first != e;
}

inline bool engine::enabled(edge_descriptor e) const
{
  CHECK_PRECONDITION(e != edge_descriptor());

  return graph_[e] != active_edge_ticket();
}

inline bool engine::active(edge_descriptor e) const
{
  return enabled(e) || base(e);
}

inline bool engine::active(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].position != topological_position();
}

inline bool engine::conditional(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

  return graph_[v].conditional;
}

inline bool engine::ordered(vertex_descriptor v) const
{
  CHECK_PRECONDITION(v != vertex_descriptor());

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
} // internal
} // dataflow
