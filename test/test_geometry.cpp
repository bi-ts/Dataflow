
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

#include <dataflow/geometry.h>
#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_geometry)

BOOST_AUTO_TEST_CASE(test_vec2_default_ctor_getters)
{
  vec2<int> a;

  BOOST_CHECK_EQUAL(a.x(), 0);
  BOOST_CHECK_EQUAL(a.y(), 0);

  vec2<float> b;

  BOOST_CHECK_EQUAL(b.x(), 0.0f);
  BOOST_CHECK_EQUAL(b.y(), 0.0f);
}

BOOST_AUTO_TEST_CASE(test_vec2_ctor_getters)
{
  vec2<int> a = {1, 2};

  BOOST_CHECK_EQUAL(a.x(), 1);
  BOOST_CHECK_EQUAL(a.y(), 2);

  vec2<float> b = {1.1f, 2.2f};

  BOOST_CHECK_EQUAL(b.x(), 1.1f);
  BOOST_CHECK_EQUAL(b.y(), 2.2f);
}

BOOST_AUTO_TEST_CASE(test_vec2_int_to_string)
{
  vec2<int> a(1, 2);

  BOOST_CHECK_EQUAL(core::to_string(a), "vec2( 1 2 )");
}

BOOST_AUTO_TEST_CASE(test_vec2_int_subtract)
{
  vec2<int> a(1, 2);
  vec2<int> b(11, 22);

  BOOST_CHECK_EQUAL(a - b, (vec2<int>(-10, -20)));

  BOOST_CHECK_EQUAL(b - a, (vec2<int>(10, 20)));

  BOOST_CHECK_EQUAL(a - a, (vec2<int>()));
}

BOOST_AUTO_TEST_CASE(test_vec2_int_add)
{
  vec2<int> a(1, 2);
  vec2<int> b(11, 22);

  BOOST_CHECK_EQUAL(a + b, (vec2<int>(12, 24)));

  BOOST_CHECK_EQUAL(b + a, (vec2<int>(12, 24)));

  BOOST_CHECK_EQUAL(a + a, (vec2<int>(2, 4)));
}

BOOST_AUTO_TEST_CASE(test_vec2_int_squared_norm)
{
  vec2<int> a(2, 3);

  BOOST_CHECK_EQUAL(a.squared_norm(), 13);
}

BOOST_AUTO_TEST_CASE(test_Vec2)
{
  Engine engine;

  auto x = Var(1);
  auto y = Var(2);
  const auto a = Vec2(x, y);
  const auto m = Main(a);

  BOOST_CHECK_EQUAL(introspect::label(a), "vec2");

  BOOST_CHECK_EQUAL(*m, (vec2<int>(1, 2)));

  x = 4;

  BOOST_CHECK_EQUAL(*m, (vec2<int>(4, 2)));

  y = 0;

  BOOST_CHECK_EQUAL(*m, (vec2<int>(4, 0)));
}

BOOST_AUTO_TEST_CASE(test_Vec2_Add)
{
  Engine engine;

  const auto a = Vec2(1, 2);
  const auto b = Vec2(3, 4);
  const auto m = Main(Add(a, b));

  BOOST_CHECK_EQUAL(*m, (vec2<int>(4, 6)));
}

BOOST_AUTO_TEST_CASE(test_Vec2_Add_operator)
{
  Engine engine;

  const auto a = Vec2(1, 2);
  const auto b = Vec2(3, 4);
  const auto m = Main(a + b);

  BOOST_CHECK_EQUAL(*m, (vec2<int>(4, 6)));
}

BOOST_AUTO_TEST_CASE(test_Vec2_Sub)
{
  Engine engine;

  const auto a = Vec2(1, 8);
  const auto b = Vec2(3, 4);
  const auto m = Main(Sub(a, b));

  BOOST_CHECK_EQUAL(*m, (vec2<int>(-2, 4)));
}

BOOST_AUTO_TEST_CASE(test_Vec2_Sub_operator)
{
  Engine engine;

  const auto a = Vec2(1, 8);
  const auto b = Vec2(3, 4);
  const auto m = Main(a - b);

  BOOST_CHECK_EQUAL(*m, (vec2<int>(-2, 4)));
}

BOOST_AUTO_TEST_CASE(test_Vec2_SquaredNorm)
{
  Engine engine;

  auto x = Var(1);
  auto y = Var(2);
  const auto a = SquaredNorm(Vec2(x, y));
  const auto m = Main(a);

  BOOST_CHECK_EQUAL(introspect::label(a), "squared-norm");

  BOOST_CHECK_EQUAL(*m, 5);

  x = 4;

  BOOST_CHECK_EQUAL(*m, 20);

  y = 0;

  BOOST_CHECK_EQUAL(*m, 16);
}

BOOST_AUTO_TEST_CASE(test_vec2_PointsClose)
{
  Engine engine;

  auto x = Var<vec2<int>>(2, 1);
  auto y = Var<vec2<int>>(4, 3);
  auto r = Var(3);

  const auto m = Main(PointsClose(x, y, r));

  BOOST_CHECK_EQUAL(*m, true);

  x = vec2<int>(1, 2);

  BOOST_CHECK_EQUAL(*m, false);

  y = vec2<int>(2, 4);

  BOOST_CHECK_EQUAL(*m, true);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
