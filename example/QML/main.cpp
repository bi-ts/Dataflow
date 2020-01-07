
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
#include "Input.h"
#include "Output.h"

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>
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

ref<QPointF> operator+(const ref<QPointF>& a, const ref<QPointF>& b)
{
  struct policy
  {
    static std::string label()
    {
      return "+";
    }
    QPointF calculate(const QPointF& a, const QPointF& b)
    {
      return a + b;
    }
  };
  return core::Lift<policy>(a, b);
}

ref<double> Distance(const ref<QPointF>& a, const ref<QPointF>& b)
{
  struct policy
  {
    static std::string label()
    {
      return "distance";
    }
    double calculate(const QPointF& a, const QPointF& b)
    {
      const QPointF v = a - b;
      return std::sqrt(v.x() * v.x() + v.y() * v.y());
    }
  };
  return core::Lift<policy>(a, b);
}

ref<QPointF> AdjustableCirclePosition(const arg<QPointF>& initial_circle_pos,
                                      const arg<double>& radius,
                                      const arg<QPointF>& mouse_pos,
                                      const arg<int>& mouse_pressed,
                                      dtime t0)
{
  const auto mouse_down =
    mouse_pressed - Prev(mouse_pressed(t0), mouse_pressed, t0);

  const auto s = StateMachine(
    TupleC(mode::idle, initial_circle_pos),
    [=](const ref<tuple<mode, QPointF>>& sp) {
      auto prev_mode = First(sp);
      auto circle_pos = Second(sp);

      return Transitions(
        On(prev_mode == mode::idle && mouse_down == 1 &&
             Distance(mouse_pos, circle_pos) < radius,
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

int main(int argc, char* p_argv[])
{
  QApplication app(argc, p_argv);

  QQmlEngine qml_engine;

  Engine engine;

  Input input;

  std::function<void()> update = []() {};

  auto x = Main([&](dtime t0) {
    auto pos = AdjustableCirclePosition(QPointF(100, 100),
                                        50.0,
                                        input.mouse_pos,
                                        input.left_mouse_button_pressed,
                                        t0);

    return core::Lift("update", pos, [&](const QPointF& pos) {
      update();
      return pos;
    });
  });

  Output output(x);

  update = [&]() { emit output.circlePosChanged(); };

  qml_engine.rootContext()->setContextProperty("input", &input);
  qml_engine.rootContext()->setContextProperty("output", &output);

  QQmlComponent qml_component(&qml_engine, QUrl("qrc:/main.qml"));

  if (qml_component.isError())
  {
    std::cout << "Error loading QML file: "
              << qml_component.errorString().toStdString();
    return -1;
  }

  std::unique_ptr<QObject> p_component(qml_component.create());

  return app.exec();
}
