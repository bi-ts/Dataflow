
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

#include "config.h"

#include <dataflow/prelude/core/internal/node.h>

#include <dst/allocator/global_counter_allocator.h>
#include <dst/binary_tree/list.h>
#include <dst/binary_tree/mixin/avl.h>
#include <dst/binary_tree/mixin/marking.h>
#include <dst/binary_tree/mixin/ordering.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

// #define DATAFLOW___EXPERIMENTAL_BUILD_WITH_BOOST_POOL_ALLOCATOR
#ifdef DATAFLOW___EXPERIMENTAL_BUILD_WITH_BOOST_POOL_ALLOCATOR
#define BOOST_POOL_NO_MT       // disable multi-threading
#define BOOST_THREAD_MUTEX_HPP // disable <boost/thread/mutex.hpp> header
#include <boost/pool/pool_alloc.hpp>
#endif

#include <cstddef>
#include <limits>
#include <list>
#include <type_traits>

#ifndef NDEBUG
#include <unordered_set>
#endif

namespace dataflow
{
namespace internal
{
template <typename Allocator> struct out_edge_listS
{
};

template <typename Allocator> struct vertex_listS
{
};
} // internal
} // dataflow

namespace boost
{
template <typename Allocator, typename T>
struct container_gen<dataflow::internal::out_edge_listS<Allocator>, T>
{
  using type = std::vector<
    T,
    typename std::allocator_traits<Allocator>::template rebind_alloc<T>>;
};

template <typename Allocator>
struct parallel_edge_traits<dataflow::internal::out_edge_listS<Allocator>>
: public parallel_edge_traits<boost::vecS>
{
};

template <typename Allocator, typename T>
struct container_gen<dataflow::internal::vertex_listS<Allocator>, T>
{
  using type = std::list<
    T,
    typename std::allocator_traits<Allocator>::template rebind_alloc<T>>;
};
} // boost

namespace dataflow
{
namespace internal
{
using vertex_descriptor = void*;

template <typename T> using memory_allocator = dst::global_counter_allocator<T>;

#ifdef DATAFLOW___EXPERIMENTAL_BUILD_WITH_BOOST_POOL_ALLOCATOR
template <typename T>
using list_element_allocator = dst::global_counter_allocator<
  T,
  boost::fast_pool_allocator<T,
                             boost::default_user_allocator_new_delete,
                             boost::details::pool::null_mutex,
                             1024 * 1024>>;
#else
template <typename T> using list_element_allocator = memory_allocator<T>;
#endif

using topological_list =
  dst::binary_tree::list<vertex_descriptor,
                         list_element_allocator<vertex_descriptor>,
                         dst::binary_tree::Marking<>,
                         dst::binary_tree::AVL,
                         dst::binary_tree::Ordering>;

using topological_position = topological_list::const_iterator;

using consumers_list =
  std::list<vertex_descriptor, memory_allocator<vertex_descriptor>>;

using active_edge_ticket = consumers_list::const_iterator;

class vertex final
{
private:
  using uint = std::size_t;

public:
  vertex(node* p_node)
  : eager(false)
  , conditional(false)
  , constant(false)
  , straight(false)
  , initialized(false)
  , hidden(false)
  , ref_count_(0)
  , position()
  , p_node(p_node)
  {
    assert(p_node);

    const auto expected_size = sizeof(uint) +          // Flags
                               sizeof(uint) +          // Reference counter
                               sizeof(void*) +         // Topological position
                               sizeof(void*) +         // Node pointer
                               sizeof(consumers_list); // Consumers list

    static_assert(sizeof(vertex) == expected_size,
                  "Vertex size must be kept small");
  }

  uint ref_count() const
  {
    return ref_count_;
  }

  uint add_ref()
  {
    assert(ref_count_ < std::numeric_limits<uint>::max());
    return ++ref_count_;
  }

  bool release()
  {
    assert(ref_count_ > 0);
    return --ref_count_ == 0;
  }

public:
  uint eager : 1;
  uint conditional : 1;
  uint constant : 1;
  const uint straight : 1; // TODO: not used?
  uint initialized : 1;
  const uint hidden : 1; // TODO: not used?

private:
  uint ref_count_;

public:
  topological_position position;
  node* const p_node;
  consumers_list consumers;
};

using dependency_graph_base =
  boost::adjacency_list<out_edge_listS<memory_allocator<void>>,
                        vertex_listS<memory_allocator<void>>,
                        boost::directedS,
                        vertex,
                        active_edge_ticket,
                        boost::no_property>;

// TODO: reimplement `dependency_graph` class to be able to identify invalid
//       vertices in constant time
class dependency_graph : public dependency_graph_base
{
public:
  vertex& operator[](const vertex_descriptor& v)
  {
    CHECK_CONDITION_DEBUG(vertex_registry_.find(v) != vertex_registry_.end());

    dependency_graph_base& g = *this;

    return g[v];
  }

  const vertex& operator[](const vertex_descriptor& v) const
  {
    CHECK_CONDITION_DEBUG(vertex_registry_.find(v) != vertex_registry_.end());

    const dependency_graph_base& g = *this;

    return g[v];
  }

  dependency_graph_base::edge_property_type&
  operator[](const dependency_graph_base::edge_descriptor& e)
  {
    dependency_graph_base& g = *this;

    return g[e];
  }

  const dependency_graph_base::edge_property_type&
  operator[](const dependency_graph_base::edge_descriptor& e) const
  {
    const dependency_graph_base& g = *this;

    return g[e];
  }

  friend vertex_descriptor add_vertex(vertex properties, dependency_graph& g)
  {
    dependency_graph_base& base_g = g;

    const auto v = add_vertex(properties, base_g);

    CHECK_CONDITION_DEBUG(g.vertex_registry_.insert(v).second);

    return v;
  }

  friend void remove_vertex(vertex_descriptor v, dependency_graph& g)
  {
    CHECK_CONDITION_DEBUG(g.vertex_registry_.erase(v) == 1);

    dependency_graph_base& base_g = g;

    remove_vertex(v, base_g);
  }

private:
#ifndef NDEBUG
  std::unordered_set<vertex_descriptor> vertex_registry_;
#endif
};

static_assert(
  std::is_same<vertex_descriptor,
               boost::graph_traits<dependency_graph>::vertex_descriptor>::value,
  "wrong vertex_descriptor type");

using edge_descriptor = boost::graph_traits<dependency_graph>::edge_descriptor;

} // internal
} // dataflow
