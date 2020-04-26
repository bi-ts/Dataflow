
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

#include "qt/internal/qobject_deleter.h"
#include <dataflow/qt.h>
#include <dataflow/tuple.h>

#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

namespace dataflow
{
EngineQml::EngineQml(const QCoreApplication& app)
{
}

EngineQml& EngineQml::instance()
{
  if (!Engine::engine_())
    throw std::logic_error("No Engine instance available");

  if (!dynamic_cast<EngineQml*>(Engine::engine_()))
    throw std::logic_error("The engine is not EngineQml");

  return *static_cast<EngineQml*>(Engine::engine_());
}

QQmlEngine& EngineQml::GetQmlEngine()
{
  return qml_engine_;
}

ref<std::shared_ptr<QObject>>
qt::QmlComponent(const arg<std::string>& qml_url,
                 const arg<std::shared_ptr<QObject>>& context)
{
  using component_data =
    tupleC<std::shared_ptr<QQmlContext>, std::shared_ptr<QObject>>;

  class policy
  {
  public:
    std::string label() const
    {
      return "qml-component";
    }

    component_data calculate(const std::string& qml_url,
                             const std::shared_ptr<QObject>& p_context)
    {
      QQmlComponent qml_component_factory{
        &EngineQml::instance().GetQmlEngine(),
        QUrl(QString::fromUtf8(qml_url.c_str()))};

      if (qml_component_factory.isError())
      {
        throw std::runtime_error("Can't create qml component");
      }

      const std::shared_ptr<QQmlContext> p_qml_context{
        new QQmlContext{EngineQml::instance().GetQmlEngine().rootContext()},
        qt::internal::qobject_deleter{}};

      p_qml_context->setContextProperty("view_context", p_context.get());

      const auto p_qml_component = std::shared_ptr<QObject>{
        qml_component_factory.create(p_qml_context.get()),
        qt::internal::qobject_deleter{}};


      return component_data{p_qml_context, p_qml_component};
    };
  };

  return Second(core::Lift<policy>(qml_url, context));
}
}
