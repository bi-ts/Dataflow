
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
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
#include "vd_handle.h"

#include <dst/allocator/utility.h>

#include <cstdint> // std::intptr_t
#include <stack>

namespace dataflow
{
namespace internal
{
engine* engine::gp_engine_ = nullptr;

void engine::start(void* p_data, engine_options options)
{
  CHECK_PRECONDITION(gp_engine_ == nullptr);

  gp_engine_ = new engine(p_data, options);

  const auto p_node = static_cast<node_time*>(dst::memory::allocate_aligned(
    gp_engine_->get_allocator(), sizeof(node_time), alignof(node_time)));

  new (std::addressof(*p_node)) node_time();

  const auto v = add_vertex(vertex(p_node), gp_engine_->graph_);

  gp_engine_->graph_[v].eager = true;

  gp_engine_->activate_vertex_(
    v, gp_engine_->new_topological_pos_(gp_engine_->order_.end(), v), v);

  // TODO: check why it is necessary?
  gp_engine_->graph_[v].initialized = true;

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

dtimestamp engine::current_time() const
{
  const auto p_time_node =
    static_cast<const node_time*>(graph_[time_node_v_].p_node);

  return p_time_node->value();
}

vertex_descriptor engine::add_node(node* p_node,
                                   const node_id* p_args,
                                   std::size_t args_count,
                                   bool eager,
                                   bool conditional,
                                   bool pump)
{
  CHECK_ARGUMENT(!pump || eager); // pump => (implies) eager
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

    activate_vertex_(v, new_topological_pos_(order_.end(), v), order_.front());

    order_.mark(graph_[v].position);

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

  if (pump)
  {
    CHECK_CONDITION(!is_pumping());
    CHECK_CONDITION(eager);
    CHECK_CONDITION(order_.marked(graph_[v].position));

    pumpa_.pump(graph_, order_, time_node_v_);
  }

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

void engine::add_data_edge(vertex_descriptor u, vertex_descriptor v)
{
  CHECK_PRECONDITION(!is_active_node(u));

  add_data_edge_(u, v);
}

void engine::remove_data_edge(vertex_descriptor u, std::size_t idx)
{
  CHECK_PRECONDITION(!is_active_node(u));
  CHECK_PRECONDITION(out_degree(u, graph_) > idx);
  CHECK_PRECONDITION(!is_active_data_dependency(out_edge_at_(u, idx)));

  const auto e = out_edge_at_(u, idx);

  release(target(e, graph_));

  remove_edge(e, graph_);
}

void engine::schedule_and_pump(vertex_descriptor v)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(!is_pumping());

  order_.mark(graph_[v].position);

  pumpa_.pump(graph_, order_, time_node_v_);
}

void engine::schedule_for_next_update(vertex_descriptor v)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(is_pumping());

  pumpa_.schedule_for_next_update(graph_[v].position);
}

bool engine::is_pumping() const
{
  return pumpa_.is_pumping();
}

void engine::set_metadata(const node* p_node,
                          std::shared_ptr<const metadata> p_metadata)
{
  // TODO: check if the node is active
  return pumpa_.set_metadata(p_node, p_metadata);
}

const std::shared_ptr<const metadata>& engine::get_metadata(const node* p_node)
{
  return pumpa_.get_metadata(p_node);
}

update_status engine::update_node_if_activator(vertex_descriptor v,
                                               bool initialized,
                                               std::size_t new_value,
                                               std::size_t old_value)
{
  const auto w = main_consumer_(v);

  if (!initialized)
  {
    const auto e = *(out_edges(w, graph_).first + 1 + new_value);

    activate_subgraph_(e);

    return update_status::updated;
  }
  else
  {
    if (new_value != old_value)
    {
      const auto e_prev = out_edge_at_(w, 1 + old_value);
      const auto e_curr = out_edge_at_(w, 1 + new_value);

      deactivate_subgraph_(e_prev);

      activate_subgraph_(e_curr);

      return update_status::updated;
    }
  }

  return update_status::nothing;
}

update_status engine::update_node_selector_activator(vertex_descriptor v,
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
      return update_status::nothing;

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

  return update_status::updated;
}

update_status engine::update_node_snapshot_activator(vertex_descriptor v,
                                                     bool initialized)
{
  const auto w = main_consumer_(v);

  const auto e = *(out_edges(w, graph_).first + 1);

  if (!initialized)
  {
    activate_subgraph_(e);

    return update_status::updated | update_status::updated_next;
  }

  deactivate_subgraph_(e);

  return update_status::nothing;
}

update_status engine::update_node_since_activator(vertex_descriptor v,
                                                  bool initialized,
                                                  bool start_condition)
{
  if (!initialized)
  {
    const auto w = main_consumer_(v);

    const auto e = *(out_edges(w, graph_).first + 1);

    activate_subgraph_(e);

    return update_status::updated;
  }
  else
  {
    if (start_condition)
    {
      const auto w = main_consumer_(v);

      const auto e = out_edge_at_(w, 1);

      deactivate_subgraph_(e);

      activate_subgraph_(e);

      return update_status::updated;
    }
  }

  return update_status::nothing;
}

void engine::update_node_recursion(vertex_descriptor v)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(out_degree(v, graph_) == 4);

