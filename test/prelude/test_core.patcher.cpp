
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
  explicit patch(const data& curr, const data& prev)
  : restored(curr != prev)
  , diff(restored ? curr.value - prev.value : 0)
  {
  }

  explicit patch(int diff)
  : restored(false)
  , diff(diff)
  {
  }

  data apply(data v) const
  {
    return data{v.value + diff};
  }

  const bool restored;
  const int diff;
};

class patcher_test_counters
{
public:
  explicit patcher_test_counters()
  : calculate_counter_()
  , prepare_patch_counter_()
  , consumed_restored_patches_counter_()
  {
  }

  patcher_test_counters(int calculate_counter,
                        int prepare_patch_counter,
                        int consumed_restored_patches_counter = 0)
  : calculate_counter_(calculate_counter)
  , prepare_patch_counter_(prepare_patch_counter)
  , consumed_restored_patches_counter_(consumed_restored_patches_counter)
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

  void inc_consumed_restored_patches_counter()
  {
    ++consumed_restored_patches_counter_;
  }

  bool operator==(const patcher_test_counters& other) const
  {
    return calculate_counter_ == other.calculate_counter_ &&
           prepare_patch_counter_ == other.prepare_patch_counter_ &&
           consumed_restored_patches_counter_ ==
             other.consumed_restored_patches_counter_;
  }

  friend std::ostream& operator<<(std::ostream& out,
                                  const patcher_test_counters& value)
  {
    out << "{" << value.calculate_counter_ << ", "
        << value.prepare_patch_counter_;

    if (value.consumed_restored_patches_counter_ != 0)
      out << ", restored: " << value.consumed_restored_patches_counter_;

    out << "}";

    return out;
  }

private:
  int calculate_counter_;
  int prepare_patch_counter_;
  int consumed_restored_patches_counter_;
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

      if (v.patch().restored)
        counters.inc_consumed_restored_patches_counter();

      return patch{v.patch().diff + (u.curr() - u.prev())};
    }

    patcher_test_counters& counters;
  };

  return core::LiftPatcher(transform_data_policy{counters}, x, y);
}

ref<data> TransformData2(patcher_test_counters& counters,
                         ref<data> x,
                         ref<data> y,
                         ref<int> z)
{
  struct transform_data_policy
  {
    static std::string label()
    {
      return "transform-data-2";
    }

    data calculate(data v, data w, int u)
    {
      counters.inc_calculate_counter();
      return data{v.value + w.value + u};
    }

    patch prepare_patch(const core::diff_type_t<data>& v,
                        const core::diff_type_t<data>& w,
                        const core::diff_type_t<int>& u)
    {
      counters.inc_prepare_patch_counter();

      if (v.patch().restored)
        counters.inc_consumed_restored_patches_counter();

      if (w.patch().restored)
        counters.inc_consumed_restored_patches_counter();

      return patch{v.patch().diff + w.patch().diff + (u.curr() - u.prev())};
    }

    patcher_test_counters& counters;
  };

  return core::LiftPatcher(transform_data_policy{counters}, x, y, z);
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

  auto f = Main(b);

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(*f, data{24});

  x = 32;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(*f, data{34});

  x = 12;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(*f, data{14});

  y = 3;

  BOOST_CHECK_EQUAL(make_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 3));
  BOOST_CHECK_EQUAL(*f, data{15});
}

BOOST_AUTO_TEST_CASE(test_LiftPatcher_restored_patches_from_var)
{
  Engine engine;

  patcher_test_counters transform_data_counters;

  auto x = Var<data>(data{22});
  auto y = Var<int>(33);

  auto z = TransformData(transform_data_counters, x, y);

  //         f
  //         |
  //         z
  //         |
  // TransformData (+)
  // |              |
  // x              y

  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(0, 0));

  auto f = Main(z);

  //         f=55
  //          |
  //          z
  //          |
  //  TransformData (+)
  //  |              |
  // x=22           y=33

  BOOST_CHECK_EQUAL(*f, data{55});
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 0));

  x = data{44};

  //         f=77
  //          |
  //          z
  //          |
  //  TransformData (+)
  //  |              |
  // x=44           y=33

  BOOST_CHECK_EQUAL(*f, data{77});
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 1, 1));

  y = 55;

  //         f=99
  //          |
  //          z
  //          |
  //  TransformData (+)
  //  |              |
  // x=44           y=55

  BOOST_CHECK_EQUAL(*f, data{99});
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 2, 1));

  x = data{11};

  //         f=66
  //          |
  //          z
  //          |
  //  TransformData (+)
  //  |              |
  // x=11           y=55

  BOOST_CHECK_EQUAL(*f, data{66});
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 3, 2));
}

