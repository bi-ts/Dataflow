
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include <dataflow/internal/config.h>

#define CHECK_ARGUMENT(x) DATAFLOW___CHECK_ARGUMENT(x)
#define CHECK_NOT_CALLED() DATAFLOW___CHECK_NOT_CALLED()
#define CHECK_PRECONDITION(x) DATAFLOW___CHECK_PRECONDITION(x)
#define CHECK_CONDITION(x) DATAFLOW___CHECK_CONDITION(x)
#define CHECK_POSTCONDITION(x) DATAFLOW___CHECK_POSTCONDITION(x)
