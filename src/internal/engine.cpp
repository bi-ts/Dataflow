
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

#include "engine.h"
#include "config.h"
#include "node_time.h"
#include "utility.h"

#include <dst/allocator/utility.h>

#include <cstdint> // std::intptr_t
#include <stack>

namespace dataflow
{
namespace internal
{
engine* engine::gp_engine_ = nullptr;

void engine::start()
{
  CHECK_PRECONDITION(gp_engine_ == nullptr);

  gp_engine_ = new engine();

  const auto p_node = static_cast<node_time*>(dst::memory::allocate_aligned(
    gp_engine_->get_allocator(), sizeof(node_time), alignof(node_time)));

  new (std::addressof(*p_node)) node_time();

  const auto v = add_vertex(vertex(p_node), gp_engine_->graph_);

  gp_engine_->graph_[v].eager = true;
  gp_engine_->graph_[v].initialized = true;

  gp_engine_->add_logical_edge_(v, v);

  gp_engine_->graph_[v].position =
    gp_engine_->order_.insert(gp_engine_->order_.end(), v);

  gp_engine_->add_ref(v);

  gp_engine_->time_node_v_ = v;
}

void engine::stop()
{
  CHECK_PRECONDITION(gp_engine_ != nullptr);

  delete gp_engine_;

  gp_engine_ = nullptr;
}

vertex_descriptor engine::get_time_node() const
{
  return time_node_v_;
}

vertex_descriptor engine::add_node(node* p_node,
                                   const node_id* p_args,
                                   std::size_t args_count,
                                   bool eager,
                                   bool conditional)
{
  CHECK_PRECONDITION(p_node != nullptr);

  const auto v = add_vertex(vertex(p_node), graph_);

  for (std::size_t i = 0; i < args_count; ++i)
  {
    const auto w = converter::convert(p_args[i]);

    add_data_edge_(v, w);
  }

  graph_[v].conditional = conditional;

  if (eager)
  {
    graph_[v].eager = true;
    graph_[v].position = order_.insert(order_.end(), v);

    schedule(v);

    add_logical_edge_(v, order_.front());

    auto es = out_edges(v, graph_);

    CHECK_CONDITION(es.first != es.second);

    --es.second;

    for (; es.first != es.second; ++es.first)
    {
      activate_subgraph_(*es.first);

      if (conditional)
        break;
    }
  }

  CHECK_POSTCONDITION(is_active_node(v) == eager);
  CHECK_POSTCONDITION(is_conditional_node(v) == conditional);

  return v;
}

vertex_descriptor engine::add_persistent_node(node* p_node)
{
  CHECK_PRECONDITION(p_node != nullptr);

  const auto v = add_vertex(vertex(p_node), graph_);

  graph_[v].constant = true;

  graph_[v].position = order_.begin();

  CHECK_POSTCONDITION(is_persistent_node(v));

  return v;
}

void engine::schedule(vertex_descriptor v)
{
  if (graph_[v].position != topological_position())
    order_.mark(graph_[v].position);
}

void engine::schedule_for_next_update(vertex_descriptor v)
{
  CHECK_PRECONDITION(pumping_started_);
  CHECK_PRECONDITION(v != vertex_descriptor());

  if (graph_[v].position != topological_position())
    next_update_.push_back(graph_[v].position);
}

void engine::pump()
{
  CHECK_PRECONDITION(!pumping_started_);

  pumping_started_ = true;

  try
  {
    pump_();

    while (!next_update_.empty())
    {
      for (auto pos : next_update_)
        order_.mark(pos);

      next_update_.clear();

      pump_();
    }
  }
  catch (...)
  {
    pumping_started_ = false;
    args_buffer_.clear();
    throw;
  }

  pumping_started_ = false;
}

bool engine::update_node_if_activator(vertex_descriptor v,
                                      bool initialized,
                                      std::size_t new_value,
                                      std::size_t old_value)
{
  const auto w = main_consumer_(v);

  if (!initialized)
  {
    const auto e = *(out_edges(w, graph_).first + 1 + new_value);

    activate_subgraph_(e);

    return true;
  }
  else
  {
    if (new_value != old_value)
    {
      const auto e_prev = out_edge_at_(w, 1 + old_value);
      const auto e_curr = out_edge_at_(w, 1 + new_value);

      deactivate_subgraph_(e_prev);

      activate_subgraph_(e_curr);

      return true;
    }
  }

  return false;
}

bool engine::update_node_selector_activator(vertex_descriptor v,
                                            vertex_descriptor x,
                                            bool initialized)
{
  const auto w = main_consumer_(v);

  if (initialized)
  {
    CHECK_CONDITION(out_degree(w, graph_) == 3);

    const auto old_e = out_edge_at_(w, 1);
    const auto old_x = target(old_e, graph_);

    if (old_x == x)
      return false;

    const auto activator_e = last_out_edge_(w);
    const auto activator = target(activator_e, graph_);

    remove_edge(activator_e, graph_);

    const auto new_e = add_data_edge_(w, x);

    add_logical_edge_(w, activator);

    CHECK_CONDITION(out_degree(w, graph_) == 4);

    activate_subgraph_(new_e);

    deactivate_subgraph_(old_e);
    release(old_x);
    remove_edge(old_e, graph_);

    CHECK_CONDITION(out_degree(w, graph_) == 3);
  }
  else
  {
    CHECK_CONDITION(out_degree(w, graph_) == 2);

    const auto activator_e = last_out_edge_(w);
    const auto activator = target(activator_e, graph_);

    remove_edge(activator_e, graph_);

    const auto new_e = add_data_edge_(w, x);

    add_logical_edge_(w, activator);

    CHECK_CONDITION(out_degree(w, graph_) == 3);

    activate_subgraph_(new_e);
  }

  return true;
}

bool engine::update_node_snapshot_activator(vertex_descriptor v,
                                            bool initialized)
{
  const auto w = main_consumer_(v);

  const auto e = *(out_edges(w, graph_).first + 1);

  if (!initialized)
  {
    activate_subgraph_(e);

    schedule_for_next_update(v);

    return true;
  }

  deactivate_subgraph_(e);

  return false;
}

void engine::update_node_state(vertex_descriptor v)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(out_degree(v, graph_) == 4);

