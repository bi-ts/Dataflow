
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

#include <boost/test/unit_test.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace dataflow_naive
{
using dtime = int;
using dtimestamp = int;
using integer = int;

static dtime g_time = 0;
static bool g_initialized = false;

class Engine
{
public:
  Engine()
  {
    g_time = 0;
    g_initialized = true;
  }

  ~Engine()
  {
    g_time = 0;
    g_initialized = false;
  }
};

template <typename T> using ref = std::function<T(dtime)>;

template <typename T> using init_function = std::function<ref<T>(dtime)>;

template <typename T> ref<T> Const(const T& v)
{
  return [v](dtime t) { return v; };
}

template <typename T> class var
{
private:
  using values_type = std::vector<std::pair<dtime, T>>;

public:
  explicit var(const T& v)
  : p_values_{std::make_shared<values_type>()}
  {
    p_values_->push_back(std::make_pair(g_time, v));
  }

  const T& operator()(dtime t)
  {
    auto it =
      std::find_if(p_values_->begin(),
                   p_values_->end(),
                   [t](const std::pair<dtime, T>& p) { return p.first > t; });

    return (--it)->second;
  }

  var& operator=(const T& v)
  {
    p_values_->emplace_back(++g_time, v);

    return *this;
  }

private:
  std::shared_ptr<values_type> p_values_;
};

template <typename T> var<T> Var(const T& v)
{
  return var<T>{v};
}

template <typename T> ref<T> Main(const init_function<T>& f)
{
  return f(g_time);
}

template <typename T> ref<T> Sum(const ref<T>& x, const ref<T>& y)
{
  return [x, y](dtime t) { return x(t) + y(t); };
}

}

using namespace dataflow_naive;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_core_naive)

BOOST_AUTO_TEST_CASE(test_Var)
{
  Engine engine;

  var<int> x = Var(0);
  var<int> y = Var(0);

  const ref<int> f = Main<int>([=](dtime t0) { return Sum<int>(x, y); });

  BOOST_CHECK_EQUAL(g_time, 0);
  BOOST_CHECK_EQUAL(f(g_time), 0);

  x = 5;

  BOOST_CHECK_EQUAL(g_time, 1);
  BOOST_CHECK_EQUAL(f(g_time), 5);

  y = 100;

  BOOST_CHECK_EQUAL(g_time, 2);
  BOOST_CHECK_EQUAL(f(g_time), 105);

  x = 3;

  BOOST_CHECK_EQUAL(g_time, 3);
  BOOST_CHECK_EQUAL(f(g_time), 103);

  BOOST_CHECK_EQUAL(x(0), 0);
  BOOST_CHECK_EQUAL(x(1), 5);
  BOOST_CHECK_EQUAL(x(2), 5);
  BOOST_CHECK_EQUAL(x(3), 3);

  BOOST_CHECK_EQUAL(y(0), 0);
  BOOST_CHECK_EQUAL(y(1), 0);
  BOOST_CHECK_EQUAL(y(2), 100);
  BOOST_CHECK_EQUAL(y(3), 100);
}

BOOST_AUTO_TEST_SUITE_END()
}
