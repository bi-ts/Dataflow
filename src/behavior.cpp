
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#include <dataflow/behavior.h>

std::ostream& dataflow::operator<<(std::ostream& out, button_state v)
{
  out << "button_state::";

  switch (v)
  {
  case button_state::up:
    out << "up";
    break;
  case button_state::down:
    out << "down";
    break;
  default:
    out << "unknown (" << static_cast<int>(v) << ")";
    break;
  }

  return out;
}

std::ostream& dataflow::operator<<(std::ostream& out, button_state_change v)
{
  out << "button_state_change::";

  switch (v)
  {
  case button_state_change::released:
    out << "released";
    break;
  case button_state_change::pressed:
    out << "pressed";
    break;
  case button_state_change::unchanged:
    out << "unchanged";
    break;
  default:
    out << "unknown (" << static_cast<int>(v) << ")";
    break;
  }

  return out;
}

dataflow::button_state_change dataflow::operator-(button_state lhs,
                                                  button_state rhs)
{
  return static_cast<button_state_change>(static_cast<int>(lhs) -
                                          static_cast<int>(rhs));
}
