
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

#pragma once

#include "dataflow++_export.h"

#include <cstdint>
#include <string>
#include <utility>

namespace dataflow
{

namespace internal
{

class tick_count;

using node_id = std::intptr_t;

enum class update_status
{
  nothing = 0x00,
  updated = 0x01,
  updated_next = 0x02
};

inline update_status operator|(update_status lhs, update_status rhs)
{
  using type = std::underlying_type<update_status>::type;

  return static_cast<update_status>(static_cast<type>(lhs) |
                                    static_cast<type>(rhs));
}

inline update_status& operator|=(update_status& lhs, update_status rhs)
{
  return lhs = lhs | rhs;
}

inline update_status operator&(update_status lhs, update_status rhs)
{
  using type = std::underlying_type<update_status>::type;

  return static_cast<update_status>(static_cast<type>(lhs) &
                                    static_cast<type>(rhs));
}

inline update_status& operator&=(update_status& lhs, update_status rhs)
{
  return lhs = lhs & rhs;
}

class DATAFLOW___EXPORT node
{
public:
  void activate(node_id id, const tick_count& t0)
  {
    activate_(id, t0);
  }

  update_status update(node_id id,
                       bool initialized,
                       const node** p_args,
                       std::size_t args_count)
  {
    return update_(id, initialized, p_args, args_count);
  }

  void deactivate(node_id id)
  {
    deactivate_(id);
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

protected:
  static const tick_count& ticks_();
  static void pump(node_id id);

private:
  virtual void activate_(node_id id, const tick_count& t0)
  {
  }

  virtual update_status update_(node_id id,
                                bool initialized,
                                const node** p_args,
                                std::size_t args_count) = 0;

  virtual void deactivate_(node_id id)
  {
  }

  virtual std::string label_() const = 0;

  virtual std::string to_string_() const = 0;

  virtual std::pair<std::size_t, std::size_t> mem_info_() const = 0;
};
}
}
