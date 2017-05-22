
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

#if !defined(DATAFLOW___CONDITIONAL_H)
#error '.inl' file can't be included directly. Use 'conditional.h' instead
#endif

#include "comparison.h"

template <typename T>
dataflow::ref<T>
dataflow::If(const ref<bool>& x, const ref<T>& y, const ref<T>& z)
{
  return core::Conditional(x, y, z);
}

template <typename T>
dataflow::ref<T> dataflow::If(const ref<bool>& x, const T& y, const ref<T>& z)
{
  return If(x, Const<T>(y), z);
}

template <typename T>
dataflow::ref<T> dataflow::If(const ref<bool>& x, const ref<T>& y, const T& z)
{
  return If(x, y, Const<T>(z));
}

template <typename T, typename>
dataflow::ref<T> dataflow::If(const ref<bool>& x, const T& y, const T& z)
{
  return If(x, Const<T>(y), Const<T>(z));
}

template <typename T>
dataflow::ref<T>
dataflow::Switch(const std::pair<ref<bool>, ref<T>>& first_case,
                 const ref<T>& default_case)
{
  return If(first_case.first, first_case.second, default_case);
}

template <typename T, typename... Cases>
dataflow::ref<T>
dataflow::Switch(const std::pair<ref<bool>, ref<T>>& first_case,
                 const Cases&... other_cases)
{
  return If(first_case.first, first_case.second, Switch(other_cases...));
}

template <typename T, typename U>
dataflow::ref<U> dataflow::Switch(const ref<T>& x,
                                  const std::pair<ref<T>, ref<U>>& first_case,
                                  const ref<U>& default_case)
{
  return If(x == first_case.first, first_case.second, default_case);
}

template <typename T, typename U, typename... Cases>
dataflow::ref<U> dataflow::Switch(const ref<T>& x,
                                  const std::pair<ref<T>, ref<U>>& first_case,
                                  const Cases&... other_cases)
{
  return If(
    x == first_case.first, first_case.second, Switch(x, other_cases...));
}

template <typename T, typename U>
std::pair<dataflow::ref<T>, dataflow::ref<U>> dataflow::Case(const ref<T>& x,
                                                             const ref<U>& y)
{
  return std::make_pair(x, y);
}

template <typename T, typename U, typename>
std::pair<dataflow::ref<T>, dataflow::ref<U>> dataflow::Case(const ref<T>& x,
                                                             const U& y)
{
  return std::make_pair(x, Const(y));
}

template <typename T>
std::pair<dataflow::ref<T>, dataflow::ref<std::string>>
dataflow::Case(const ref<T>& x, const char* y)
{
  return Case(x, Const(y));
}

template <typename T, typename U, typename>
std::pair<dataflow::ref<T>, dataflow::ref<U>> dataflow::Case(const T& x,
                                                             const ref<U>& y)
{
  return Case(Const(x), y);
}

template <typename T, typename U, typename, typename>
std::pair<dataflow::ref<T>, dataflow::ref<U>> dataflow::Case(const T& x,
                                                             const U& y)
{
  return Case(Const(x), Const(y));
}

template <typename T, typename>
std::pair<dataflow::ref<T>, dataflow::ref<std::string>>
dataflow::Case(const T& x, const char* y)
{
  return Case(Const(x), y);
}

template <typename U>
std::pair<dataflow::ref<std::string>, dataflow::ref<U>>
dataflow::Case(const char* x, const ref<U>& y)
{
  return Case(Const(x), y);
}

template <typename U, typename>
std::pair<dataflow::ref<std::string>, dataflow::ref<U>>
dataflow::Case(const char* x, const U& y)
{
  return Case(x, Const(y));
}

inline std::pair<dataflow::ref<std::string>, dataflow::ref<std::string>>
dataflow::Case(const char* x, const char* y)
{
  return Case(Const(x), Const(y));
}

template <typename T> dataflow::ref<T> dataflow::Default(const ref<T>& x)
{
  return x;
}

template <typename T> dataflow::ref<T> dataflow::Default(const T& v)
{
  return Const(v);
}

inline dataflow::ref<std::string> dataflow::Default(const char* v)
{
  return Default<std::string>(v);
}

template <typename T>
std::pair<dataflow::ref<bool>, dataflow::ref<T>> dataflow::
operator>>=(const ref<bool>& x, const ref<T>& y)
{
  return Case(x, y);
}

template <typename T, typename>
std::pair<dataflow::ref<bool>, dataflow::ref<T>> dataflow::
operator>>=(const ref<bool>& x, const T& y)
{
  return Case(x, y);
}

inline std::pair<dataflow::ref<bool>, dataflow::ref<std::string>> dataflow::
operator>>=(const ref<bool>& x, const char* y)
{
  return Case(x, y);
}
