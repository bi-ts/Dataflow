
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

#include <dataflow/qt/qml_data.h>

namespace dataflow
{
namespace qt
{
qml_data::qml_data() = default;

qml_data::operator const QVariant&() const
{
  return value_;
}

const std::shared_ptr<QObject> qml_data::to_qobject() const
{
  return p_qobject_;
}

bool qml_data::operator==(const qml_data& other) const
{
  return value_ == other.value_;
}

bool qml_data::operator!=(const qml_data& other) const
{
  return !(*this == other);
}
}

std::ostream& qt::operator<<(std::ostream& out, const qml_data& v)
{
  // TODO: implement
  return out;
}
}
