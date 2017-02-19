
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

#include <dataflow/introspect.h>

#include "internal/engine.h"

#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <memory>
#include <type_traits>
#include <regex>

namespace dataflow
{

namespace introspect
{
class converter final
{
public:
  static dependency_graph::vertex_descriptor
  convert(internal::vertex_descriptor v)
  {
    return dependency_graph::vertex_descriptor(internal::converter::convert(v));
  }

  static internal::vertex_descriptor
  convert(dependency_graph::vertex_descriptor v)
  {
    return internal::converter::convert(v.id_);
  }

  static internal::edge_descriptor
  convert(const dependency_graph::edge_descriptor& e)
  {
    if (e == dependency_graph::edge_descriptor())
      return internal::edge_descriptor();

    const auto& g = internal::engine::instance().graph();
    const auto u = convert(e.u);

    assert(out_degree(u, g) > e.idx);

    return *(out_edges(u, g).first + e.idx);
  }

  static dependency_graph::edge_descriptor
  convert(const internal::dependency_graph::out_edge_iterator& ei)
  {
    const auto& g = internal::engine::instance().graph();

    const auto u = source(*ei, g);

    return dependency_graph::edge_descriptor(
      convert(u), std::distance(out_edges(u, g).first, ei));
  }
};

namespace
{

template <typename Iterator, typename T>
class iterator_delegate final : public detail::iterator_impl<T>
{
public:
  using transform_function = std::function<T(const Iterator&)>;

public:
  iterator_delegate(Iterator it, transform_function fn)
  : it_(it)
  , value_()
  , fn_(fn)
  {
  }

private:
  virtual void increment() override
  {
    ++it_;
  }

  virtual std::unique_ptr<detail::iterator_impl<T>> clone() override
  {
    return std::unique_ptr<detail::iterator_impl<T>>(
      new iterator_delegate(it_, fn_));
  }

  virtual T& value() override
  {
    value_ = fn_(it_);
    return value_;
  }

