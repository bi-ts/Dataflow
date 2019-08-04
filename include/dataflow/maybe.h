
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

template <typename T> class maybe
{
public:
  explicit maybe();

  explicit maybe(const ref<T>& x);

  bool engaged() const;

  ref<T> value_or(const ref<T>& def) const;

  bool operator==(const maybe<T>& other) const;
  bool operator!=(const maybe<T>& other) const;

private:
  union data;

  data data_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const maybe<T>& value)
{
  if (value.engaged())
  {
  }
  else
  {
    out << "nothing";
  }

  return out;
}

/// \}
} // dataflow

#include "maybe.inl"

#endif // DATAFLOW___MAYBE_H
