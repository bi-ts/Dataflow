
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

#include "check_points.h"

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace dataflow;
using namespace benchmark;

std::string Title(const std::string& title, char filler)
{
  const auto screen_width = 80;
  const auto title_length = title.length() + 2;
  const auto filler_length_1 = (screen_width - title_length) / 2;
  const auto filler_length_2 = screen_width - filler_length_1 - title_length;

  return std::string(filler_length_1, filler) + " " + title + " " +
         std::string(filler_length_2, filler) + "\n";
}

std::string Title1(const std::string& title)
{
  return Title(title, '=');
}

std::string Title2(const std::string& title)
{
  return Title(title, '-');
}

ref<int> ConstructLinearSequenceIncr(std::size_t exponent, ref<int> x)
{
  if (exponent == 0)
    return x;

  const auto y = ConstructLinearSequenceIncr(exponent - 1, x);

  return ConstructLinearSequenceIncr(exponent - 1, y++);
}

ref<int> ConstructLinearSequenceDecr(std::size_t exponent, ref<int> x)
{
  if (exponent == 0)
    return x;

  const auto y = ConstructLinearSequenceDecr(exponent - 1, x);

  return ConstructLinearSequenceDecr(exponent - 1, y--);
}

std::pair<ref<int>, std::size_t>
ConstructLinearSequenceAndPrintDescription(std::size_t exponent, ref<int> x)
{
  const auto prev = std::cout.fill('-');

  std::cout << std::endl;
  std::cout << "f(x) = incr( incr( incr( ... incr( x ) ... ) ) )" << std::endl;
  std::cout << "      |                           |" << std::endl;
  std::cout << "      `-----" << std::right << std::setw(10)
            << (1 << exponent) - 1 << " times------'" << std::endl;
  std::cout << std::endl;

  std::cout.fill(prev);

  return {ConstructLinearSequenceIncr(exponent, x), (1 << exponent) - 1};
}

std::pair<ref<int>, std::size_t>
ConstructConditionalLinearSequenceAndPrintDescription(std::size_t exponent,
                                                      ref<int> x)
{
  const auto prev = std::cout.fill('-');

  std::cout << std::endl;
  std::cout << "       ,-" << std::endl;
  std::cout << "       |  incr( incr( incr( ... incr( x ) ... ) ) ), if x == 1;"
            << std::endl;
  std::cout << "f(x) = {" << std::endl;
  std::cout << "       |  decr( decr( decr( ... decr( x ) ... ) ) ), otherwise."
            << std::endl;
  std::cout << "       `-" << std::endl;
  std::cout << "         |                           |" << std::endl;
  std::cout << "         `-----" << std::right << std::setw(10)
            << (1 << exponent) - 1 << " times------'" << std::endl;
  std::cout << std::endl;

  std::cout.fill(prev);

  const auto incr = ConstructLinearSequenceIncr(exponent, x);
  const auto decr = ConstructLinearSequenceDecr(exponent, x);

  const auto nodes_count = 2                     // conditional node
                           + (1 << exponent) - 1 // nodes per branch
                           + 1                   // equality comparison
                           + 0;                  // `Const(1)` is never active

  return {If(x == 1, incr, decr), nodes_count};
}

ref<int> ConstructBinary(std::size_t exponent, ref<int> x)
{
  std::vector<ref<int>> level = {x};

  for (std::size_t i = 0; i < exponent - 1; ++i)
  {
    std::vector<ref<int>> tmp;

    for (const auto& a : level)
    {
      tmp.push_back(a++);
      tmp.push_back(a++);
    }

    level.swap(tmp);
  }

  for (std::size_t i = exponent - 1; i > 0; --i)
  {
    std::vector<ref<int>> tmp;

    for (std::size_t j = 0; j < level.size(); j += 2)
    {
      tmp.push_back(level.at(j) + level.at(j + 1));
    }

    level.swap(tmp);
  }

  if (level.size() != 1)
    throw std::logic_error("benchmark logic error");

  return level.front();
}

std::pair<ref<int>, std::size_t>
ConstructBinaryAndPrintDescription(std::size_t exponent, ref<int> x)
{
  const std::pair<int, std::string> desc[] = {
    {0, "                            f(x)                                "},
    {1, "                             |                                  "},
    {1, "             ,--------------add----------------.          -.    "},
    {1, "             |                                 |           |    "},
    {2, "       ,----add---.                      ,----add---.     -|    "},
    {2, "       |          |                      |          |      |    "},
    {3, "   ,--add-.   ,--add-.               ,--add-.   ,--add-.  -|    "},
    {3, "   |      |   |      |               |      |   |      |   |    "},
    {4, "                                                           |    "},
    {4, "   |      |   |      |               |      |   |      |   |    "},
    {3,
     [&]() {
       std::stringstream ss;
       ss << " incr   incr incr  incr .." << std::left << std::setw(7)
          << std::setfill('.') << (1 << (exponent - 1))
          << ". incr   incr incr  incr -| " << std::left << std::setfill(' ')
          << 2 * (exponent - 1);
       return ss.str();
     }()},
    {3, "   |      |   |      |               |      |   |      |   | levels"},
    {4, "                                                           |    "},
    {4, "   |      |   |      |               |      |   |      |   |    "},
    {3, "   `-incr-'   `-incr-'               `-incr-'   `-incr-'  -|    "},
    {2, "       |          |                      |          |      |    "},
    {2, "       `---incr---'                      `---incr---'     -'    "},
    {1, "            |                                 |                 "},
    {1, "            `----------------.----------------'                 "},
    {0, "                             |                                  "},
    {0, "                             x                                  "}};

  std::cout << std::endl;
  for (const auto p : desc)
  {
    if (exponent > p.first)
      std::cout << p.second << std::endl;
  }
  std::cout << std::endl;

  return {ConstructBinary(exponent, x), 3 * (1 << (exponent - 1)) - 3};
}

