
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

#include <dataflow/list.h>
#include <dataflow/macro.h>
#include <dataflow/prelude.h>
#include <dataflow/qt.h>

#include <QtWidgets/QApplication>

using namespace dataflow;

DATAFLOW_COMPOSITE(game, Game, (list<vec2<int>>, SnakeBody), (bool, GameOver));

ref<game> InitialGame()
{
  return Game(ListC(Vec2(10, 15), Vec2(10, 16), Vec2(10, 17)), false);
}

ref<game> GameState(const sig& turn_east,
                    const sig& turn_north,
                    const sig& turn_south,
                    const sig& turn_west,
                    const ref<vec2<int>>& field_size,
                    dtime t0)
{
  return Recursion(
    InitialGame(),
    [=](const ref<game>& prev_game) {
      const auto prev_snake_body = SnakeBody(prev_game);
      const auto prev_game_over = GameOver(prev_game);

      const auto step = Switch(turn_north >>= Vec2(0, -1),
                               turn_east >>= Vec2(1, 0),
                               turn_south >>= Vec2(0, 1),
                               turn_west >>= Vec2(-1, 0),
                               Default(Vec2(0, 0)));

      const auto head_position = FromMaybe(prev_snake_body[0]) + step;

      const auto game_over = prev_game_over || head_position.x() < 0 ||
                             head_position.y() < 0 ||
                             head_position.x() >= field_size.x() ||
                             head_position.y() >= field_size.y();

      const auto snake_body =
        If(!game_over && step != Vec2(0, 0),
           Insert(Erase(prev_snake_body, Length(prev_snake_body) - 1),
                  0,
                  head_position),
           prev_snake_body);

      return Game(snake_body, game_over);
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