  engine::instance().schedule_for_next_update(
    target(out_edge_at_(v, 0), graph_));
}

const node* engine::update_node_state_prev(vertex_descriptor v,
                                           bool initialized)
{
  const auto u = main_consumer_(v);

  CHECK_PRECONDITION(is_active_node(u));
  CHECK_PRECONDITION(out_degree(u, graph_) >= 3);

  const auto e_init = out_edge_at_(u, 1);
  const auto e_regular = out_edge_at_(u, 2);

  if (!initialized)
  {
    CHECK_PRECONDITION(!is_active_data_dependency(e_init));

    activate_subgraph_(e_init);

    CHECK_POSTCONDITION(is_active_data_dependency(e_init));

    schedule_for_next_update(v);

    return nullptr;
  }
  else
  {
    if (!is_active_data_dependency(e_regular))
    {
      deactivate_subgraph_(e_init);
      activate_subgraph_(e_regular);
    }

    CHECK_POSTCONDITION(!is_active_data_dependency(e_init));
    CHECK_POSTCONDITION(is_active_data_dependency(e_regular));
  }

  return graph_[u].p_node;
}

engine::engine()
: allocator_()
, graph_()
, order_(allocator_)
, pumping_started_(false)
, args_buffer_(allocator_)
, ticks_()
, time_node_v_()
{
}

engine::~engine() noexcept
{
  assert(num_vertices(graph_) == 1);

  try
  {
    release(time_node_v_);
  }
  catch (const std::exception& ex)
  {
    assert(false);
  }
}

vertex_descriptor engine::implied_activator_(vertex_descriptor u,
                                             vertex_descriptor v) const
{
  CHECK_PRECONDITION(is_active_node(u));

  if (is_conditional_node(u))
  {
    const auto w = target(out_edge_at_(u, 0), graph_);

    if (v != w)
      return w;
  }

  return target(last_out_edge_(u), graph_);
}

edge_descriptor engine::add_logical_edge_(vertex_descriptor u,
                                          vertex_descriptor v)
{
  edge_descriptor new_e = edge_descriptor();
  bool new_edge_added = false;

  std::tie(new_e, new_edge_added) = add_edge(u, v, graph_);

  CHECK_CONDITION(new_edge_added);

  return new_e;
}

edge_descriptor engine::add_data_edge_(vertex_descriptor u, vertex_descriptor v)
{
  edge_descriptor new_e = edge_descriptor();
  bool new_edge_added = false;

  std::tie(new_e, new_edge_added) = add_edge(u, v, graph_);

  CHECK_CONDITION(new_edge_added);

  graph_[v].add_ref();

  return new_e;
}

void engine::delete_node_(vertex_descriptor v)
{
  CHECK_PRECONDITION(!is_active_node(v));
  CHECK_PRECONDITION(graph_[v].p_node);

  const auto p_node = graph_[v].p_node;

  const auto info = p_node->mem_info();

  p_node->~node();

  dst::memory::deallocate_aligned(
    get_allocator(), p_node, info.first, info.second);

  remove_vertex(v, graph_);
}

void engine::activate_vertex_(vertex_descriptor v,
                              topological_position pos,
                              vertex_descriptor w)
{
  CHECK_PRECONDITION(requires_activation(v));
  CHECK_PRECONDITION(*pos == v);

  graph_[v].position = pos;

  add_logical_edge_(v, w);

  CHECK_POSTCONDITION(!graph_[v].initialized);
  CHECK_POSTCONDITION(is_active_node(v));
}

void engine::deactivate_vertex_partially_(vertex_descriptor v)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(graph_[v].initialized);
  CHECK_PRECONDITION(graph_[v].consumers.size() == 0);

