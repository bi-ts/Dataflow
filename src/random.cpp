
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

#include <random>

namespace dataflow
{

ref<random::random_number> random::DefaultSeed()
{
  return Const<random_number>(182605794);
}

ref<random::random_number> random::GenerateLCG(const arg<random_number>& seed)
{
  class policy
  {
  public:
    std::string label() const
    {
      return "random-generate-lcg";
    }

    random_number calculate(const random_number& v) const
    {
      return std::minstd_rand{v == 0 ? 1 : v}();
    };
  };

  return core::Lift<policy>(seed);
}

ref<random::random_number> random::GenerateLCG()
{
  return GenerateLCG(DefaultSeed());
}
} // namespace dataflow
