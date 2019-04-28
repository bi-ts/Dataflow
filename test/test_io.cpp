
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include "tools/io_fixture.h"

#include <dataflow/io.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

class test_io_fixture : public io_fixture
{
protected:
  test_io_fixture()
  : engine_()
  , x(Var<int>(1))
  , y(Var<int>(1))
  {
  }

  ~test_io_fixture()
  {
  }

private:
  Engine engine_;

protected:
  var<int> x;
  var<int> y;
};

BOOST_AUTO_TEST_SUITE(test_io)

BOOST_FIXTURE_TEST_CASE(test_Message_Error, test_io_fixture)
{
  capture_output();

  const auto z = Main(
    [=](const Time& t) { return console::Error(t, "(", x, ", ", y, ")"); });

  reset_output();

  BOOST_CHECK_EQUAL(err_string(), "(1, 1);");

  capture_output();

  x = 2;

  reset_output();

  BOOST_CHECK_EQUAL(err_string(), "(1, 1);(2, 1);");

  capture_output();

  y = 3;

  reset_output();

  BOOST_CHECK_EQUAL(err_string(), "(1, 1);(2, 1);(2, 3);");
}

BOOST_FIXTURE_TEST_CASE(test_Message_Log, test_io_fixture)
{
  capture_output();

  const auto z = Main(
    [=](const Time& t) { return console::Log(t, "Point(", x, ", ", y, ")"); });

  reset_output();

  BOOST_CHECK_EQUAL(log_string(), "Point(1, 1);");

  capture_output();

  x = 2;

  reset_output();

  BOOST_CHECK_EQUAL(log_string(), "Point(1, 1);Point(2, 1);");

  capture_output();

  y = 3;

  reset_output();

  BOOST_CHECK_EQUAL(log_string(), "Point(1, 1);Point(2, 1);Point(2, 3);");
}

BOOST_FIXTURE_TEST_CASE(test_Message_Output, test_io_fixture)
{
  capture_output();

  const auto z = Main([=](const Time& t) {
    return console::Output(t, x, " + ", y, " = ", x + y);
  });

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "1 + 1 = 2;");

  capture_output();

  x = 2;

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "1 + 1 = 2;2 + 1 = 3;");

  capture_output();

  y = 3;

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "1 + 1 = 2;2 + 1 = 3;2 + 3 = 5;");
}

BOOST_FIXTURE_TEST_CASE(test_Input, test_io_fixture)
{
  set_input("world\n");
  capture_output();

  const auto x = Main([](const Time& t) {
    const auto name = console::Input(t, Const("What's your name?: "));
    return console::Log(t, "Hello, ", name, "!");
  });

  reset();

  BOOST_CHECK_EQUAL(log_string(), "Hello, world!;");
}

BOOST_FIXTURE_TEST_CASE(test_Input_int, test_io_fixture)
{
  set_input("10\n20\n");
  capture_output();

  const auto y = Main([](const Time& t) {
    const auto x = console::Input<int>(t, Const("x = "));
    const auto y = console::Input<int>(t, Const("y = "));

    return console::Log(t, x, " - ", y, " = ", x - y);
  });

  reset();

  BOOST_CHECK_EQUAL(out_string(), "x = y = ");
  BOOST_CHECK_EQUAL(log_string(), "10 - 20 = -10;");
}

BOOST_FIXTURE_TEST_CASE(test_Message_Output_Time, test_io_fixture)
{
  capture_output();

  const auto z = Main([=](const Time& t) {
    return console::Output(t, "x(", CurrentTime(), ") = ", x);
  });

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "x(0) = 1;");

  capture_output();

  x = 2;

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "x(0) = 1;x(1) = 2;");

  capture_output();

  x = 5;

  reset_output();

  BOOST_CHECK_EQUAL(out_string(), "x(0) = 1;x(1) = 2;x(2) = 5;");
}

BOOST_AUTO_TEST_SUITE_END()
}
