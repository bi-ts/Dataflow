
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

#include <dataflow/random.h>

#include <boost/test/unit_test.hpp>

#include <array>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_random)

// Random values that follow 0.
const std::array<random::random_number, 5> random_table{
  48271, 182605794, 1291394886, 1914720637, 2078669041};

BOOST_AUTO_TEST_CASE(test_random_GenerateLCG_seed)
{
  Engine engine;

  const auto seed = Var<random::random_number>(0);

  const auto a = random::GenerateLCG(seed);

  const auto m = Main(a);

  BOOST_CHECK_EQUAL(*m, random_table[0]);

  seed = *m;

  BOOST_CHECK_EQUAL(*m, random_table[1]);

  seed = *m;

  BOOST_CHECK_EQUAL(*m, random_table[2]);

  seed = *m;

  BOOST_CHECK_EQUAL(*m, random_table[3]);

  seed = *m;

  BOOST_CHECK_EQUAL(*m, random_table[4]);
}

BOOST_AUTO_TEST_CASE(test_random_GenerateLCG_seed_zero)
{
  Engine engine;

  const auto a = random::GenerateLCG(0);

  const auto m = Main(a);

  BOOST_CHECK_EQUAL(*m, 48271);
}

BOOST_AUTO_TEST_CASE(test_random_GenerateLCG_seed_default)
{
  Engine engine;

  const auto a = random::GenerateLCG();

  const auto m = Main(a);

  BOOST_CHECK_EQUAL(*m, 1291394886);
}

BOOST_AUTO_TEST_SUITE_END()
}
