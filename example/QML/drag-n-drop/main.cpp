
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

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>
#include <dataflow/qt.h>
#include <dataflow/tuple.h>

#include <QtQml/QQmlEngine>
#include <QtWidgets/QApplication>

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

ref<point> DragNDrop(const arg<point>& initial_circle_pos,
                     const arg<double>& radius,
                     const arg<point>& mouse_pos,
                     const arg<int>& mouse_pressed,
                     dtime t0)
{
  const auto mouse_down = Diff(mouse_pressed, t0);

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

int main(int argc, char* p_argv[])
{
  QApplication app(argc, p_argv);

  EngineQml engine(app);

  // TODO: make sure `Main()` is called only once
  const auto m = Main([&](dtime t0) {
    auto mouse_pos = Var<point>();
    auto lmb_pressed = Var<int>();
    auto view = Var("view1.qml");

    const auto circle_radius = Const(50.0);

    const auto circle_pos =
      DragNDrop(point(100, 100), circle_radius, mouse_pos, lmb_pressed, t0);

    const auto context =
      qt::QmlContext(qt::QmlPropertyRW("mousePressed", lmb_pressed),
                     qt::QmlPropertyRW("mousePos", mouse_pos),
                     qt::QmlPropertyRW("view", view),
                     qt::QmlProperty("circleRadius", circle_radius),
                     qt::QmlProperty("circlePos", circle_pos));

    return qt::QmlComponent("qrc:/" + view, context);
  });

  return app.exec();
}
