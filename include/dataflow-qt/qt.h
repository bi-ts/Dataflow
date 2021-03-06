
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

#ifndef DATAFLOW_QT_H
#define DATAFLOW_QT_H

#include "dataflow-qt_export.h"

#include <dataflow-qt/qt/conversion.h>

#include <dataflow/list.h>
#include <dataflow/prelude.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

namespace dataflow2qt
{

/// \defgroup dataflow2qt
/// \{

class DATAFLOW_QT_EXPORT EngineQml : public dataflow::Engine
{
public:
  explicit EngineQml(const QCoreApplication& app);
  ~EngineQml();

  static EngineQml& instance();

  QQmlEngine& GetQmlEngine();

private:
  virtual dataflow::ref<bool>
  timeout_(const dataflow::ref<dataflow::integer>& interval_msec,
           dataflow::dtime t0) override;

private:
  const QCoreApplication& app_;
  QQmlEngine qml_engine_;
};

template <typename T>
std::pair<std::string, dataflow::var<T>> QmlPropertyRW(const std::string& name,
                                                       dataflow::var<T>& x);

std::pair<std::string, dataflow::sig> QmlFunction(const std::string& name,
                                                  const dataflow::sig& x);

template <typename T,
          typename...,
          typename =
            typename std::enable_if<is_convertible_to_qml_type<T>::value>::type>
std::pair<std::string, dataflow::ref<T>> QmlProperty(const std::string& name,
                                                     const dataflow::ref<T>& x);

DATAFLOW_QT_EXPORT std::pair<std::string, dataflow::ref<qobject>>
QmlProperty(const std::string& name,
            const dataflow::ref<dataflow::listC<qvariant>>& xs);

template <typename T>
std::pair<std::string, dataflow::ref<qobject>>
QmlProperty(const std::string& name,
            const dataflow::ref<dataflow::listC<T>>& xs);

template <typename... Refs>
dataflow::ref<qobject> QmlContext(const std::pair<std::string, Refs>&... props);

DATAFLOW_QT_EXPORT dataflow::ref<qobject>
QmlComponent(const dataflow::arg<std::string>& qml_url,
             const dataflow::arg<qobject>& context);
/// \}
} // dataflow2qt

#include "qt.inl"

#endif // DATAFLOW_QT_H
