
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

#define DATAFLOW___NO_BOOST

#include <dataflow/introspect.h>
#include <dataflow/prelude.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>

using namespace dataflow;

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

ref<int> ConstructUnary(std::size_t exponent, ref<int> x)
{
  if (exponent == 0)
    return x;

  const auto y = ConstructUnary(exponent - 1, x);

  return ConstructUnary(exponent - 1, y++);
}

ref<int> ConstructBinary(std::size_t exponent, ref<int> x)
{
  if (exponent == 0)
    return x;

  return ConstructBinary(exponent - 1, x) + ConstructBinary(exponent - 1, x);
}

const double Duration(std::chrono::steady_clock::time_point start,
                      std::chrono::steady_clock::time_point end)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start)
           .count() /
         1000000.0;
}

template <typename T> struct benchmark_check_points
{
  T start;
  T constructed;
  T activated;
  T updated;
  T deactivated;
  T second_time_activated;
  T second_time_deactivated;
  T destructed;
};

void Benchmark(std::function<ref<int>(int, const ref<int>& x)> constructor)
{
  Engine engine;

  const auto exponent = 17;
  const auto interactive_fps = 25;

  int initial_value = 0;
  int last_value = 0;
  int total_nodes_count = 0;

  benchmark_check_points<std::size_t> memory_consumption;
  benchmark_check_points<std::chrono::steady_clock::time_point> time_points;

  {
    time_points.start = std::chrono::steady_clock::now();
    memory_consumption.start = introspect::memory_consumption();

    auto x = Var<int>(1);
    auto y = constructor(exponent, x);

    time_points.constructed = std::chrono::steady_clock::now();
    memory_consumption.constructed = introspect::memory_consumption();

    {
      auto r = Curr(y);

      time_points.activated = std::chrono::steady_clock::now();
      memory_consumption.activated = introspect::memory_consumption();

      initial_value = r();

      x = 42;

      time_points.updated = std::chrono::steady_clock::now();
      memory_consumption.updated = introspect::memory_consumption();

      last_value = r();

      total_nodes_count = introspect::num_vertices();
    }

    time_points.deactivated = std::chrono::steady_clock::now();
    memory_consumption.deactivated = introspect::memory_consumption();

    {
      auto r = Curr(y);

      time_points.second_time_activated = std::chrono::steady_clock::now();
      memory_consumption.second_time_activated =
        introspect::memory_consumption();
    }
    time_points.second_time_deactivated = std::chrono::steady_clock::now();
    memory_consumption.second_time_deactivated =
      introspect::memory_consumption();
  }

  time_points.destructed = std::chrono::steady_clock::now();
  memory_consumption.destructed = introspect::memory_consumption();

  std::cout << "Initial value is:                " << initial_value
            << std::endl;
  std::cout << "Last value is:                   " << last_value << std::endl
            << std::endl;

  std::cout << "Total nodes count:               " << total_nodes_count
            << std::endl
            << std::endl;

  std::cout << "Durations (seconds)" << std::endl;
  std::cout << "Construction: "
            << Duration(time_points.start, time_points.constructed)
            << std::endl;

  std::cout << "Activation*:  "
            << Duration(time_points.constructed, time_points.activated)
            << std::endl;

  std::cout << "Update:       "
            << Duration(time_points.activated, time_points.updated)
            << std::endl;

  std::cout << "Deactivation: "
            << Duration(time_points.updated, time_points.deactivated)
            << std::endl;

  std::cout << "Activation*:  "
            << Duration(time_points.deactivated,
                        time_points.second_time_activated)
            << std::endl;

  std::cout << "Deactivation: "
            << Duration(time_points.second_time_activated,
                        time_points.second_time_deactivated)
            << std::endl;

  std::cout << "Destruction:  "
            << Duration(time_points.second_time_deactivated,
                        time_points.destructed)
            << std::endl
            << std::endl;

  std::cout << "Interactive updates (" << interactive_fps << " FPS):    "
            << static_cast<std::size_t>(
                 std::pow(2, exponent) /
                 Duration(time_points.activated, time_points.updated)) /
                 interactive_fps
            << std::endl
            << std::endl;

  std::cout << "Memory consumption (bytes)" << std::endl;
  std::cout << "Initial:     " << (memory_consumption.start) << std::endl;
  std::cout << "Constructed: " << (memory_consumption.constructed) << std::endl;
  std::cout << "Activated*:  " << (memory_consumption.activated) << std::endl;
  std::cout << "Updated:     " << (memory_consumption.updated) << std::endl;
  std::cout << "Deactivated: " << (memory_consumption.deactivated) << std::endl;
  std::cout << "Activated*:  " << (memory_consumption.second_time_activated)
            << std::endl;
  std::cout << "Deactivated: " << (memory_consumption.second_time_deactivated)
            << std::endl;
  std::cout << "Destructed:  " << (memory_consumption.destructed) << std::endl
            << std::endl;

  std::cout << "*Activation operation also includes initial update" << std::endl
            << std::endl;
}

int main()
{
  std::cout << Title1("Dataflow++ benchmark") << std::endl;

  std::cout << Title2("Unary nodes update") << std::endl;

  Benchmark(ConstructUnary);

  std::cout << Title2("Binary nodes update") << std::endl;

  Benchmark(ConstructBinary);

  return 0;
}
