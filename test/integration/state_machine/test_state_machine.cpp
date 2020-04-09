
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

#include "../../tools/graph_invariant.h"

#include <dataflow/geometry.h>
#include <dataflow/introspect.h>
#include <dataflow/macro.h>
#include <dataflow/prelude.h>
#include <dataflow/tuple.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

namespace
{
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

using point = vec2<int>;

DATAFLOW_COMPOSITE(dnd_state, DragNDropState, (mode, Mode), (point, CirclePos));

inline ref<dnd_state> Idle(const arg<point>& circle_pos)
{
  return DragNDropState(mode::idle, circle_pos);
}

inline ref<dnd_state> Active(const arg<point>& circle_pos)
{
  return DragNDropState(mode::active, circle_pos);
}

}

ref<point> DragNDropCircle(const arg<point>& initial_circle_pos,
                           const arg<int>& radius,
                           const arg<point>& mouse_pos,
                           const arg<int>& mouse_pressed,
                           dtime t0)
{
  // TODO: add Diff(d0, x, t0) function
  const auto mouse_down =
    mouse_pressed - Prev(mouse_pressed(t0), mouse_pressed, t0);

  const auto s = StateMachine(
    Idle(initial_circle_pos),
    [=](const ref<dnd_state>& sp) {
      auto prev_mode = Mode(sp);
      auto prev_circle_pos = CirclePos(sp);

      return Transitions(On(prev_mode == mode::idle && mouse_down == 1 &&
                              PointsClose(mouse_pos, prev_circle_pos, radius),
                            [=](dtime t0) {
                              return Active(prev_circle_pos(t0) +
                                            (mouse_pos - mouse_pos(t0)));
                            }),
                         On(prev_mode == mode::active && mouse_down == -1,
                            Idle(prev_circle_pos)));
    },
    t0);

  return CirclePos(s);
}

BOOST_AUTO_TEST_SUITE(test_integration_state_machine)

BOOST_AUTO_TEST_CASE(test_drag_and_drop)
{
  Engine engine;

  auto mouse_pressed = Var<int>(false);
  auto mouse_pos = Var(point(0, 0));

  auto f = Main([mouse_pos = mouse_pos.as_ref(),
                 mouse_pressed = mouse_pressed.as_ref()](dtime t0) {
    return DragNDropCircle(point(100, 100), 30, mouse_pos, mouse_pressed, t0);
  });

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*f, (point(100, 100)));

  mouse_pos = point(90, 110);

  BOOST_CHECK(graph_invariant_holds());

  mouse_pressed = true;

  BOOST_CHECK(graph_invariant_holds());

  mouse_pos = point(200, 100);

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*f, point(210, 90));

  mouse_pos = point(150, 150);

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*f, point(160, 140));

  mouse_pressed = false;

  BOOST_CHECK(graph_invariant_holds());

  mouse_pos = point(200, 100);

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*f, point(160, 140));

  mouse_pos = point(90, 110);

  BOOST_CHECK(graph_invariant_holds());

  mouse_pressed = true;

  BOOST_CHECK(graph_invariant_holds());

  mouse_pos = point(200, 100);

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*f, point(160, 140));

  mouse_pressed = false;

  BOOST_CHECK(graph_invariant_holds());

  mouse_pos = point(150, 150);

  BOOST_CHECK(graph_invariant_holds());

  mouse_pressed = true;

  BOOST_CHECK(graph_invariant_holds());

  mouse_pos = point(170, 170);

  BOOST_CHECK(graph_invariant_holds());

  BOOST_CHECK_EQUAL(*f, point(180, 160));
}

BOOST_AUTO_TEST_SUITE_END()
}
