
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

#ifndef DATAFLOW___STRING_H
#define DATAFLOW___STRING_H

#include "dataflow++_export.h"

#include "prelude/arithmetic.h"
#include "prelude/core.h"

namespace dataflow
{
/// \defgroup string
/// \{

// String functions

template <typename X> ref<std::string> ToString(const ref<X>& x);

template <>
DATAFLOW___EXPORT ref<std::string> ToString(const ref<std::string>& x);

template <typename T, typename = core::enable_if_convertible_to_flowable_t<T>>
ref<std::string> ToString(const T& v);

template <typename A, typename B, typename... Args>
ref<std::string> ToString(const A& a, const B& b, const Args&... args);

template <typename T>
dataflow::ref<T> FromString(const ref<std::string>& str, const T& d = T());

template <>
DATAFLOW___EXPORT ref<std::string> FromString(const ref<std::string>& x,
                                              const std::string&);

template <typename T>
dataflow::ref<T> FromString(const char* str, const T& d = T());

/// \}
} // dataflow

#include "string.inl"

#endif // DATAFLOW___STRING_H
