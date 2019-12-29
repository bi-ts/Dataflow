
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

#include <cstddef>

namespace dataflow
{
namespace internal
{
class tick_count
{
public:
  tick_count()
  : ticks_(std::numeric_limits<std::size_t>::max())
  {
  }

  tick_count& operator++()
  {
    ++ticks_;
    return *this;
  }

  operator std::size_t() const
  {
    return ticks_;
  }

private:
  tick_count(const tick_count&) = delete;
  tick_count& operator=(const tick_count&) = delete;
  tick_count operator++(int) = delete;

private:
  // TODO: get rid of parallel counters (the second counter is in `node_time`)
  std::size_t ticks_;
};

} // internal
} // dataflow