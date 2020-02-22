
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

#include <dataflow/geometry.h>

#include <QtCore/QVariant>

namespace dataflow
{
namespace qt
{
namespace internal
{
DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(bool v);

DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(int v);

DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(double v);
DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(float v);

DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(const std::string& v);

DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(const vec2<double>& v);
DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(const vec2<float>& v);
DATAFLOW_QT_EXPORT QVariant convert_to_qml_type(const vec2<int>& v);

template <typename T> T convert_from_qml_type(QVariant v);

template <> DATAFLOW_QT_EXPORT bool convert_from_qml_type(QVariant v);

template <> DATAFLOW_QT_EXPORT int convert_from_qml_type(QVariant v);

template <> DATAFLOW_QT_EXPORT double convert_from_qml_type(QVariant v);
template <> DATAFLOW_QT_EXPORT float convert_from_qml_type(QVariant v);

template <> DATAFLOW_QT_EXPORT std::string convert_from_qml_type(QVariant v);

template <> DATAFLOW_QT_EXPORT vec2<double> convert_from_qml_type(QVariant v);
}
}
}
