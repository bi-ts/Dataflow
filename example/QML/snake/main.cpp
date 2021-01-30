
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

DATAFLOW_COMPOSITE(game,
                   Game,
                   (vec2<int>, SnakeDir),
                   (list<vec2<int>>, SnakeBody),
                   (bool, GameOver),
                   (bool, Tick));

ref<game> InitialGame(dtime t0)
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
              false,
              Timeout(500, t0));
}

ref<game> GameState(const sig& turn_east,
                    const sig& turn_north,
                    const sig& turn_south,
                    const sig& turn_west,
                    const ref<vec2<int>>& field_size,
                    dtime t0)
{
  return StateMachine(
    InitialGame(t0),
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
        On(!GameOver(prev_game) && step && next_head_position_is_bad,
           Game(snake_dir, prev_snake_body, true, false)),
        On(!GameOver(prev_game) && step, [=](dtime t0) {
          const auto snake_body =
            prev_snake_body.erase(prev_snake_body.length() - 1)
              .prepend(next_head_position);

          return Game(snake_dir, snake_body(t0), false, Timeout(50, t0));
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

    const auto game_state =
      GameState(turn_east, turn_north, turn_south, turn_west, field_size, t0);

    const auto context = qt::QmlContext(
      qt::QmlFunction("turnNorth", turn_north),
      qt::QmlFunction("turnEast", turn_east),
      qt::QmlFunction("turnSouth", turn_south),
      qt::QmlFunction("turnWest", turn_west),
      qt::QmlProperty("fieldSize", field_size),
      qt::QmlProperty("snakeBody", qt::QmlData(SnakeBody(game_state))),
      qt::QmlProperty("gameOver", GameOver(game_state)));

    return qt::QmlComponent("qrc:/view.qml", context);
  });

  return app.exec();
}
