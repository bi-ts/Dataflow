
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

#include <dataflow/qt/internal/conversion.h>

#include <QtCore/QPoint>
#include <QtCore/QString>

namespace dataflow
{
namespace qt
{
QVariant internal::convert_to_qml_type(bool v)
{
  return QVariant::fromValue(v);
}

QVariant internal::convert_to_qml_type(int v)
{
  return QVariant::fromValue(v);
}

QVariant internal::convert_to_qml_type(double v)
{
  return QVariant::fromValue(v);
}

QVariant internal::convert_to_qml_type(float v)
{
  return QVariant::fromValue(v);
}

QVariant internal::convert_to_qml_type(const std::string& v)
{
  return QVariant::fromValue(QString::fromUtf8(v.c_str()));
}

QVariant internal::convert_to_qml_type(const vec2<double>& v)
{
  return QVariant::fromValue(
    QPointF(static_cast<qreal>(v.x()), static_cast<qreal>(v.y())));
}

QVariant internal::convert_to_qml_type(const vec2<float>& v)
{
  return QVariant::fromValue(
    QPointF(static_cast<qreal>(v.x()), static_cast<qreal>(v.y())));
}

QVariant internal::convert_to_qml_type(const vec2<int>& v)
{
  return QVariant::fromValue(
    QPointF(static_cast<qreal>(v.x()), static_cast<qreal>(v.y())));
}

template <> bool internal::convert_from_qml_type(QVariant v)
{
  return v.toBool();
}

template <> int internal::convert_from_qml_type(QVariant v)
{
  return v.toInt();
}

template <> double internal::convert_from_qml_type(QVariant v)
{
  return v.toDouble();
}

template <> float internal::convert_from_qml_type(QVariant v)
{
  return v.toFloat();
}

template <> std::string internal::convert_from_qml_type(QVariant v)
{
  return v.toString().toStdString();
}

template <> vec2<double> internal::convert_from_qml_type(QVariant v)
{
  const auto& pt = v.toPointF();
  return vec2<double>(static_cast<double>(pt.x()), static_cast<double>(pt.y()));
}

template <> vec2<float> internal::convert_from_qml_type(QVariant v)
{
  const auto& pt = v.toPointF();

  return vec2<float>(static_cast<float>(pt.x()), static_cast<float>(pt.y()));
}

template <> vec2<int> internal::convert_from_qml_type(QVariant v)
{
  const auto& pt = v.toPointF();

  return vec2<int>(static_cast<int>(pt.x()), static_cast<int>(pt.y()));
}
}
}
