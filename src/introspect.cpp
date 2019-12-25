
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

#include <dataflow/introspect.h>

#include "prelude/core/internal/engine.h"

#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <memory>
#include <regex>
#include <type_traits>

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

std::ostream& introspect::
operator<<(std::ostream& out, const dependency_graph::vertex_descriptor& v)
{
  out << v.id_;
  return out;
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
      target(*(to - 1), dg) == converter::convert(v))
  {
    --to;
  }

  iterator_delegate::transform_function fn = [](const base_iterator& ei) {
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
    [](internal::vertex_descriptor v) {
      return false == internal::engine::instance().graph()[v].hidden;
    };

  iterator_delegate::transform_function fn = [](const base_iterator& vi) {
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
  return num_vertices(internal::engine::instance().graph());
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
    [](const internal::topological_list::const_iterator& it) {
      return converter::convert(*it);
    };

  return std::make_pair(
    dependency_graph::vertex_iterator(std::unique_ptr<iterator_delegate>(
      new iterator_delegate(order.begin(), fn))),
    dependency_graph::vertex_iterator(std::unique_ptr<iterator_delegate>(
      new iterator_delegate(order.end(), fn))));
}

introspect::dependency_graph::vertices_size_type introspect::num_active_nodes()
{
  return internal::engine::instance().order().size();
}

introspect::dependency_graph::vertices_size_type introspect::num_changed_nodes()
{
  return internal::engine::instance().changed_nodes_count();
}

introspect::dependency_graph::vertices_size_type introspect::num_updated_nodes()
{
  return internal::engine::instance().updated_nodes_count();
}

std::size_t introspect::memory_consumption()
{
  return internal::engine::allocator_type::allocated();
}

// Vertex properties

bool introspect::active_node(dependency_graph::vertex_descriptor v)
{
  if (v == boost::graph_traits<dependency_graph>::null_vertex())
    return false;

  return internal::engine::instance().is_active_node(converter::convert(v));
}

bool introspect::conditional_node(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance().is_conditional_node(
    converter::convert(v));
}

bool introspect::eager_node(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance().is_eager_node(converter::convert(v));
}

bool introspect::persistent_node(dependency_graph::vertex_descriptor v)
{
  return internal::engine::instance().is_persistent_node(converter::convert(v));
}

int introspect::update_order(dependency_graph::vertex_descriptor u,
                             dependency_graph::vertex_descriptor v)
{
  if (u == v)
    return 0;

  if (!active_node(u) && !active_node(v))
    return 0;

  if (!active_node(u))
  {
    assert(active_node(v));
    return 1;
  }

  if (!active_node(v))
  {
    assert(active_node(u));
    return -1;
  }

  assert(active_node(u) && active_node(v));

  const auto& g = internal::engine::instance().graph();

  const auto u_pos = g[converter::convert(u)].position;
  const auto v_pos = g[converter::convert(v)].position;

  assert(u_pos != v_pos);

  const int order = internal::engine::instance().order().order(u_pos, v_pos);

  return (order << 1) - 1;
}

introspect::vertex_range
introspect::consumers(dependency_graph::vertex_descriptor v)
{
  using iterator_delegate =
    iterator_delegate<internal::consumers_list::const_iterator,
                      const dependency_graph::vertex_descriptor>;

  iterator_delegate::transform_function fn =
    [](const internal::consumers_list::const_iterator& it) {
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
  if (v == dependency_graph::vertex_descriptor())
    return "";

  return internal::engine::instance()
    .graph()[converter::convert(v)]
    .p_node->label();
}

std::size_t introspect::ref_count(dependency_graph::vertex_descriptor v)
{
  if (v == dependency_graph::vertex_descriptor())
    return 0;

  return internal::engine::instance()
    .graph()[converter::convert(v)]
    .ref_count();
}

std::string introspect::value(dependency_graph::vertex_descriptor v)
{
  if (v == dependency_graph::vertex_descriptor())
    return "";

  return internal::engine::instance()
    .graph()[converter::convert(v)]
    .p_node->to_string();
}

// Data nodes properties

bool introspect::activator_node(dependency_graph::vertex_descriptor v)
{
  const auto& l = label(v);

  if (l == "state-prev")
    return true;

  // TODO: make this function independent on the label.
  const auto& ending = std::string("-activator");

  return l.length() >= ending.length() &&
         l.compare(l.length() - ending.length(), ending.length(), ending) == 0;
}

// Dependencies properties

bool introspect::active_dependency(dependency_graph::edge_descriptor e)
{
  const auto& engine = internal::engine::instance();
  const auto ie = converter::convert(e);

  return engine.is_active_data_dependency(ie) ||
         engine.is_logical_dependency(ie);
}

bool introspect::logical_dependency(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_logical_dependency(
    converter::convert(e));
}

bool introspect::primary_dependency(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_primary_data_dependency(
    converter::convert(e));
}

bool introspect::secondary_dependency(dependency_graph::edge_descriptor e)
{
  return internal::engine::instance().is_secondary_data_dependency(
    converter::convert(e));
}

void introspect::write_graphviz(std::ostream& out)
{
  assert(out);

  const auto delimiter = "\n";
  const auto indent = "\t";

  out << "digraph G {" << delimiter;
  out << indent << "edge [dir=back]" << delimiter;

  for (auto vs = vertices(); vs.first != vs.second; ++vs.first)
  {
    const auto u = *vs.first;
    out << indent << u;
    out << " [label=\"" << label(u) << " (" << value(u) << ")\"";
    if (!active_node(u))
    {
      out << ", color=lightgray";
    }
    if (conditional_node(u))
    {
    }
    out << "];" << delimiter;
  }

  for (auto vs = vertices(); vs.first != vs.second; ++vs.first)
  {
    const auto u = *vs.first;

    for (auto es = out_edges(*vs.first, graph()); es.first != es.second;
         ++es.first)
    {
      const auto e = *es.first;
      const auto v = target(e, graph());
      out << indent << u << "->" << v;

      if (logical_dependency(e))
      {
        out << " [color=gray, weight=0, style=\"dotted\"]";
      }
      else if (conditional_node(u) && primary_dependency(e))
      {
        out << " [dir=none, weight=100]";
      }
      else if (!active_dependency(e))
      {
        out << " [color=lightgray, weight=0]";
      }

      out << delimiter;
    }
  }

  out << "}" << delimiter;
}

} // dataflow
