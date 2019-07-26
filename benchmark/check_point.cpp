
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

#include "check_point.h"

#define DATAFLOW___NO_BOOST

#include <dataflow/introspect.h>

#include <iomanip>

namespace benchmark
{
namespace
{
void PrintCheckPointWithoutTime(std::ostream& out,
                                const CheckPoint& check_point)
{
  out << std::left << std::setw(17) << check_point.memory_consumption;

  out << std::right << std::setw(8) << check_point.active_nodes_count;
  out << "/";
  out << std::left << std::setw(8) << check_point.all_nodes_count;

  out << std::endl;
}
}
}

benchmark::CheckPoint benchmark::make_check_point()
{
  return CheckPoint{dataflow::introspect::memory_consumption(),
                    std::chrono::steady_clock::now(),
                    dataflow::introspect::num_active_nodes(),
                    dataflow::introspect::num_vertices()};
}

void benchmark::PrintCheckPoint(std::ostream& out,
                                const std::string& name,
                                const CheckPoint& check_point)
{
  out << std::left << std::setw(31) << name + ':';

  PrintCheckPointWithoutTime(out, check_point);
}

void benchmark::PrintCheckPoint(std::ostream& out,
                                const std::string& name,
                                const CheckPoint& check_point,
                                std::chrono::steady_clock::time_point prev_time)
{
  const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                          check_point.time - prev_time)
                          .count() /
                        1000000.0;

  out << std::left << std::setw(14) << name + ':' << std::setw(17) << duration;

  PrintCheckPointWithoutTime(out, check_point);
}
