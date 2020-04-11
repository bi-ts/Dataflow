
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

#include <dataflow/behavior.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_behavior)

BOOST_AUTO_TEST_CASE(test_button_state)
{
  Engine engine;

  auto x = Var(button_state::up);

  const auto f = Main(x);

  BOOST_CHECK_EQUAL(*f, button_state::up);
  BOOST_CHECK_EQUAL(introspect::value(f), "button_state::up");

  x = button_state::down;

  BOOST_CHECK_EQUAL(*f, button_state::down);
  BOOST_CHECK_EQUAL(introspect::value(f), "button_state::down");

  x = static_cast<button_state>(11042020);

  BOOST_CHECK_EQUAL(introspect::value(f), "button_state::unknown (11042020)");
}

BOOST_AUTO_TEST_CASE(test_button_state_change)
{
  Engine engine;

  var<button_state> x = Var(button_state::up);
  var<button_state> y = Var(button_state::up);

  const val<button_state_change> f = Main(y - x);

  BOOST_CHECK_EQUAL(*f, button_state_change::unchanged);
  BOOST_CHECK_EQUAL(introspect::value(f), "button_state_change::unchanged");

  x = button_state::down;

  BOOST_CHECK_EQUAL(*f, button_state_change::released);
  BOOST_CHECK_EQUAL(introspect::value(f), "button_state_change::released");

  y = button_state::down;

  BOOST_CHECK_EQUAL(*f, button_state_change::unchanged);
  BOOST_CHECK_EQUAL(introspect::value(f), "button_state_change::unchanged");

  x = button_state::up;

  BOOST_CHECK_EQUAL(*f, button_state_change::pressed);
  BOOST_CHECK_EQUAL(introspect::value(f), "button_state_change::pressed");

  y = static_cast<button_state>(11042020);

  BOOST_CHECK_EQUAL(introspect::value(f),
                    "button_state_change::unknown (11042020)");
}

BOOST_AUTO_TEST_SUITE_END()
}
