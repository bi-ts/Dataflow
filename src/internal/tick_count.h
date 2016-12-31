
//  Copyright 2014-2016 Maksym V. Bilinets.
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
  : ticks_(0)
  {
  }

  tick_count& operator++()
  {
    ++ticks_;
    return *this;
  }

private:
  tick_count(const tick_count&) = delete;
  tick_count(tick_count&&) = delete;
  tick_count& operator=(const tick_count&) = delete;
  tick_count& operator=(tick_count&&) = delete;
  tick_count operator++(int) = delete;

private:
  std::size_t ticks_;
};

} // internal
} // dataflow