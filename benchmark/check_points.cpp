
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

#include "check_points.h"

#include <iomanip>

double benchmark::CalculateUpdateDurationSec(const CheckPoints& check_points)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(
           check_points.updated.time - check_points.activated.time)
           .count() /
         1000000.0;
}

void benchmark::PrintCheckPoints(std::ostream& out,
                                 const CheckPoints& check_points)
{
  const std::string table_head[] = {
    "------------------------------------------------------------------------",
    "                  Total    Time per Affected      Memory    Nodes count",
    "              time (ms)   node (ns)    nodes     (bytes)   active/total",
    "------------------------------------------------------------------------"};

  for (const auto l : table_head)
  {
    out << l << std::endl;
  }

  PrintCheckPoint(out, "Initial", check_points.start);

  PrintCheckPoint(out,
                  "Constructed",
                  check_points.constructed,
                  check_points.start.time,
                  check_points.constructed.all_nodes_count -
                    check_points.start.all_nodes_count);

  PrintCheckPoint(out,
                  "Activated*",
                  check_points.activated,
                  check_points.constructed.time,
                  check_points.activated.active_nodes_count -
                    check_points.constructed.active_nodes_count);

  PrintCheckPoint(out,
                  "Updated",
                  check_points.updated,
                  check_points.activated.time,
                  check_points.updated.updated_nodes_count);

  PrintCheckPoint(out,
                  "Deactivated",
                  check_points.deactivated,
                  check_points.updated.time,
                  check_points.updated.active_nodes_count -
                    check_points.deactivated.active_nodes_count);

  PrintCheckPoint(out,
                  "Activated*",
                  check_points.second_time_activated,
                  check_points.deactivated.time,
                  check_points.second_time_activated.active_nodes_count -
                    check_points.deactivated.active_nodes_count);

  PrintCheckPoint(out,
                  "Deactivated",
                  check_points.second_time_deactivated,
                  check_points.second_time_activated.time,
                  check_points.second_time_activated.active_nodes_count -
                    check_points.second_time_deactivated.active_nodes_count);

  PrintCheckPoint(out,
                  "Destructed",
                  check_points.destructed,
                  check_points.second_time_deactivated.time,
                  check_points.second_time_deactivated.all_nodes_count -
                    check_points.destructed.all_nodes_count);

  out << std::endl;

  out << "*Activation operation also includes initial update" << std::endl;
}