  order_.erase(graph_[v].position);
  graph_[v].position = topological_position();

  graph_[v].initialized = false;

  CHECK_POSTCONDITION(!graph_[v].initialized);
  CHECK_POSTCONDITION(requires_activation(v));
}

void engine::deactivate_vertex_(vertex_descriptor v)
{
  CHECK_PRECONDITION(out_degree(v, graph_) >= 1);

  deactivate_vertex_partially_(v);

  remove_edge(last_out_edge_(v), graph_);
}

void engine::reset_activator_(vertex_descriptor v, vertex_descriptor w)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(is_logical_dependency(last_out_edge_(v)));

  remove_edge(last_out_edge_(v), graph_);

  add_logical_edge_(v, w);
}

void engine::activate_edge_(edge_descriptor e)
{
  CHECK_PRECONDITION(!is_active_data_dependency(e));

  const auto u = source(e, graph_);
  const auto v = target(e, graph_);

  graph_[v].consumers.push_front(u);
  graph_[e] = graph_[v].consumers.cbegin();

  CHECK_POSTCONDITION(is_active_data_dependency(e));
}

void engine::deactivate_edge_(edge_descriptor e)
{
  CHECK_PRECONDITION(is_active_data_dependency(e));

  graph_[target(e, graph_)].consumers.erase(graph_[e]);

  graph_[e] = active_edge_ticket();

  CHECK_POSTCONDITION(!is_active_data_dependency(e));
}

