
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
  out << "              Duration (sec)   Memory (bytes)   Nodes (active/all)"
      << std::endl;

  PrintCheckPoint(out, "Initial", check_points.start);

  PrintCheckPoint(
    out, "Constructed", check_points.constructed, check_points.start.time);

  PrintCheckPoint(
    out, "Activated*", check_points.activated, check_points.constructed.time);

  PrintCheckPoint(
    out, "Updated", check_points.updated, check_points.activated.time);

  PrintCheckPoint(
    out, "Deactivated", check_points.deactivated, check_points.updated.time);

  PrintCheckPoint(out,
                  "Activated*",
                  check_points.second_time_activated,
                  check_points.deactivated.time);

  PrintCheckPoint(out,
                  "Deactivated",
                  check_points.second_time_deactivated,
                  check_points.second_time_activated.time);

  PrintCheckPoint(out,
                  "Destructed",
                  check_points.destructed,
                  check_points.second_time_deactivated.time);

  out << std::endl;

  out << "*Activation operation also includes initial update" << std::endl;
}
