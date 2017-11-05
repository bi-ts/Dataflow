
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

#pragma once

#include "dataflow++_export.h"

#include "ref.h"

#include <cstddef> // std::size_t
#include <memory>  // std::addressof
#include <utility> // std::forward

namespace dataflow
{

namespace internal
{

class DATAFLOW___EXPORT nodes_factory
{
public:
  template <typename Node, typename... Args>
  static ref create(const node_id* p_args,
                    std::size_t args_count,
                    bool eager,
                    Args&&... args)
  {
    return add_(
      new_node_<Node>(std::forward<Args>(args)...), p_args, args_count, eager);
  }

  template <typename Node, typename... Args>
  static ref create_conditional(const node_id* p_args,
                                std::size_t args_count,
                                bool eager,
                                Args&&... args)
  {
    return add_conditional_(
      new_node_<Node>(std::forward<Args>(args)...), p_args, args_count, eager);
  }

  template <typename Node, typename... Args>
  static ref create_constant(Args&&... args)
  {
    return add_constant_(new_node_<Node>(std::forward<Args>(args)...));
  }

  static ref get_time()
  {
    return get_time_();
  }

private:
  template <typename Node, typename... Args>
  static Node* new_node_(Args&&... args)
  {
    const auto p_node =
      static_cast<Node*>(allocate_(sizeof(Node), alignof(Node)));

    try
    {
      new (std::addressof(*p_node)) Node(std::forward<Args>(args)...);
    }
    catch (...)
    {
      deallocate_(p_node, sizeof(Node), alignof(Node));
      throw;
    }

    return p_node;
  }

  static void* allocate_(std::size_t size, std::size_t allignment);
  static void deallocate_(void*, std::size_t size, std::size_t alignment);

  static ref
  add_(node* p_node, const node_id* p_args, std::size_t args_count, bool eager);
  static ref add_conditional_(node* p_node,
                              const node_id* p_args,
                              std::size_t args_count,
                              bool eager);
  static ref add_constant_(node* p_node);
  static ref get_time_();
};
}
}
