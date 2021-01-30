
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

#include <dataflow/geometry.h>

std::ostream& dataflow::operator<<(std::ostream& out, dir2 v)
{
  out << "dir2::";

  switch (v)
  {
  case dir2::east:
    return out << "east";
  case dir2::north:
    return out << "north";
  case dir2::south:
    return out << "south";
  case dir2::west:
    return out << "west";
  default:
    return out << "unknown (" << static_cast<int>(v) << ")";
  }

  return out;
}

dataflow::dir2 dataflow::operator-(dir2 dir)
{
  switch (dir)
  {
  case dir2::north:
    return dir2::south;
  case dir2::east:
    return dir2::west;
  case dir2::south:
    return dir2::north;
  case dir2::west:
    return dir2::east;
  };

  return dir;
}
