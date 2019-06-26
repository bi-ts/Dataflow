
//  Copyright (c) 2014 - 2019 Maksym V. Bilinets.
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

#include <dataflow/prelude/core.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
struct patch;

struct data
{
  using patch_type = patch;

  bool operator==(const data& other) const
  {
    return value == other.value;
  }

  bool operator!=(const data& other) const
  {
    return !(*this == other);
  }

  friend std::ostream& operator<<(std::ostream& out, const data& x)
  {
    out << "data (" << x.value << ")";
    return out;
  }

  int value;
};

struct patch
{
  using data_type = data;

  explicit patch()
  {
  }

  explicit patch(data)
  : diff()
  {
  }

  explicit patch(int diff)
  : diff(diff)
  {
  }

  data apply(data v) const
  {
    return data{v.value + diff};
  }

  int diff;
};

BOOST_AUTO_TEST_SUITE(test_core_patcher)

BOOST_AUTO_TEST_CASE(test_LiftPatcher_custom_patch)
{
  Engine engine;

  struct make_data_policy
  {
    static std::string label()
    {
      return "make-data";
    }

    data calculate(int v)
    {
      ++calculate_counter;
      return data{v};
    }

    patch prepare_patch(const core::diff_type_t<int>& v)
    {
      ++prepare_patch_counter;
      return patch{v.curr() - v.prev()};
    }

    int& calculate_counter;
    int& prepare_patch_counter;
  };

  struct transform_data_policy
  {
    static std::string label()
    {
      return "transform-data";
    }

    data calculate(data v, int u)
    {
      ++calculate_counter;
      return data{v.value + u};
    }

    patch prepare_patch(const core::diff_type_t<data>& v,
                        const core::diff_type_t<int>& u)
    {
      ++prepare_patch_counter;
      return patch{v.patch().diff + (u.curr() - u.prev())};
    }

    int& calculate_counter;
    int& prepare_patch_counter;
  };

  int make_data_calculate_counter = 0;
  int make_data_prepare_patch_counter = 0;

  int transform_data_calculate_counter = 0;
  int transform_data_prepare_patch_counter = 0;

  auto x = Var<int>(22);
  auto y = Var<int>(2);

  auto a = core::LiftPatcher(make_data_policy{make_data_calculate_counter,
                                              make_data_prepare_patch_counter},
                             x);

  auto b = core::LiftPatcher(
    transform_data_policy{transform_data_calculate_counter,
                          transform_data_prepare_patch_counter},
    a,
    y);

  BOOST_CHECK_EQUAL(make_data_calculate_counter, 0);
  BOOST_CHECK_EQUAL(make_data_prepare_patch_counter, 0);
  BOOST_CHECK_EQUAL(transform_data_calculate_counter, 0);
  BOOST_CHECK_EQUAL(transform_data_prepare_patch_counter, 0);

  auto f = *b;

  BOOST_CHECK_EQUAL(make_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(make_data_prepare_patch_counter, 0);
  BOOST_CHECK_EQUAL(transform_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(transform_data_prepare_patch_counter, 0);
  BOOST_CHECK_EQUAL(f(), data{24});

  x = 32;

  BOOST_CHECK_EQUAL(make_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(make_data_prepare_patch_counter, 1);
  BOOST_CHECK_EQUAL(transform_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(transform_data_prepare_patch_counter, 1);
  BOOST_CHECK_EQUAL(f(), data{34});

  x = 12;

  BOOST_CHECK_EQUAL(make_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(make_data_prepare_patch_counter, 2);
  BOOST_CHECK_EQUAL(transform_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(transform_data_prepare_patch_counter, 2);
  BOOST_CHECK_EQUAL(f(), data{14});

  y = 3;

  BOOST_CHECK_EQUAL(make_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(make_data_prepare_patch_counter, 2);
  BOOST_CHECK_EQUAL(transform_data_calculate_counter, 1);
  BOOST_CHECK_EQUAL(transform_data_prepare_patch_counter, 3);
  BOOST_CHECK_EQUAL(f(), data{15});
}

BOOST_AUTO_TEST_CASE(test_LiftPatcher_generic_patch)
{
  Engine engine;

  struct policy
  {
    static std::string label()
    {
      return "test";
    }

    static int calculate(int v)
    {
      return 0;
    }

    static core::generic_patch<int>
    prepare_patch(const core::diff_type_t<int>& v)
    {
      return core::generic_patch<int>(v.curr() - v.prev());
    }
  };

  auto x = Var<int>(22);
  auto y = core::LiftPatcher<policy>(x);
  auto z = *y;

  BOOST_CHECK_EQUAL(z(), 0);

  x = 64;

  BOOST_CHECK_EQUAL(z(), 42);

  x = 0;

  BOOST_CHECK_EQUAL(z(), -64);
}

BOOST_AUTO_TEST_SUITE_END()
}
