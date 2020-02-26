
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

#define DATAFLOW___NO_BOOST

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>
#include <dataflow/qt.h>
#include <dataflow/tuple.h>

#include <QtCore/QObject>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickWindow>
#include <QtWidgets/QApplication>

#include <cmath>
#include <iostream>
#include <memory>

using namespace dataflow;

enum class mode
{
  idle,
  active
};

std::ostream& operator<<(std::ostream& out, mode value)
{
  out << "mode::" << (value == mode::idle ? "idle" : "active");
  return out;
}

using point = vec2<double>;

ref<point> AdjustableCirclePosition(const arg<point>& initial_circle_pos,
                                    const arg<double>& radius,
                                    const arg<point>& mouse_pos,
                                    const arg<int>& mouse_pressed,
                                    dtime t0)
{
  const auto mouse_down =
    mouse_pressed - Prev(mouse_pressed(t0), mouse_pressed, t0);

  const auto s = StateMachine(
    TupleC(mode::idle, initial_circle_pos),
    [=](const ref<tuple<mode, point>>& sp) {
      auto prev_mode = First(sp);
      auto circle_pos = Second(sp);

      return Transitions(
        On(prev_mode == mode::idle && mouse_down == 1 &&
             PointsClose(mouse_pos, circle_pos, radius),
           [=](dtime t0) {
             const auto mouse_shift = mouse_pos - mouse_pos(t0);
             return TupleC(mode::active, circle_pos(t0) + mouse_shift);
           }),
        On(prev_mode == mode::active && mouse_down == -1,
           TupleC(mode::idle, circle_pos)));
    },
    t0);

  return Second(s);
}

ref<std::shared_ptr<QObject>>
QmlComponent(const arg<std::string>& qml_url,
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
      const auto& p_qml_component = std::make_shared<QQmlComponent>(
        &EngineQml::instance().GetQmlEngine(),
        QUrl(QString::fromUtf8(qml_url.c_str())));

      if (p_qml_component->isError())
      {
        std::cout << "Error loading QML file: "
                  << p_qml_component->errorString().toStdString();
        throw std::runtime_error("Can't create qml component");
      }

      const auto& p_qml_context = std::make_shared<QQmlContext>(
        EngineQml::instance().GetQmlEngine().rootContext());

      p_qml_context->setContextProperty("context", p_context.get());

      const auto& p_object =
        std::shared_ptr<QObject>(p_qml_component->create(p_qml_context.get()));

      return component_data(p_qml_context, p_object);
    };
  };

  return Second(core::Lift<policy>(qml_url, context));
}

int main(int argc, char* p_argv[])
{
  QApplication app(argc, p_argv);

  EngineQml engine(app);

  // TODO: Allow these variables to be local in main
  auto mouse_pos = Var<point>();
  auto lmb_pressed = Var<int>();
  const auto circle_radius = Const(50.0);

  // TODO: make sure `Main()` is called only once
  const auto m = Main([&](dtime t0) {
    const auto circle_pos = AdjustableCirclePosition(
      point(100, 100), circle_radius, mouse_pos, lmb_pressed, t0);

    const auto context =
      qt::QmlContext(qt::RW(qt::QmlPropertyRW("mousePressed", lmb_pressed),
                            qt::QmlPropertyRW("mousePos", mouse_pos)),
                     qt::QmlProperty("circleRadius", circle_radius),
                     qt::QmlProperty("circlePos", circle_pos));

    return QmlComponent("qrc:/main.qml", context);
  });

  return app.exec();
}
