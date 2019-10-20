
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

#ifndef DATAFLOW___MAYBE_H
#define DATAFLOW___MAYBE_H

#include "prelude.h"

namespace dataflow
{
/// \defgroup maybe
/// \{

struct nothing_t
{
};

template <typename T> class maybe : public core::composite_base
{
public:
  maybe(nothing_t);

  explicit maybe();

  explicit maybe(const T& x);

  bool engaged() const;

  T value_or(const T& def = T()) const;

  bool operator==(const maybe& other) const;
  bool operator!=(const maybe& other) const;

private:
  union data;

  data data_;
};

template <typename T> class maybe<ref<T>> : public core::aggregate_base
{
public:
  maybe(nothing_t);

  explicit maybe();

  explicit maybe(const ref<T>& x);

  bool engaged() const;

  ref<T> value_or(const ref<T>& def) const;
  ref<T> value_or() const;

  bool operator==(const maybe& other) const;
  bool operator!=(const maybe& other) const;

private:
  union data;

  data data_;
};

template <typename T> maybe<T> just(const T& x)
{
  return maybe<T>{x};
}

nothing_t nothing()
{
  return nothing_t{};
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const maybe<T>& value);

template <typename T> ref<maybe<T>> JustE(const ref<T>& x);

template <typename T,
          typename U,
          typename = core::enable_for_argument_data_type_t<U, T>>
ref<T> FromMaybe(const ref<maybe<T>>& x, const U& def);

template <typename T> ref<T> FromMaybe(const ref<maybe<T>>& x);

template <typename T> ref<maybe<T>> Nothing();

template <typename T> ref<maybe<ref<T>>> Just(const ref<T>& x);
template <typename T,
          typename U,
          typename = core::enable_for_argument_data_type_t<U, T>>
ref<T> FromMaybe(const ref<maybe<ref<T>>>& x, const U& def);

template <typename T> ref<T> FromMaybe(const ref<maybe<ref<T>>>& x);

/// \}
} // dataflow

#include "maybe.inl"

#endif // DATAFLOW___MAYBE_H
