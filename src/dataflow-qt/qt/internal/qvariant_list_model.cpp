
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

#include "qvariant_list_model.h"

#include <QtCore/QPoint>

namespace dataflow2qt
{
namespace internal
{
qvariant_list_model::qvariant_list_model(dataflow::list<qvariant> data,
                                         QObject* p_parent)
: QAbstractListModel{p_parent}
, data_{std::move(data)}
{
}

int qvariant_list_model::rowCount(const QModelIndex& index) const
{
  return static_cast<int>(data_.size());
}

QVariant qvariant_list_model::data(const QModelIndex& index, int role) const
{
  const auto idx = static_cast<std::size_t>(index.row());

  if (idx < data_.size())
    return cast_to_qml_type(data_[idx]);

  return {};
}
}
}
