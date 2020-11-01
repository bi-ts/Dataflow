
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#ifndef DATAFLOW___INTROSPECT_H
#define DATAFLOW___INTROSPECT_H

#include "dataflow++_export.h"

#include <dataflow/prelude/core.h>

#ifdef DATAFLOW_CONFIG_HEADER_INTROSPECT_BOOST_GRAPH_COMPATIBLE
#include <boost/graph/graph_traits.hpp>
#endif

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace dataflow
{
namespace introspect
{

/// \defgroup introspect
/// \{

namespace detail
{
template <typename T> class iterator_impl
{
public:
  virtual ~iterator_impl() = default;
  virtual void increment() = 0;
  virtual std::unique_ptr<iterator_impl> clone() = 0;
  virtual T& value() = 0;
  virtual bool equal(const iterator_impl& other) = 0;

protected:
  iterator_impl() = default;
};
}

template <typename T>
class graph_iterator final : public std::iterator<std::forward_iterator_tag, T>
{
public:
  graph_iterator();
  graph_iterator(std::unique_ptr<detail::iterator_impl<T>>&& p_delegate);
  graph_iterator(const graph_iterator& other);

  graph_iterator& operator=(const graph_iterator& other);

  bool operator==(const graph_iterator& other) const;
  bool operator!=(const graph_iterator& other) const;

  graph_iterator& operator++();
  graph_iterator operator++(int);

  typename graph_iterator::reference operator*() const;

private:
  std::unique_ptr<detail::iterator_impl<T>> p_impl_;
};

class DATAFLOW___EXPORT dependency_graph
{
  DATAFLOW___EXPORT friend const dependency_graph& graph();

public:
  // Graph
  class DATAFLOW___EXPORT vertex_descriptor
  {
    friend class converter;
    DATAFLOW___EXPORT friend std::ostream&
    operator<<(std::ostream& out, const vertex_descriptor& v);

  public:
    vertex_descriptor();

    vertex_descriptor(const internal::ref& x);

    vertex_descriptor(internal::node_id id);

    bool operator==(vertex_descriptor other) const;
    bool operator!=(vertex_descriptor other) const;
    bool operator<(const vertex_descriptor& other) const;

  private:
    internal::node_id id_;
  };

  class DATAFLOW___EXPORT edge_descriptor final
  {
    friend class converter;

  public:
    edge_descriptor();

    bool operator==(const edge_descriptor& other) const;
    bool operator!=(const edge_descriptor& other) const;

  private:
    edge_descriptor(vertex_descriptor u, std::size_t idx);

  private:
    vertex_descriptor u;
    std::size_t idx;
  };

#ifdef DATAFLOW_CONFIG_HEADER_INTROSPECT_BOOST_GRAPH_COMPATIBLE
  using directed_category = boost::directed_tag;
  using edge_parallel_category = boost::allow_parallel_edge_tag;
  class traversal_category : public boost::incidence_graph_tag,
                             public boost::vertex_list_graph_tag
  {
  };
#endif

  // IncidenceGraph
  using out_edge_iterator = graph_iterator<const edge_descriptor>;
  using degree_size_type = std::size_t;

  // VertexListGraph
  using vertex_iterator = graph_iterator<const vertex_descriptor>;
  using vertices_size_type = std::size_t;

public:
  static vertex_descriptor null_vertex();

private:
  dependency_graph();
  dependency_graph(const dependency_graph&) = delete;
};

using out_edge_range = std::pair<dependency_graph::out_edge_iterator,
                                 dependency_graph::out_edge_iterator>;

using vertex_range = std::pair<dependency_graph::vertex_iterator,
                               dependency_graph::vertex_iterator>;

DATAFLOW___EXPORT const dependency_graph& graph();

DATAFLOW___EXPORT std::ostream&
operator<<(std::ostream& out, const dependency_graph::vertex_descriptor& v);

// IncidenceGraph
DATAFLOW___EXPORT out_edge_range out_edges(
  dependency_graph::vertex_descriptor v, const dependency_graph& g = graph());

DATAFLOW___EXPORT dependency_graph::degree_size_type
out_degree(dependency_graph::vertex_descriptor v,
           const dependency_graph& g = graph());

// VertexListGraph
DATAFLOW___EXPORT
vertex_range vertices(const dependency_graph& g = graph());

DATAFLOW___EXPORT dependency_graph::vertices_size_type
num_vertices(const dependency_graph& g = graph());

// edge_descriptor
DATAFLOW___EXPORT dependency_graph::vertex_descriptor
source(dependency_graph::edge_descriptor e, const dependency_graph& g);

DATAFLOW___EXPORT dependency_graph::vertex_descriptor
target(dependency_graph::edge_descriptor e, const dependency_graph& g);

// Graph properties
DATAFLOW___EXPORT
vertex_range topological_order();

DATAFLOW___EXPORT dependency_graph::vertices_size_type num_active_nodes();

DATAFLOW___EXPORT dependency_graph::vertices_size_type num_changed_nodes();

DATAFLOW___EXPORT dependency_graph::vertices_size_type num_updated_nodes();

DATAFLOW___EXPORT std::size_t memory_consumption();

/// \name Data nodes properties
/// \{

/// \note `null_vertex()` is considered inactive.
DATAFLOW___EXPORT bool active_node(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT bool activator_node(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT bool conditional_node(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT bool eager_node(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT bool persistent_node(dependency_graph::vertex_descriptor v);

/// Gets the order of update for two data nodes.
///
/// That is, the order in which these two nodes, both being marked as outdated,
/// will be updated.
///
/// Inactive nodes are considered to be up-to-date and never need to be updated,
/// which means that all active nodes are updated after the inactive.
///
/// Since inactive nodes are never updated, any two inactive nodes are
/// considered to be updated concurrently.
///
/// If `v` has active (direct or indirect) dependency on `u` (`u` &larr; `v`),
/// `u` is updated before `v`.
///
/// If `u` has active (direct or indirect) dependency on `v` (`u` &rarr; `v`),
/// `u` is updated after `v`.
///
/// If `u` and `v` are independent, the order of update is implementation
/// defined.
///
/// \param u, v Two data nodes, represented by corresponding vertices of the
///             dependency graph
///
/// \returns
///       value | condition
///       ------|----------------------------------------------------
///         +1  | if `u` is updated before `v`
///          0  | if `u = v` or both nodes are updated concurrently
///         -1  | if `u` is updated after `v`
///
/// \see active_node()
/// \see active_dependency()
DATAFLOW___EXPORT int update_order(dependency_graph::vertex_descriptor u,
                                   dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT
vertex_range consumers(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT std::string label(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT std::size_t ref_count(dependency_graph::vertex_descriptor v);

DATAFLOW___EXPORT std::string value(dependency_graph::vertex_descriptor v);

/// \}

/// \name Dependencies properties
/// \{

DATAFLOW___EXPORT bool active_dependency(dependency_graph::edge_descriptor e);

/// Checks if the given edge represents a _logical_ dependency.
///
/// A logical dependency links an active node with its activator.
///
DATAFLOW___EXPORT bool logical_dependency(dependency_graph::edge_descriptor e);

DATAFLOW___EXPORT bool data_dependency(dependency_graph::edge_descriptor e);

DATAFLOW___EXPORT bool primary_dependency(dependency_graph::edge_descriptor e);

DATAFLOW___EXPORT bool
secondary_dependency(dependency_graph::edge_descriptor e);

/// \}

template <typename T>
ref<T> Log(const ref<T>& x, const std::string& label = "");

DATAFLOW___EXPORT integer current_time();

void DATAFLOW___EXPORT write_graphviz(std::ostream& out);

void DATAFLOW___EXPORT write_graphviz();

} // introspect
} // dataflow

#include "introspect.inl"

#endif // DATAFLOW___INTROSPECT_H
