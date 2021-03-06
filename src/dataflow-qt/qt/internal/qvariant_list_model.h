
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

#include <dataflow-qt/qt/conversion.h>
#include <dataflow/list.h>

#include <QtCore/QAbstractListModel>

namespace dataflow2qt
{
namespace internal
{
class qvariant_list_model : public QAbstractListModel
{
  Q_OBJECT

public:
  qvariant_list_model(dataflow::list<qvariant> data,
                      QObject* p_parent = nullptr);

  int rowCount(const QModelIndex& index) const override;
  QVariant data(const QModelIndex& index, int role) const override;

  const dataflow::list<qvariant>& list_data() const
  {
    return data_;
  }

  void insert(int idx, const qvariant& v)
  {
    beginInsertRows(QModelIndex(), idx, idx);

    data_ = data_.insert(idx, v);

    endInsertRows();
  }

  void erase(int idx)
  {
    beginRemoveRows(QModelIndex(), idx, idx);

    data_ = data_.erase(idx);

    endRemoveRows();
  }

private:
  dataflow::list<qvariant> data_;
};
}
}
