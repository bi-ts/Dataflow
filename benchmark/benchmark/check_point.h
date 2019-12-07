
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

#include <chrono>
#include <cstddef>
#include <ostream>

namespace benchmark
{
struct CheckPoint
{
  std::size_t memory_consumption;
  std::chrono::steady_clock::time_point time;
  std::size_t updated_nodes_count;
  std::size_t active_nodes_count;
  std::size_t all_nodes_count;
};

CheckPoint make_check_point();

void PrintCheckPoint(std::ostream& out,
                     const std::string& name,
                     const CheckPoint& check_point);

void PrintCheckPoint(std::ostream& out,
                     const std::string& name,
                     const CheckPoint& check_point,
                     std::chrono::steady_clock::time_point prev_time,
                     std::size_t affected_nodes);
}
