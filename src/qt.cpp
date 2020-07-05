
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

#include "qt/internal/qobject_factory.h"
#include <dataflow/qt.h>
#include <dataflow/tuple.h>

#include <QtCore/QTimer>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>

namespace dataflow
{
EngineQml::EngineQml(const QCoreApplication& app)
: app_{app}
{
}

EngineQml::~EngineQml()
{
  QTimer::singleShot(0, &app_, QCoreApplication::quit);

  app_.exec();

  qt::internal::qobject_factory::on_shutdown();
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

ref<qt::qml_data> qt::QmlComponent(const arg<std::string>& qml_url,
                                   const arg<qml_data>& context)
{
  using component_data = tupleC<std::shared_ptr<QQmlContext>, qml_data>;

  class policy
  {
  public:
    std::string label() const
    {
      return "qml-component";
    }

    component_data calculate(const std::string& qml_url,
                             const qml_data& context)
    {
      QQmlComponent qml_component_factory{
        &EngineQml::instance().GetQmlEngine(),
        QUrl(QString::fromUtf8(qml_url.c_str()))};

      if (qml_component_factory.isError())
      {
        throw std::runtime_error(
          "Can't create qml component: " +
          qml_component_factory.errorString().toStdString());
      }

      const auto p_qml_context = internal::qobject_factory::create<QQmlContext>(
        EngineQml::instance().GetQmlEngine().rootContext());

      p_qml_context->setContextProperty("view_context", context);

      const auto p_qml_component = internal::qobject_factory::make_shared(
        qml_component_factory.create(p_qml_context.get()));

      return component_data{p_qml_context, p_qml_component};
    };
  };

  return Second(core::Lift<policy>(qml_url, context));
}
}