BOOST_AUTO_TEST_CASE(test_LiftPatcher_pass_custom_patch_through_if_block)
{
  Engine engine;

  patcher_test_counters make_data_counters_a;
  patcher_test_counters make_data_counters_b;
  patcher_test_counters transform_data_counters;

  auto x = Var<int>(22);
  auto y = Var<int>(2);
  auto z = Var<int>(2);

  auto a = MakeData(make_data_counters_a, x);
  auto b = MakeData(make_data_counters_b, y);

  struct policy
  {
    static std::string label()
    {
      return "< 4";
    }
    bool calculate(int v)
    {
      return v < 4;
    }
  };

  auto c = TransformData2(
    transform_data_counters, If(core::Lift<policy>(y), a, b), b, z);

  //                   f
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //    .-no-<if>-yes-.  |     |
  //    |      |      |  |     |
  //    .------+------+--*     |
  //    |      |      |        |
  //    b      |      a        |
  //    |      |      |        |
  // MakeData  |   MakeData    |
  //    |      |      |        |
  //    |      |      |        |
  //    |   (y < 4)   |        |
  //    |      |      |        |
  //    *--.---*      |        |
  //       |          |        |
  //       y          x        z

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(0, 0));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(0, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(0, 0));

  auto f = Main(c);

  //                  f=26
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //         <if>-yes-.  |     |
  //           |      |  |     |
  //    .------+------+--*     |
  //    |      |      |        |
  //    b      |      a        |
  //    |      |      |        |
  // MakeData  |   MakeData    |
  //    |      |      |        |
  //    |      |      |        |
  //    |   (y < 4)   |        |
  //    |      |      |        |
  //    *--.---*      |        |
  //       |          |        |
  //      y=2        x=22     z=2

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(*f, data{26});

  x = 32;

  //                  f=36
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //         <if>-yes-.  |     |
  //           |      |  |     |
  //    .------+------+--*     |
  //    |      |      |        |
  //    b      |      a        |
  //    |      |      |        |
  // MakeData  |   MakeData    |
  //    |      |      |        |
  //    |      |      |        |
  //    |   (y < 4)   |        |
  //    |      |      |        |
  //    *--.---*      |        |
  //       |          |        |
  //      y=2        x=32     z=2

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(*f, data{36});

  z = 3;

  //                  f=37
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //         <if>-yes-.  |     |
  //           |      |  |     |
  //    .------+------+--*     |
  //    |      |      |        |
  //    b      |      a        |
  //    |      |      |        |
  // MakeData  |   MakeData    |
  //    |      |      |        |
  //    |      |      |        |
  //    |   (y < 4)   |        |
  //    |      |      |        |
  //    *--.---*      |        |
  //       |          |        |
  //      y=2        x=32     z=3

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 0));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(*f, data{37});

  y = 4;

  //                  f=11
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //    .-no-<if>        |     |
  //    |      |         |     |
  //    .------+---------*     |
  //    |      |               |
  //    b      |               |
  //    |      |               |
  // MakeData  |               |
  //    |      |               |
  //    |      |               |
  //    |   (y < 4)            |
  //    |      |               |
  //    *--.---*               |
  //       |                   |
  //      y=4                 z=3

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 3, 1));
  BOOST_CHECK_EQUAL(*f, data{11});

  y = 8;

  //                  f=19
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //    .-no-<if>        |     |
  //    |      |         |     |
  //    .------+---------*     |
  //    |      |               |
  //    b      |               |
  //    |      |               |
  // MakeData  |               |
  //    |      |               |
  //    |      |               |
  //    |   (y < 4)            |
  //    |      |               |
  //    *--.---*               |
  //       |                   |
  //      y=8                 z=3

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(1, 1));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 2));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 4, 1));
  BOOST_CHECK_EQUAL(*f, data{19});

  y = 3;

  //                  f=38
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //         <if>-yes-.  |     |
  //           |      |  |     |
  //    .------+------+--*     |
  //    |      |      |        |
  //    b      |      a        |
  //    |      |      |        |
  // MakeData  |   MakeData    |
  //    |      |      |        |
  //    |      |      |        |
  //    |   (y < 4)   |        |
  //    |      |      |        |
  //    *--.---*      |        |
  //       |          |        |
  //      y=3        x=32     z=3

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(2, 1));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 3));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 5, 2));
  BOOST_CHECK_EQUAL(*f, data{38});

  x = 0;

  //                  f=6
  //                   |
  //           TransformData2 (+)
  //           |         |     |
  //         <if>-yes-.  |     |
  //           |      |  |     |
  //    .------+------+--'     |
  //    |      |      |        |
  //    b      |      a        |
  //    |      |      |        |
  // MakeData  |   MakeData    |
  //    |      |      |        |
  //    |      |      |        |
  //    |   (y < 4)   |        |
  //    |      |      |        |
  //    `--.---'      |        |
  //       |          |        |
  //      y=3        x=0      z=3

  BOOST_CHECK_EQUAL(make_data_counters_a, patcher_test_counters(2, 2));
  BOOST_CHECK_EQUAL(make_data_counters_b, patcher_test_counters(1, 3));
  BOOST_CHECK_EQUAL(transform_data_counters, patcher_test_counters(1, 6, 2));
  BOOST_CHECK_EQUAL(*f, data{6});
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
  auto z = Main(y);

  BOOST_CHECK_EQUAL(*z, 110);

  x = 64;

  BOOST_CHECK_EQUAL(*z, 320);

  x = 0;

  BOOST_CHECK_EQUAL(*z, 0);
}

BOOST_AUTO_TEST_SUITE_END()
}
