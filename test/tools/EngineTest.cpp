
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

#include "EngineTest.h"

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{
namespace
{
dataflow::engine_options get_options()
{
  const auto& test_suit = boost::unit_test::framework::master_test_suite();

  if (test_suit.argc == 2)
  {
    if (std::string(test_suit.argv[1]) == "--no-optimization")
    {
      return dataflow::engine_options::nothing;
    }
  }

  return dataflow::engine_options::fully_optimized;
}
}

EngineTest::EngineTest()
: Engine{get_options()}
{
}

EngineTest::~EngineTest()
{
}
}
