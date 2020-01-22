
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
#include "Context.h"

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

ref<std::shared_ptr<QObject>> CreateOutput(const ref<QPointF>& circle_pos,
                                           var<int> lmb_pressed,
                                           var<QPointF> mouse_pos)
{
  const auto p_output = std::make_shared<Context>(lmb_pressed, mouse_pos);

  class policy
  {
  public:
    policy(const std::shared_ptr<Context>& p_output)
    : p_output_(p_output)
    {
    }

    std::string label() const
    {
      return "set-context-property";
    }

    std::shared_ptr<QObject> calculate(const QPointF& pos) const
    {
      p_output_->setCirclePos(pos);
      return p_output_;
    };

  private:
    const std::shared_ptr<Context> p_output_;
  };

  return Lift(policy(p_output), circle_pos);
}

ref<std::shared_ptr<QObject>>
SetContextProperty(QQmlContext* p_context,
                   const std::string& name,
                   const ref<std::shared_ptr<QObject>>& value)
{
  class policy
  {
  public:
    policy(QQmlContext* p_context, const std::string& name)
    : p_context_(p_context)
    , name_(QString::fromUtf8(name.c_str()))
    {
    }

    std::string label() const
    {
      return "set-context-property";
    }

    std::shared_ptr<QObject>
    calculate(const std::shared_ptr<QObject>& p_object) const
    {
      p_context_->setContextProperty(name_, p_object.get());
      return p_object;
    };

  private:
    QQmlContext* p_context_;
    const QString name_;
  };

  return Lift(policy(p_context, name), value);
}

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

  auto mouse_pos = Var<QPointF>();
  auto lmb_pressed = Var<int>();

  auto x = Main([&](dtime t0) {
    return AdjustableCirclePosition(
      QPointF(100, 100), 50.0, mouse_pos, lmb_pressed, t0);
  });

  QQmlContext context(qml_engine.rootContext());

  const auto p_context = &context;

  const auto output = CreateOutput(x, lmb_pressed, mouse_pos);

  const auto m = Main(SetContextProperty(p_context, "context", output));

  QQmlComponent qml_component(&qml_engine, QUrl("qrc:/main.qml"));

  if (qml_component.isError())
  {
    std::cout << "Error loading QML file: "
              << qml_component.errorString().toStdString();
    return -1;
  }

  std::unique_ptr<QObject> p_component(qml_component.create(p_context));

  return app.exec();
}
