
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

#include <cassert>

#define DATAFLOW___CHECK_ARGUMENT(x) assert(x)

#define DATAFLOW___CHECK_NOT_REACHABLE() assert(!"Not supposed to be reached")
#define DATAFLOW___CHECK_PRECONDITION(x) assert(x)
#define DATAFLOW___CHECK_CONDITION(x) assert(x)
#define DATAFLOW___CHECK_POSTCONDITION(x) assert(x)

#define DATAFLOW___CHECK_NOT_REACHABLE_DEBUG()                                 \
  assert(!"Not supposed to be reached")
#define DATAFLOW___CHECK_PRECONDITION_DEBUG(x) assert(x)
#define DATAFLOW___CHECK_CONDITION_DEBUG(x) assert(x)
#define DATAFLOW___CHECK_POSTCONDITION_DEBUG(x) assert(x)

#define DATAFLOW___CHECK_NOT_REACHABLE_NOEXCEPT()                              \
  assert(!"Not supposed to be reached")
#define DATAFLOW___CHECK_PRECONDITION_NOEXCEPT(x) assert(x)
#define DATAFLOW___CHECK_CONDITION_NOEXCEPT(x) assert(x)
#define DATAFLOW___CHECK_POSTCONDITION_NOEXCEPT(x) assert(x)
