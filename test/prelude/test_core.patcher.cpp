
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

  explicit patch(const data& curr, const data& prev)
  : diff(curr.value - prev.value)
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

class patcher_test_counters
{
public:
  explicit patcher_test_counters()
  : calculate_counter_()
  , prepare_patch_counter_()
  {
  }

  patcher_test_counters(int calculate_counter, int prepare_patch_counter)
  : calculate_counter_(calculate_counter)
  , prepare_patch_counter_(prepare_patch_counter)
  {
  }

  void inc_calculate_counter()
  {
    ++calculate_counter_;
  }

  void inc_prepare_patch_counter()
  {
    ++prepare_patch_counter_;
  }

  bool operator==(const patcher_test_counters& other) const
  {
    return calculate_counter_ == other.calculate_counter_ &&
           prepare_patch_counter_ == other.prepare_patch_counter_;
  }

  friend std::ostream& operator<<(std::ostream& out,
                                  const patcher_test_counters& value)
  {
    return out << "{" << value.calculate_counter_ << ", "
               << value.prepare_patch_counter_ << "}";
  }

private:
  int calculate_counter_;
  int prepare_patch_counter_;
};

ref<data> MakeData(patcher_test_counters& counters, ref<int> x)
{
  struct make_data_policy
  {
    static std::string label()
    {
      return "make-data";
    }

    data calculate(int v)
    {
      counters.inc_calculate_counter();
      return data{v};
    }

    patch prepare_patch(const core::diff_type_t<int>& v)
    {
      counters.inc_prepare_patch_counter();
      return patch{v.curr() - v.prev()};
    }

    patcher_test_counters& counters;
  };

  return core::LiftPatcher(make_data_policy{counters}, x);
}

ref<data>
TransformData(patcher_test_counters& counters, ref<data> x, ref<int> y)
{
  struct transform_data_policy
  {
    static std::string label()
    {
      return "transform-data";
    }

    data calculate(data v, int u)
    {
      counters.inc_calculate_counter();
      return data{v.value + u};
    }

    patch prepare_patch(const core::diff_type_t<data>& v,
                        const core::diff_type_t<int>& u)
    {
      counters.inc_prepare_patch_counter();
      return patch{v.patch().diff + (u.curr() - u.prev())};
    }

    patcher_test_counters& counters;
  };

  return core::LiftPatcher(transform_data_policy{counters}, x, y);
}

BOOST_AUTO_TEST_SUITE(test_core_patcher)

BOOST_AUTO_TEST_CASE(test_LiftPatcher_custom_patch)
{
  Engine engine;

  patcher_test_counters make_data_counters;
  patcher_test_counters transform_data_counters;

  auto x = Var<int>(22);
  auto y = Var<int>(2);

  auto a = MakeData(make_data_counters, x);
  auto b = TransformData(transform_data_counters, a, y);

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(0, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(0, 0));

  auto f = *b;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(f(), data{24});

  x = 32;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(f(), data{34});

  x = 12;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(f(), data{14});

  y = 3;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 3));
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
      return 5 * v;
    }

    static core::generic_patch<int>
    prepare_patch(const core::diff_type_t<int>& v)
    {
      return core::generic_patch<int>(5 * v.curr(), 5 * v.prev());
    }
  };

  auto x = Var<int>(22);
  auto y = core::LiftPatcher<policy>(x);
  auto z = *y;

  BOOST_CHECK_EQUAL(z(), 110);

  x = 64;

  BOOST_CHECK_EQUAL(z(), 320);

  x = 0;

  BOOST_CHECK_EQUAL(z(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
}
