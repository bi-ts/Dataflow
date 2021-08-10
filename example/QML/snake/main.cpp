
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

#include <dataflow-qt/qt.h>
#include <dataflow/list.h>
#include <dataflow/macro.h>
#include <dataflow/random.h>

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
                   (dir2, SnakeDir),
                   (list<vec2<int>>, SnakeBody),
                   (vec2<int>, FoodPos),
                   (random::random_number, RandomSeed),
                   (game_state, CurrentState));

ref<game> InitialGame(const ref<vec2<int>>& field_size,
                      const ref<random::random_number>& seed)
{
  const auto rv1 = random::GenerateLCG(seed);
  const auto rv2 = random::GenerateLCG(rv1);

  return Game(dir2::north,
              ListC(Vec2(5, 15), Vec2(5, 16), Vec2(5, 17)),
              Vec2(random::FromRandomNumber<int>(0, field_size.x() - 1, rv1),
                   random::FromRandomNumber<int>(0, field_size.y() - 1, rv2)),
              rv2,
              game_state::running);
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
  return Recursion(
    InitialGame(field_size, random::DefaultSeed()),
    [=](const ref<game>& prev_game) {
      return [=](dtime t0) {
        const auto tick =
          Since(Changed(SnakeBody(prev_game)[0], t0) ||
                  Equalized(CurrentState(prev_game), game_state::running, t0),
                Timeout(100),
                t0);

        const auto prev_snake_dir = SnakeDir(prev_game);
        const auto prev_snake_body = SnakeBody(prev_game);
        const auto prev_food_pos = FoodPos(prev_game);
        const auto prev_random_seed = RandomSeed(prev_game);

        const auto requested_dir = Switch(turn_north >>= dir2::north,
                                          turn_east >>= dir2::east,
                                          turn_south >>= dir2::south,
                                          turn_west >>= dir2::west,
                                          Default(prev_snake_dir));

        const auto step = tick || (prev_snake_dir != -requested_dir &&
                                   prev_snake_dir != requested_dir);

        const auto snake_dir = If(step, requested_dir, prev_snake_dir);

        const auto next_head_position =
          FromMaybe(prev_snake_body[0]) + ToVec2<int>(snake_dir);

        const auto next_head_position_is_bad =
          next_head_position.x() < 0 || next_head_position.y() < 0 ||
          next_head_position.x() >= field_size.x() ||
          next_head_position.y() >= field_size.y();

        const auto food_eaten = next_head_position == prev_food_pos;

        const auto snake_body =
          If(food_eaten,
             prev_snake_body.prepend(next_head_position),
             prev_snake_body.erase(prev_snake_body.length() - 1)
               .prepend(next_head_position));

        const auto rv1 = random::GenerateLCG(prev_random_seed);
        const auto rv2 = random::GenerateLCG(rv1);

        const auto food_pos =
          Vec2(random::FromRandomNumber<int>(0, field_size.x() - 1, rv1),
               random::FromRandomNumber<int>(0, field_size.y() - 1, rv2));

        const auto next_food_pos = If(food_eaten, food_pos, prev_food_pos);
        const auto next_random_seed = If(food_eaten, rv2, prev_random_seed);

        return Switch(
          Case(restart_game, InitialGame(field_size, next_random_seed)),
          Case(CurrentState(prev_game) == game_state::running && toggle_pause,
               Game(prev_snake_dir,
                    prev_snake_body,
                    prev_food_pos,
                    prev_random_seed,
                    game_state::paused)),
          Case(CurrentState(prev_game) == game_state::paused && toggle_pause,
               Game(prev_snake_dir,
                    prev_snake_body,
                    next_food_pos,
                    next_random_seed,
                    game_state::running)),
          Case(CurrentState(prev_game) == game_state::running && step &&
                 next_head_position_is_bad,
               Game(snake_dir,
                    prev_snake_body,
                    next_food_pos,
                    next_random_seed,
                    game_state::ended)),
          Case(CurrentState(prev_game) == game_state::running && step,
               Game(snake_dir,
                    snake_body,
                    next_food_pos,
                    next_random_seed,
                    game_state::running)),
          Default(prev_game));
      };
    },
    t0);
}

int main(int argc, char* p_argv[])
{
  QApplication app(argc, p_argv);

  dataflow2qt::EngineQml engine(app);

  const auto field_size = Vec2(10, 20);

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

    const auto context = dataflow2qt::QmlContext(
      dataflow2qt::QmlFunction("turnNorth", turn_north),
      dataflow2qt::QmlFunction("turnEast", turn_east),
      dataflow2qt::QmlFunction("turnSouth", turn_south),
      dataflow2qt::QmlFunction("turnWest", turn_west),
      dataflow2qt::QmlFunction("togglePause", toggle_pause),
      dataflow2qt::QmlFunction("restartGame", restart_game),
      dataflow2qt::QmlProperty("fieldSize", field_size),
      dataflow2qt::QmlProperty(
        "snakeBody", SnakeBody(game_state).append(FoodPos(game_state))),
      dataflow2qt::QmlProperty("gameOver",
                               CurrentState(game_state) == game_state::ended));

    return dataflow2qt::QmlComponentInstance(
      "qrc:/view.qml", context, "view_context");
  });

  return app.exec();
}
