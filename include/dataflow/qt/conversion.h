
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

#include <dataflow/geometry.h>
#include <dataflow/utility/std_future.h>

#include <QtCore/QVariant>

namespace dataflow
{
namespace qt
{
template <typename T> struct converter;

template <> struct DATAFLOW_QT_EXPORT converter<bool>
{
  static QVariant to_qml_type(bool v);
  static bool from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<double>
{
  static QVariant to_qml_type(double v);
  static double from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<float>
{
  static QVariant to_qml_type(float v);
  static float from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<int>
{
  static QVariant to_qml_type(int v);
  static int from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<std::string>
{
  static QVariant to_qml_type(const std::string& v);
  static std::string from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<vec2<double>>
{
  static QVariant to_qml_type(const vec2<double>& v);
  static vec2<double> from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<vec2<float>>
{
  static QVariant to_qml_type(const vec2<float>& v);
  static vec2<float> from_qml_type(const QVariant& v);
};

template <> struct DATAFLOW_QT_EXPORT converter<vec2<int>>
{
  static QVariant to_qml_type(const vec2<int>& v);
  static vec2<int> from_qml_type(const QVariant& v);
};

template <typename TQObject> struct converter<qhandle<TQObject>>
{
  static QVariant to_qml_type(const qhandle<TQObject>& v)
  {
    return QVariant::fromValue(v.get());
  }
};

namespace detail
{
template <typename T> struct is_convertible_to_qml_type
{
private:
  template <typename TT,
            typename = decltype(converter<TT>::to_qml_type(std::declval<TT>()))>
  static std::true_type test_(const TT*);

  static std::false_type test_(...);

public:
  using type = decltype(test_(std::declval<const T*>()));
};
}

template <typename T>
using is_convertible_to_qml_type =
  typename detail::is_convertible_to_qml_type<std20::remove_cvref_t<T>>::type;

template <typename T>
typename std::enable_if<is_convertible_to_qml_type<T>::value, QVariant>::type
convert_to_qml_type(const T& v)
{
  return converter<T>::to_qml_type(v);
}

}
}