void engine::activate_subgraph_(edge_descriptor e)
{
  CHECK_PRECONDITION(is_active_node(source(e, graph_)));
  CHECK_PRECONDITION(!is_active_data_dependency(e));

  struct vertex_data
  {
    edge_descriptor e;
    bool enabled;
    bool rebased;
    bool repositioned;
  };

  std::stack<vertex_data> stack;

  activate_edge_(e);

  stack.push({e, true, false, false});

  while (!stack.empty())
  {
    const auto vd = stack.top();
    stack.pop();

    const auto u = source(vd.e, graph_);
    const auto v = target(vd.e, graph_);

    assert(u != vertex_descriptor());
    assert(v != vertex_descriptor());

    if (vd.enabled)
    {
      assert(!vd.rebased);
      assert(!vd.repositioned);

      if (requires_activation(v))
      {
        assert(requires_activation(v));

        activate_vertex_(
          v, order_.insert(graph_[u].position, v), implied_activator_(u, v));

        order_.mark(graph_[v].position);

        if (graph_[v].conditional)
        {
          assert(out_degree(v, graph_) >= 2);

          const auto e = *out_edges(v, graph_).first;
          activate_edge_(e);
          stack.push({e, true, false, false});
        }
        else
        {
          assert(out_degree(v, graph_) >= 1);

          for (auto es = out_edges(v, graph_); es.first != es.second - 1;
               ++es.first)
          {
            const auto e = *es.first;
            activate_edge_(e);
            stack.push({e, true, false, false});
          }
        }
      }
      else
      {
        assert(!requires_activation(v));

        if (is_active_node(v))
        {
          const auto b = implied_activator_(u, v);

          assert(is_active_node(b));

          bool rebased =
            order_.order(graph_[b].position, graph_[activator_(v)].position);

          if (rebased)
          {
            reset_activator_(v, b);
          }

          bool repositioned =
            order_.order(graph_[u].position, graph_[v].position);

          if (repositioned)
          {
            bool marked = order_.marked(graph_[v].position);

            order_.erase(graph_[v].position);
            graph_[v].position = order_.insert(graph_[u].position, v);

            if (marked)
              order_.mark(graph_[v].position);
          }

          if (repositioned || rebased)
          {
            assert(out_degree(v, graph_) >= 1);

            for (auto es = out_edges(v, graph_); es.first != es.second - 1;
                 ++es.first)
            {
              if (is_active_data_dependency(*es.first))
                stack.push({*es.first, false, rebased, repositioned});
            }
          }
        }
      }
    }
    else
    {
      assert(!vd.enabled);
      assert(vd.rebased || vd.repositioned);

      if (is_active_node(v))
      {
        const auto b = implied_activator_(u, v);

        assert(is_active_node(b));

        bool rebased =
          vd.rebased &&
          order_.order(graph_[b].position, graph_[activator_(v)].position);

        if (rebased)
        {
          reset_activator_(v, activator_(u));
        }

        bool repositioned =
          vd.repositioned &&
          order_.order(graph_[u].position, graph_[v].position);

        if (repositioned)
        {
          bool marked = order_.marked(graph_[v].position);

          order_.erase(graph_[v].position);
          graph_[v].position = order_.insert(graph_[u].position, v);

          if (marked)
            order_.mark(graph_[v].position);
        }

        if (repositioned || rebased)
        {
          assert(out_degree(v, graph_) >= 1);

          for (auto es = out_edges(v, graph_); es.first != es.second - 1;
               ++es.first)
          {
            if (is_active_data_dependency(*es.first))
              stack.push({*es.first, false, rebased, repositioned});
          }
        }
      }
    }
  }

  return;
}

