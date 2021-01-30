
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

#include <dataflow/list.h>
#include <dataflow/macro.h>
#include <dataflow/prelude.h>
#include <dataflow/qt.h>

#include <QtWidgets/QApplication>

using namespace dataflow;

enum class game_state
{
  running,
  paused,
  ended
};

std::ostream& operator<<(std::ostream& out, game_state v)
{
  switch (v)
  {
  case game_state::running:
    return out << "game_state::running";
  case game_state::paused:
    return out << "game_state::paused";
  case game_state::ended:
    return out << "game_state::ended";
  };

  return out;
};

DATAFLOW_COMPOSITE(game,
                   Game,
                   (vec2<int>, SnakeDir),
                   (list<vec2<int>>, SnakeBody),
                   (game_state, CurrentState),
                   (bool, Tick));

ref<game> InitialGame(const ref<integer>& timeout, dtime t0)
{
  return Game(Vec2(0, -1),
              ListC(Vec2(15, 15),
                    Vec2(15, 16),
                    Vec2(15, 17),
                    Vec2(14, 17),
                    Vec2(13, 17),
                    Vec2(13, 18),
                    Vec2(13, 19),
                    Vec2(12, 19),
                    Vec2(11, 19)),
              game_state::running,
              Timeout(timeout, t0));
}

ref<game> GameState(const sig& turn_east,
                    const sig& turn_north,
                    const sig& turn_south,
                    const sig& turn_west,
                    const sig& toggle_pause,
                    const sig& restart_game,
                    const ref<vec2<int>>& field_size,
                    dtime t0)
{
  const auto timeout = Const(100);

  return StateMachine(
    InitialGame(timeout, t0),
    [=](const ref<game>& prev_game) {
      const auto prev_snake_body = SnakeBody(prev_game);
      const auto prev_snake_dir = SnakeDir(prev_game);

      const auto requested_dir = Switch(turn_north >>= Vec2(0, -1),
                                        turn_east >>= Vec2(1, 0),
                                        turn_south >>= Vec2(0, 1),
                                        turn_west >>= Vec2(-1, 0),
                                        Default(prev_snake_dir));

      const auto step = Tick(prev_game) || (prev_snake_dir != -requested_dir &&
                                            prev_snake_dir != requested_dir);

      const auto snake_dir = If(step, requested_dir, prev_snake_dir);

      const auto next_head_position = FromMaybe(prev_snake_body[0]) + snake_dir;

      const auto next_head_position_is_bad =
        next_head_position.x() < 0 || next_head_position.y() < 0 ||
        next_head_position.x() >= field_size.x() ||
        next_head_position.y() >= field_size.y();

      return Transitions(
        On(restart_game, [=](dtime t0) { return InitialGame(timeout, t0); }),
        On(CurrentState(prev_game) == game_state::running && step &&
             next_head_position_is_bad,
           Game(snake_dir, prev_snake_body, game_state::ended, false)),
        On(CurrentState(prev_game) == game_state::running && toggle_pause,
           Game(prev_snake_dir, prev_snake_body, game_state::paused, false)),
        On(CurrentState(prev_game) == game_state::paused && toggle_pause,
           [=](dtime t0) {
             return Game(prev_snake_dir,
                         prev_snake_body,
                         game_state::running,
                         Timeout(timeout, t0));
           }),
        On(CurrentState(prev_game) == game_state::running && step,
           [=](dtime t0) {
             const auto snake_body =
               prev_snake_body.erase(prev_snake_body.length() - 1)
                 .prepend(next_head_position);

             return Game(snake_dir,
                         snake_body(t0),
                         game_state::running,
                         Timeout(timeout, t0));
           }));
    },
    t0);
}

int main(int argc, char* p_argv[])
{
  QApplication app(argc, p_argv);

  EngineQml engine(app);

  const auto field_size = Vec2(21, 30);

  const auto m = Main([=](dtime t0) {
    auto turn_east = Signal();
    auto turn_north = Signal();
    auto turn_south = Signal();
    auto turn_west = Signal();
    auto toggle_pause = Signal();
    auto restart_game = Signal();

    const auto game_state = GameState(turn_east,
                                      turn_north,
                                      turn_south,
                                      turn_west,
                                      toggle_pause,
                                      restart_game,
                                      field_size,
                                      t0);

    const auto context = qt::QmlContext(
      qt::QmlFunction("turnNorth", turn_north),
      qt::QmlFunction("turnEast", turn_east),
      qt::QmlFunction("turnSouth", turn_south),
      qt::QmlFunction("turnWest", turn_west),
      qt::QmlFunction("togglePause", toggle_pause),
      qt::QmlFunction("restartGame", restart_game),
      qt::QmlProperty("fieldSize", field_size),
      qt::QmlProperty("snakeBody", qt::QmlData(SnakeBody(game_state))),
      qt::QmlProperty("gameOver",
                      CurrentState(game_state) == game_state::ended));

    return qt::QmlComponent("qrc:/view.qml", context);
  });

  return app.exec();
}
