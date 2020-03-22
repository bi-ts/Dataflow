
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

#include "expand.h"
#include "invoke.h"

#define DATAFLOW___TUPLE_SIZE(tuple)                                           \
  DATAFLOW___INVOKE(DATAFLOW___TUPLE_SIZE_, tuple)

// clang-format off

#define DATAFLOW___TUPLE_SIZE_(...)                                            \
  DATAFLOW___EXPAND(DATAFLOW___GET_ARG_64_(__VA_ARGS__,                        \
    64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,            \
    48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,            \
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,            \
    16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 0))

#define DATAFLOW___GET_ARG_64_(                                                \
  e000, e001, e002, e003, e004, e005, e006, e007, e008, e009, e010,            \
  e011, e012, e013, e014, e015, e016, e017, e018, e019, e020, e021,            \
  e022, e023, e024, e025, e026, e027, e028, e029, e030, e031, e032,            \
  e033, e034, e035, e036, e037, e038, e039, e040, e041, e042, e043,            \
  e044, e045, e046, e047, e048, e049, e050, e051, e052, e053, e054,            \
  e055, e056, e057, e058, e059, e060, e061, e062, e063, size, ...)             \
  size

// clang-format on
