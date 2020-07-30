
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

#ifndef DATAFLOW___QT_H
#define DATAFLOW___QT_H

#include "dataflow-qt_export.h"

#include <dataflow/list.h>
#include <dataflow/prelude.h>
#include <dataflow/qt/conversion.h>
#include <dataflow/qt/qml_data.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace dataflow
{

/// \defgroup qt
/// \{

class DATAFLOW_QT_EXPORT EngineQml : public Engine
{
public:
  explicit EngineQml(const QCoreApplication& app);
  ~EngineQml();

  static EngineQml& instance();

  QQmlEngine& GetQmlEngine();

private:
  const QCoreApplication& app_;
  QQmlEngine qml_engine_;
};

namespace qt
{
template <typename T>
std::pair<std::string, var<T>> QmlPropertyRW(const std::string& name,
                                             var<T>& x);

std::pair<std::string, sig> QmlFunction(const std::string& name, const sig& x);

template <typename T>
std::pair<std::string, ref<T>> QmlProperty(const std::string& name,
                                           const ref<T>& x);

template <typename... Refs>
ref<qml_data> QmlContext(const std::pair<std::string, Refs>&... props);

template <typename T> ref<qml_data> QmlData(const ref<listC<T>>& xs);

DATAFLOW_QT_EXPORT ref<qml_data> QmlComponent(const arg<std::string>& qml_url,
                                              const arg<qml_data>& context);
}

/// \}
} // dataflow

#include "qt.inl"

#endif // DATAFLOW___QT_H
