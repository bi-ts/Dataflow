
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

#include <dataflow/prelude/core/dtime.h>

#include "../../internal/engine.h"

namespace dataflow
{
dtimestamp::dtimestamp()
: timestamp_()
{
}

dtimestamp::dtimestamp(const Time& t)
: timestamp_(internal::pumpa::instance().ticks())
{
}

dtimestamp::operator std::size_t() const
{
  return timestamp_;
}

bool dtimestamp::operator==(const dtimestamp& other) const
{
  return timestamp_ == other.timestamp_;
}

bool dtimestamp::operator!=(const dtimestamp& other) const
{
  return !(*this == other);
}
} // dataflow

std::ostream& dataflow::operator<<(std::ostream& out, const dtimestamp& value)
{
  return out << static_cast<std::size_t>(value);
}