void engine::deactivate_subgraph_(edge_descriptor e)
{
  CHECK_PRECONDITION(is_active_node(source(e, graph_)));

  const auto v = target(e, graph_);

  deactivate_edge_(e);

  std::stack<vertex_descriptor> stack;

  stack.push(v);

  while (!stack.empty())
  {
    const auto w = stack.top();
    stack.pop();

    if (graph_[w].eager || !is_active_node(w))
      continue;

    if (graph_[w].consumers.size() == 0)
    {
      deactivate_vertex_(w);

      for (auto es = out_edges(w, graph_); es.first != es.second; ++es.first)
      {
        const auto& e = *es.first;

        if (is_active_data_dependency(e))
        {
          deactivate_edge_(e);

          stack.push(target(e, graph_));
        }
      }
    }
    else
    {
      const auto b = activator_(w);
      if (std::none_of(graph_[w].consumers.begin(),
                       graph_[w].consumers.end(),
                       [this, b](vertex_descriptor u)
                       {
                         return b == activator_(u);
                       }))
      {
        const auto it = std::min_element(
          graph_[w].consumers.begin(),
          graph_[w].consumers.end(),
          [this](vertex_descriptor u, vertex_descriptor v)
          {
            return order_.order(graph_[u].position, graph_[v].position);
          });

        bool marked = order_.marked(graph_[w].position);

        order_.erase(graph_[w].position);
        graph_[w].position = order_.insert(graph_[*it].position, w);

        if (marked)
          order_.mark(graph_[w].position);

        reset_activator_(w, activator_(*it));

        for (auto es = out_edges(w, graph_); es.first != es.second; ++es.first)
        {
          const auto& e = *es.first;

          if (is_active_data_dependency(e))
          {
            stack.push(target(e, graph_));
          }
        }
      }
    }
  }
}

void engine::remove_subgraph_(vertex_descriptor v)
{
  CHECK_PRECONDITION(graph_[v].ref_count() == 0);

  class remove_ref_dfs_visitor : public boost::default_dfs_visitor
  {
  public:
    remove_ref_dfs_visitor(dependency_graph& g)
    : graph_(g)
    {
    }

    void discover_vertex(vertex_descriptor v, const dependency_graph&)
    {
      assert(graph_[v].ref_count() == 0);

      if (engine::instance().is_active_node(v))
      {
        for (auto es = out_edges(v, graph_); es.first != es.second; ++es.first)
        {
          const auto e = *es.first;
          if (engine::instance().is_active_data_dependency(e))
            engine::instance().deactivate_subgraph_(e);
        }

        engine::instance().deactivate_vertex_(v);
      }

      CHECK_CONDITION(!engine::instance().is_active_node(v));

      for (auto es = out_edges(v, graph_); es.first != es.second; ++es.first)
      {
        const auto u = target(*es.first, graph_);
        if (graph_[u].release())
        {
          graph_[u].vertex_color = vertex::color::white;
        }
      }
    }

    void finish_vertex(vertex_descriptor v, const dependency_graph&)
    {
      if (graph_[v].ref_count() == 0)
      {
        engine::instance().delete_node_(v);
      }
    }

  private:
    dependency_graph& graph_;
  };

  graph_[v].vertex_color = vertex::color::white;

  boost::depth_first_visit(
    graph_, v, remove_ref_dfs_visitor(graph_), color_map(graph_));
}

void engine::pump_()
{
  ++ticks_;

  CHECK_CONDITION(dynamic_cast<node_time*>(graph_[time_node_v_].p_node));

  const auto p_node_time = static_cast<node_time*>(graph_[time_node_v_].p_node);

  p_node_time->increment();

  order_.mark(graph_[time_node_v_].position);

  const auto to = order_.end_marked();
  for (auto it = order_.begin_marked(); it != to; it = order_.begin_marked())
  {
    const auto v = *it;

    order_.unmark(it.base());

    const auto p_node = graph_[v].p_node;

    assert(p_node);

    assert(args_buffer_.size() == 0);

    for (auto es = out_edges(v, graph_); es.first != es.second; ++es.first)
    {
      const auto e = *es.first;

      if (is_active_data_dependency(e))
        args_buffer_.push_back(graph_[target(e, graph_)].p_node);
    }

    if (p_node->update(converter::convert(v),
                       graph_[v].initialized,
                       &args_buffer_.front(),
                       args_buffer_.size()))
    {
      for (auto v : graph_[v].consumers)
      {
        order_.mark(graph_[v].position);
      }
    }

    graph_[v].initialized = true;

    args_buffer_.clear();
  }

  assert(order_.begin_marked() == order_.end_marked());
}

} // internal
} // dataflow
