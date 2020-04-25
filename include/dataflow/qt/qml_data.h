
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

#pragma once

#include "dataflow-qt_export.h"

#include "conversion.h"

#include <QtCore/QVariant>

namespace dataflow
{
namespace qt
{
class DATAFLOW_QT_EXPORT qml_data
{
public:
  qml_data();

  template <typename T>
  qml_data(const std::shared_ptr<T>& p_qobject)
  : p_qobject_{p_qobject}
  , value_{QVariant::fromValue(p_qobject.get())}
  {
  }

  template <typename T,
            typename...,
            typename = typename std::enable_if<
              is_convertible_to_qml_type<T>::value>::type>
  qml_data(const T& value)
  : p_qobject_{}
  , value_{QVariant::fromValue(convert_to_qml_type(value))}
  {
  }

  operator const QVariant&() const;

  const std::shared_ptr<QObject> to_qobject() const;

  bool operator==(const qml_data& other) const;
  bool operator!=(const qml_data& other) const;

private:
  std::shared_ptr<QObject> p_qobject_;
  QVariant value_;
};

DATAFLOW_QT_EXPORT std::ostream& operator<<(std::ostream& out,
                                            const qml_data& v);

template <typename X> ref<qml_data> QmlData(const ref<X>& x)
{
  struct policy
  {
    std::string label() const
    {
      return "qml-data";
    }

    static qml_data calculate(const X& x)
    {
      return qml_data{x};
    }
  };

  return core::Lift<policy>(x);
}
}
}
