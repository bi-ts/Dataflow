
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

#if !defined(DATAFLOW___IO_H)
#error '.inl' file can't be included directly. Use 'io.h' instead
#endif

#include "string.h"

#include <sstream>

template <typename T>
dataflow::ref<T> dataflow::console::Input(dtime t0,
                                          const ref<std::string>& prompt)
{
  return FromString<T>(Input(t0, prompt));
}

template <typename... Args>
dataflow::ref<std::string> dataflow::console::Error(dtime t0,
                                                    const Args&... args)
{
  return Error(t0, ToString(args...));
}

template <typename... Args>
dataflow::ref<std::string> dataflow::console::Log(dtime t0, const Args&... args)
{
  return Log(t0, ToString(args...));
}

template <typename... Args>
dataflow::ref<std::string> dataflow::console::Output(dtime t0,
                                                     const Args&... args)
{
  return Output(t0, ToString(args...));
}
