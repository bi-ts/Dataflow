
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

#pragma once

#include "dataflow-qt_export.h"

#include "qhandle.h"

#include <QtCore/QVariant>

namespace dataflow2qt
{
/// A flowable wrapper for `QVariant`.
///
class DATAFLOW_QT_EXPORT qvariant
{
  template <typename FwT> friend qvariant cast_to_qvariant(const FwT& v);

public:
  qvariant();

  const QVariant& get() const;

  bool operator==(const qvariant& other) const;
  bool operator!=(const qvariant& other) const;

private:
  explicit qvariant(const qobject& obj);
  explicit qvariant(const QVariant& value);

private:
  qobject obj_;
  QVariant value_;
};

DATAFLOW_QT_EXPORT std::ostream& operator<<(std::ostream& out,
                                            const qvariant& v);
}
