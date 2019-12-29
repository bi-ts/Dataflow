
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

#ifndef DATAFLOW___IO_H
#define DATAFLOW___IO_H

#include "dataflow++_export.h"

#include "prelude.h"

#include <string>

namespace dataflow
{
namespace console
{
DATAFLOW___EXPORT ref<std::string> Input(dtime t0,
                                         const ref<std::string>& prompt);
template <typename T> ref<T> Input(dtime t0, const ref<std::string>& prompt);

DATAFLOW___EXPORT ref<std::string> Error(dtime t0,
                                         const ref<std::string>& message);
template <typename... Args>
ref<std::string> Error(dtime t0, const Args&... args);

DATAFLOW___EXPORT ref<std::string> Log(dtime t0,
                                       const ref<std::string>& message);
template <typename... Args> ref<std::string> Log(dtime t0, const Args&... args);

DATAFLOW___EXPORT ref<std::string> Output(dtime t0,
                                          const ref<std::string>& message);
template <typename... Args>
ref<std::string> Output(dtime t0, const Args&... args);
}
}

#include "io.inl"

#endif // DATAFLOW___IO_H
