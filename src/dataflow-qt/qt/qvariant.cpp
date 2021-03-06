
//  Copyright (c) 2014 - 2021 Maksym V. Bilinets.
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

#include <dataflow-qt/qt/qvariant.h>

namespace dataflow2qt
{
qvariant::qvariant() = default;

const QVariant& qvariant::get() const
{
  return value_;
}

bool qvariant::operator==(const qvariant& other) const
{
  return value_ == other.value_;
}

bool qvariant::operator!=(const qvariant& other) const
{
  return !(*this == other);
}

qvariant::qvariant(const qobject& obj)
: obj_{obj}
, value_{QVariant::fromValue(obj.get())}
{
}

qvariant::qvariant(const QVariant& value)
: obj_{}
, value_{value}
{
}
}

std::ostream& dataflow2qt::operator<<(std::ostream& out, const qvariant& v)
{
  // TODO: implement
  return out;
}
