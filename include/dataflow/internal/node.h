
//  Copyright (c) 2014 - 2016 Maksym V. Bilinets.
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

#include <cstdint>
#include <string>
#include <utility>

namespace dataflow
{

namespace internal
{

using node_id = std::intptr_t;

class DATAFLOW___EXPORT node
{
public:
  bool
  update(node_id id, bool init, const node** p_args, std::size_t args_count)
  {
    return update_(id, init, p_args, args_count);
  }

  std::string label() const
  {
    return label_();
  }

  std::string to_string() const
  {
    return to_string_();
  }

  std::pair<std::size_t, std::size_t> mem_info() const
  {
    return mem_info_();
  }

  virtual ~node(){};

private:
  virtual bool update_(node_id id,
                       bool init,
                       const node** p_args,
                       std::size_t args_count) = 0;

  virtual std::string label_() const = 0;

  virtual std::string to_string_() const = 0;

  virtual std::pair<std::size_t, std::size_t> mem_info_() const = 0;
};
}
}
