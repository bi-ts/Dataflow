
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

#pragma once

#ifndef DATAFLOW___BEHAVIOR_H
#define DATAFLOW___BEHAVIOR_H

#include "dataflow++_export.h"

#include "prelude.h"

namespace dataflow
{
enum class button_state
{
  up = 0,
  down = 1
};

DATAFLOW___EXPORT std::ostream& operator<<(std::ostream& out, button_state v);

enum class button_state_change
{
  released = -1,
  unchanged = 0,
  pressed = 1
};

DATAFLOW___EXPORT std::ostream& operator<<(std::ostream& out,
                                           button_state_change v);

DATAFLOW___EXPORT button_state_change operator-(button_state lhs,
                                                button_state rhs);

}

#include "behavior.inl"

#endif // DATAFLOW___BEHAVIOR_H
