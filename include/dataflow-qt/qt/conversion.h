
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
#include "qvariant.h"

#include <dataflow/geometry.h>
#include <dataflow/utility/std_future.h>

#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace dataflow2qt
{
template <typename FwT> struct converter
{
};

template <> struct DATAFLOW_QT_EXPORT converter<bool>
{
  static bool to_qml_type(bool v);
  static bool from_qml_type(bool v);
};

template <> struct DATAFLOW_QT_EXPORT converter<double>
{
  static double to_qml_type(double v);
  static double from_qml_type(double v);
};

template <> struct DATAFLOW_QT_EXPORT converter<float>
{
  static float to_qml_type(float v);
  static float from_qml_type(float v);
};

template <> struct DATAFLOW_QT_EXPORT converter<int>
{
  static int to_qml_type(int v);
  static int from_qml_type(int v);
};

template <> struct DATAFLOW_QT_EXPORT converter<std::string>
{
  static QString to_qml_type(const std::string& v);
  static std::string from_qml_type(const QString& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<dataflow::vec2<double>>
{
  static QPointF to_qml_type(const dataflow::vec2<double>& v);
  static dataflow::vec2<double> from_qml_type(const QPointF& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<dataflow::vec2<float>>
{
  static QPointF to_qml_type(const dataflow::vec2<float>& v);
  static dataflow::vec2<float> from_qml_type(const QPointF& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<dataflow::vec2<int>>
{
  static QPointF to_qml_type(const dataflow::vec2<int>& v);
  static dataflow::vec2<int> from_qml_type(const QPointF& v);
};

template <typename TQObject> struct converter<qhandle<TQObject>>
{
  static QVariant to_qml_type(const qhandle<TQObject>& v)
  {
    return QVariant::fromValue(v.get());
  }
};

template <> struct DATAFLOW_QT_EXPORT converter<qvariant>
{
  static QVariant to_qml_type(const qvariant& v);
};

namespace detail
{
template <typename FwT> struct is_convertible_to_qml_type
{
private:
  template <typename T,
            typename = decltype(converter<T>::to_qml_type(std::declval<T>()))>
  static std::true_type test_(const T*);

  static std::false_type test_(...);

public:
  using type = decltype(test_(std::declval<const FwT*>()));
};
}

/// A type trait that checks whether flowable type `FwT` is convertible to a
/// type that can be used from QML.
///
template <typename FwT>
using is_convertible_to_qml_type = typename detail::is_convertible_to_qml_type<
  dataflow::std20::remove_cvref_t<FwT>>::type;

template <
  typename FwT,
  typename...,
  typename QmlT = decltype(converter<FwT>::to_qml_type(std::declval<FwT>()))>
QmlT cast_to_qml_type(const FwT& v);

template <
  typename FwT,
  typename...,
  typename QmlT,
  typename = decltype(converter<FwT>::from_qml_type(std::declval<QmlT>()))>
FwT cast_from_qml_type(const QmlT& v);

template <typename FwT> qvariant cast_to_qvariant(const FwT& v);

template <typename TQObject>
qvariant cast_to_qvariant(const qhandle<TQObject>& v);

DATAFLOW_QT_EXPORT qvariant cast_to_qvariant(const qvariant& v);
}

template <typename FwT, typename..., typename QmlT>
QmlT dataflow2qt::cast_to_qml_type(const FwT& v)
{
  return converter<FwT>::to_qml_type(v);
}

template <typename FwT, typename..., typename QmlT, typename>
FwT dataflow2qt::cast_from_qml_type(const QmlT& v)
{
  return converter<FwT>::from_qml_type(v);
}

template <typename FwT>
dataflow2qt::qvariant dataflow2qt::cast_to_qvariant(const FwT& v)
{
  static_assert(is_convertible_to_qml_type<FwT>::value,
                "A type convertible to qml is expected");

  return qvariant{cast_to_qml_type(v)};
}

template <typename TQObject>
dataflow2qt::qvariant dataflow2qt::cast_to_qvariant(const qhandle<TQObject>& v)
{
  return qvariant{v};
}