  const auto w = target(out_edge_at_(v, 0), graph_);

  CHECK_CONDITION(w != vertex_descriptor());

  schedule_for_next_update(w);
}

std::pair<const node*, update_status>
engine::update_node_recursion_activator(vertex_descriptor v, bool initialized)
{
  const auto u = main_consumer_(v);

  CHECK_PRECONDITION(is_active_node(u));
  CHECK_PRECONDITION(out_degree(u, graph_) >= 3);

  const auto e_init = out_edge_at_(u, 1);
  const auto e_regular = out_edge_at_(u, 2);

  auto status = update_status::nothing;

  if (!initialized)
  {
    CHECK_PRECONDITION(!is_active_data_dependency(e_init));

    activate_subgraph_(e_init);

    CHECK_POSTCONDITION(is_active_data_dependency(e_init));
    CHECK_POSTCONDITION(!is_active_data_dependency(e_regular));

    status = update_status::updated_next;
  }
  else
  {
    if (!is_active_data_dependency(e_regular))
    {
      deactivate_subgraph_(e_init);
      activate_subgraph_(e_regular);

      status = update_status::updated;
    }

    CHECK_POSTCONDITION(!is_active_data_dependency(e_init));
    CHECK_POSTCONDITION(is_active_data_dependency(e_regular));
  }

  return std::make_pair(graph_[u].p_node, status);
}

engine::engine(void* p_data, engine_options options)
: allocator_()
, p_data_(p_data)
, options_(options)
, graph_()
#ifdef DATAFLOW___EXPERIMENTAL_BUILD_WITH_BOOST_POOL_ALLOCATOR
, order_()
#else
, order_(allocator_)
#endif
, pumpa_(allocator_, options)
, ticks_()
, time_node_v_()
{
}

engine::~engine() noexcept
{
  CHECK_PRECONDITION_NOEXCEPT(num_vertices(graph_) == 1);

  try
  {
    release(time_node_v_);
  }
  catch (const std::exception& ex)
  {
    CHECK_NOT_REACHABLE_NOEXCEPT();
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

  return activator_(u);
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

void engine::remove_from_topological_list_(vertex_descriptor v)
{
  const auto position = graph_[v].position;

  CHECK_PRECONDITION(position != topological_position());

  order_.erase(position);

  graph_[v].position = topological_position();
}

topological_position engine::new_topological_pos_(topological_position position,
                                                  vertex_descriptor v)
{
  CHECK_PRECONDITION(position != topological_position());

  return order_.insert(position, v);
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
  CHECK_PRECONDITION(graph_[v].p_node != nullptr);

  graph_[v].p_node->activate(converter::convert(v), ticks_);

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
  CHECK_PRECONDITION(graph_[v].p_node != nullptr);

  remove_from_topological_list_(v);

  graph_[v].initialized = false;

  graph_[v].p_node->deactivate(converter::convert(v));

  CHECK_POSTCONDITION(!graph_[v].initialized);
  CHECK_POSTCONDITION(requires_activation(v));
}

void engine::deactivate_vertex_(vertex_descriptor v)
{
  CHECK_PRECONDITION(out_degree(v, graph_) >= 1);

  deactivate_vertex_partially_(v);

  remove_edge(last_out_edge_(v), graph_);
}

void engine::move_to_topological_position_(vertex_descriptor v,
                                           vertex_descriptor w)
{
  CHECK_PRECONDITION(is_active_node(v));
  CHECK_PRECONDITION(is_active_node(w));

  const bool marked = order_.marked(graph_[v].position);

  remove_from_topological_list_(v);

  graph_[v].position = new_topological_pos_(graph_[w].position, v);

  if (marked)
    order_.mark(graph_[v].position);
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
  CHECK_PRECONDITION(is_active_node(source(e, graph_)));

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

        activate_vertex_(v,
                         new_topological_pos_(graph_[u].position, v),
                         implied_activator_(u, v));

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
            move_to_topological_position_(v, u);
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

        bool repositioned = vd.repositioned && order_.order(graph_[u].position,
                                                            graph_[v].position);

        if (repositioned)
        {
          move_to_topological_position_(v, u);
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

  std::stack<vd_handle> stack;

  stack.push(v);

  while (!stack.empty())
  {
    const auto w = stack.top();
    stack.pop();

    if (graph_[w].eager || !is_active_node(w))
      continue;

    if (graph_[w].consumers.empty())
    {
      CHECK_CONDITION(graph_[w].consumers.empty());

      for (auto es = out_edges(w, graph_); es.first != es.second; ++es.first)
      {
        const auto& e = *es.first;

        if (is_active_data_dependency(e))
        {
          deactivate_edge_(e);

          stack.push(target(e, graph_));
        }
      }

      deactivate_vertex_(w);
    }
    else
    {
      CHECK_CONDITION(!graph_[w].consumers.empty());

      if (std::none_of(graph_[w].consumers.begin(),
                       graph_[w].consumers.end(),
                       [this, w, a = activator_(w)](vertex_descriptor u) {
                         return a == implied_activator_(u, w);
                       }))
      {
        const auto b = *std::min_element(
          graph_[w].consumers.begin(),
          graph_[w].consumers.end(),
          [this](vertex_descriptor u, vertex_descriptor v) {
            return order_.order(graph_[u].position, graph_[v].position);
          });

        move_to_topological_position_(w, b);

        const auto c = *std::min_element(
          graph_[w].consumers.begin(),
          graph_[w].consumers.end(),
          [this, w](vertex_descriptor u, vertex_descriptor v) {
            const auto uim = implied_activator_(u, w);
            const auto vim = implied_activator_(v, w);
            return order_.order(graph_[uim].position, graph_[vim].position);
          });

        CHECK_CONDITION(activator_(w) != implied_activator_(c, w));

        reset_activator_(w, implied_activator_(c, w));

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
  std::stack<vertex_descriptor> stack;

  stack.push(v);

  while (!stack.empty())
  {
    const auto u = stack.top();
    stack.pop();

    CHECK_CONDITION(graph_[u].ref_count() == 0);

    if (is_active_node(u))
    {
      for (auto es = out_edges(u, graph_); es.first != es.second; ++es.first)
      {
        const auto e = *es.first;
        if (is_active_data_dependency(e))
          deactivate_subgraph_(e);
      }

      deactivate_vertex_(u);
    }

    CHECK_CONDITION(!is_active_node(u));

    for (auto es = out_edges(u, graph_); es.first != es.second; ++es.first)
    {
      CHECK_CONDITION(!is_logical_dependency(*es.first));

      const auto w = target(*es.first, graph_);
      if (graph_[w].release())
        stack.push(w);
    }

    delete_node_(u);
  }
}
} // internal
} // dataflow