  virtual bool equal(const detail::iterator_impl<T>& other) override
  {
    const auto p_other = dynamic_cast<const iterator_delegate*>(&other);
    return p_other && it_ == p_other->it_;
  }

private:
  Iterator it_;
  typename std::remove_const<T>::type value_;
  transform_function fn_;
};
} // anonymous
} // introspect

introspect::dependency_graph::dependency_graph()
{
}

introspect::dependency_graph::vertex_descriptor
introspect::dependency_graph::null_vertex()
{
  return dependency_graph::vertex_descriptor();
}

const introspect::dependency_graph& introspect::graph()
{
  static dependency_graph g_graph;

  return g_graph;
}

// IncidenceGraph

introspect::out_edge_range
introspect::out_edges(dependency_graph::vertex_descriptor v,
                      const dependency_graph& g)
{
  using base_iterator =
    boost::graph_traits<internal::dependency_graph>::out_edge_iterator;

  base_iterator from, to;
  std::tie(from, to) =
    out_edges(converter::convert(v), internal::engine::instance().graph());

  using iterator_delegate =
    iterator_delegate<base_iterator, const dependency_graph::edge_descriptor>;

  const auto& dg = internal::engine::instance().graph();

  if (out_degree(converter::convert(v), dg) > 0 &&
      dg[target(*(to - 1), dg)].hidden)
  {
    --to;
  }

  iterator_delegate::transform_function fn = [](const base_iterator& ei)
  {
    return converter::convert(ei);
  };

  return std::make_pair(
    dependency_graph::out_edge_iterator(
      std::unique_ptr<iterator_delegate>(new iterator_delegate(from, fn))),
    dependency_graph::out_edge_iterator(
      std::unique_ptr<iterator_delegate>(new iterator_delegate(to, fn))));
}

introspect::dependency_graph::degree_size_type
introspect::out_degree(dependency_graph::vertex_descriptor v,
                       const dependency_graph& g)
{
  return out_degree(converter::convert(v),
                    internal::engine::instance().graph());
}

// VertexListGraph

introspect::vertex_range introspect::vertices(const dependency_graph& g)
{
  const auto vs = vertices(internal::engine::instance().graph());

  using base_iterator = boost::filter_iterator<
    std::function<bool(internal::vertex_descriptor)>,
    boost::graph_traits<internal::dependency_graph>::vertex_iterator>;

  using iterator_delegate =
    iterator_delegate<base_iterator, const dependency_graph::vertex_descriptor>;

  std::function<bool(internal::vertex_descriptor)> predicate =
    [](internal::vertex_descriptor v)
  {
    return false == internal::engine::instance().graph()[v].hidden;
  };

  iterator_delegate::transform_function fn = [](const base_iterator& vi)
  {
    return converter::convert(*vi);
  };

  return std::make_pair(
    dependency_graph::vertex_iterator(
      std::unique_ptr<iterator_delegate>(new iterator_delegate(
        boost::make_filter_iterator(predicate, vs.first, vs.second), fn))),
    dependency_graph::vertex_iterator(
      std::unique_ptr<iterator_delegate>(new iterator_delegate(
        boost::make_filter_iterator(predicate, vs.second, vs.second), fn))));
}

introspect::dependency_graph::vertices_size_type
introspect::num_vertices(const dependency_graph& g)
{
  return num_vertices(internal::engine::instance().graph()) - 1;
}

// edge_descriptor

introspect::dependency_graph::edge_descriptor::edge_descriptor()
: u()
, idx()
{
}

introspect::dependency_graph::edge_descriptor::edge_descriptor(
  vertex_descriptor u, std::size_t idx)
: u(u)
, idx(idx)
{
}

introspect::dependency_graph::vertex_descriptor
introspect::source(dependency_graph::edge_descriptor e,
                   const dependency_graph& g)
{
  return converter::convert(
    source(converter::convert(e), internal::engine::instance().graph()));
}

introspect::dependency_graph::vertex_descriptor
introspect::target(dependency_graph::edge_descriptor e,
                   const dependency_graph& g)
{
  return converter::convert(
    target(converter::convert(e), internal::engine::instance().graph()));
}

// Graph properties

introspect::vertex_range introspect::topological_order()
{
  using iterator_delegate =
    iterator_delegate<internal::topological_list::const_iterator,
                      const dependency_graph::vertex_descriptor>;

  const auto& order = internal::engine::instance().order();

  assert(order.size() > 0);
  // assert(order.front() label is "ground");

  iterator_delegate::transform_function fn =
    [](const internal::topological_list::const_iterator& it)
  {
    return converter::convert(*it);
  };

  return std::make_pair(
    dependency_graph::vertex_iterator(std::unique_ptr<iterator_delegate>(
      new iterator_delegate(++order.begin(), fn))),
    dependency_graph::vertex_iterator(std::unique_ptr<iterator_delegate>(
      new iterator_delegate(order.end(), fn))));
}

introspect::dependency_graph::vertices_size_type
introspect::num_ordered_vertices()
{
  return internal::engine::instance().order().size() - 1;
}

std::size_t introspect::memory_consumption()
{
  return internal::engine::allocator_type::allocated();
}

// Vertex properties

bool introspect::is_node_active(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance().is_active_node(converter::convert(v));
}

bool introspect::conditional(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance().is_conditional_node(
    converter::convert(v));
}

bool introspect::constant(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance().graph()[converter::convert(v)].constant;
}

introspect::vertex_range
introspect::consumers(dependency_graph::vertex_descriptor v)
{
  using iterator_delegate =
    iterator_delegate<internal::consumers_list::const_iterator,
                      const dependency_graph::vertex_descriptor>;

  iterator_delegate::transform_function fn =
    [](const internal::consumers_list::const_iterator& it)
  {
    return converter::convert(*it);
  };

  const auto& consumers =
    internal::engine::instance().graph()[converter::convert(v)].consumers;

  return std::make_pair(
    dependency_graph::vertex_iterator(std::unique_ptr<iterator_delegate>(
      new iterator_delegate(consumers.begin(), fn))),
    dependency_graph::vertex_iterator(std::unique_ptr<iterator_delegate>(
      new iterator_delegate(consumers.end(), fn))));
}

std::string introspect::label(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance()
    .graph()[converter::convert(v)]
    .p_node->label();
}

bool introspect::varying(dependency_graph::vertex_descriptor v)
{
  return !constant(v) && is_node_active(v);
}

bool introspect::preceedes(dependency_graph::vertex_descriptor u,
                           dependency_graph::vertex_descriptor v)
{
  const auto null_vertex = dependency_graph::vertex_descriptor();

  if (u == v || v == null_vertex)
    return false;

  if (u == null_vertex)
    return true;

  assert(u != null_vertex);
  assert(v != null_vertex);

  const auto& g = internal::engine::instance().graph();

  return internal::engine::instance().order().order(
    g[converter::convert(u)].position, g[converter::convert(v)].position);
}

std::size_t introspect::ref_count(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance()
    .graph()[converter::convert(v)]
    .ref_count();
}

std::string introspect::value(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance()
    .graph()[converter::convert(v)]
    .p_node->to_string();
}

// Edge properties

bool introspect::is_dependency_active(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_active_data_dependency(
           converter::convert(e)) ||
         is_dependency_logical(e);
}

bool introspect::is_dependency_logical(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_logical_dependency(
    converter::convert(e));
}

bool introspect::is_dependency_primary(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_primary_data_dependency(
    converter::convert(e));
}

bool introspect::is_dependency_secondary(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_secondary_data_dependency(
    converter::convert(e));
}
} // dataflow
