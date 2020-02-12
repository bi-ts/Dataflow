
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

#include <dataflow/qt.h>

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_qt)

BOOST_AUTO_TEST_CASE(test_EngineQml_instance_throws_if_no_engine)
{
  BOOST_CHECK_THROW(dataflow::EngineQml::instance(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_EngineQml_instance_throws_if_wrong_engine_type)
{
  dataflow::Engine engine;

  BOOST_CHECK_THROW(dataflow::EngineQml::instance(), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