class array_data
{
public:
  array_data(int x = 0)
  {
    for (auto& v : values)
      v = x;
  }

  bool operator==(const array_data& other) const
  {
    return values == other.values;
  }

  bool operator!=(const array_data& other) const
  {
    return !(*this == other);
  }

  array_data& operator++()
  {
    for (auto& v : values)
      ++v;

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& out, const array_data& data)
  {
    out << "array_data{" << size << "x" << data.values[0] << "}";
    return out;
  }

private:
  static constexpr std::size_t size = 256;
  std::array<int, size> values;
};

ref<array_data> ConstructLinearSequenceArray(std::size_t exponent,
                                             ref<array_data> x)
{
  if (exponent == 0)
    return x;

  const auto y = ConstructLinearSequenceArray(exponent - 1, x);

  return ConstructLinearSequenceArray(exponent - 1, y++);
}

std::pair<ref<array_data>, std::size_t>
ConstructLinearSequenceArrayAndPrintDescription(std::size_t exponent,
                                                ref<array_data> x)
{
  const auto prev = std::cout.fill('-');

  std::cout << std::endl;
  std::cout << "f(x) = incr( incr( incr( ... incr( x ) ... ) ) )" << std::endl;
  std::cout << "      |                           |" << std::endl;
  std::cout << "      `-----" << std::right << std::setw(10)
            << (1 << exponent) - 1 << " times------'" << std::endl;
  std::cout << std::endl;
  std::cout << "sizeof(x) = " << sizeof(array_data) << " bytes" << std::endl;
  std::cout << std::endl;

  std::cout.fill(prev);

  return {ConstructLinearSequenceArray(exponent, x), (1 << exponent) - 1};
}

template <typename T>
void Benchmark(
  std::function<std::pair<ref<T>, std::size_t>(int, const ref<T>& x)>
    constructor,
  engine_options options = engine_options::nothing)
{
  Engine engine{options};

  const auto exponent = 14;
  const auto interactive_fps = 25;

  const T initial_x{1};
  const T last_x{42};

  T initial_value{};
  T last_value{};
  int total_nodes_count{};

  CheckPoints check_points;

  std::size_t expected_active_nodes_count = 0;
  {
    check_points.start = make_check_point();

    auto x = Var<T>(initial_x);
    const auto info = constructor(exponent, x);
    const auto y = info.first;
    const auto constructed_nodes_count = info.second;
    expected_active_nodes_count = constructed_nodes_count + 3;

    check_points.constructed = make_check_point();

    {
      auto r = Main(y);

      if (introspect::num_active_nodes() != expected_active_nodes_count)
        throw std::logic_error("benchmark logic error");

      check_points.activated = make_check_point();

      initial_value = *r;

      x = last_x;

      check_points.updated = make_check_point();

      last_value = *r;

      total_nodes_count = introspect::num_vertices();
    }

    check_points.deactivated = make_check_point();

    {
      auto r = Main(y);

      check_points.second_time_activated = make_check_point();
    }
    check_points.second_time_deactivated = make_check_point();
  }

  check_points.destructed = make_check_point();

  std::cout << "f(" << initial_x << ") = " << initial_value << std::endl;

  std::cout << "f(" << last_x << ") = " << last_value << std::endl;

  std::cout << std::endl;

  std::cout << "Total nodes count: " << total_nodes_count << std::endl;

  std::cout << std::endl;

  PrintCheckPoints(std::cout, check_points);

  std::cout << std::endl;

  std::cout << "Interactive updates (" << interactive_fps << " FPS):    "
            << static_cast<std::size_t>(
                 expected_active_nodes_count /
                 CalculateUpdateDurationSec(check_points)) /
                 interactive_fps
            << std::endl
            << std::endl;
}

int main(int argc, char** argv)
{
  std::cout << Title1("Dataflow++ benchmark") << std::endl;

  std::cout << Title2("Linear sequence update") << std::endl;

  Benchmark<int>(ConstructLinearSequenceAndPrintDescription);

  std::cout << Title2("Linear sequence update (optimized)") << std::endl;

  Benchmark<int>(ConstructLinearSequenceAndPrintDescription,
                 engine_options::fully_optimized);

  std::cout << Title2("Conditional linear sequence update") << std::endl;

  Benchmark<int>(ConstructConditionalLinearSequenceAndPrintDescription);

  std::cout << Title2("Conditional linear sequence update (optimized)")
            << std::endl;

  Benchmark<int>(ConstructConditionalLinearSequenceAndPrintDescription,
                 engine_options::fully_optimized);

  std::cout << Title2("Binary nodes update") << std::endl;

  Benchmark<int>(ConstructBinaryAndPrintDescription);

  std::cout << Title2("Binary nodes update (optimized)") << std::endl;

  Benchmark<int>(ConstructBinaryAndPrintDescription,
                 engine_options::fully_optimized);

  std::cout << Title2("Linear sequence update (array data)") << std::endl;

  Benchmark<array_data>(ConstructLinearSequenceArrayAndPrintDescription);

  return 0;
}
