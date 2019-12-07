
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

#include <dataflow/prelude.h>

#include <benchmark/benchmark.h>

#include <vector>

using namespace dataflow;

static void Construct_NoArgs_Const_Int(benchmark::State& state)
{
  Engine engine;

  std::vector<ref<int>> tmp;
  tmp.reserve(state.max_iterations);

  for (auto _ : state)
  {
    tmp.push_back(Const(0));
  }
}

BENCHMARK(Construct_NoArgs_Const_Int);

static void Construct_NoArgs_Var_Int(benchmark::State& state)
{
  Engine engine;

  std::vector<ref<int>> tmp;
  tmp.reserve(state.max_iterations);

  for (auto _ : state)
  {
    tmp.push_back(Var(0));
  }
}

BENCHMARK(Construct_NoArgs_Var_Int);

static void Construct_Unary_Incr_Int(benchmark::State& state)
{
  Engine engine;

  const auto x = Var(1);

  std::vector<ref<int>> tmp;
  tmp.reserve(state.max_iterations);

  for (auto _ : state)
  {
    tmp.push_back(Incr(x));
  }
}

BENCHMARK(Construct_Unary_Incr_Int);

static void Construct_Binary_Add_Int(benchmark::State& state)
{
  Engine engine;

  const auto x = Var(1);
  const auto y = Var(5);

  std::vector<ref<int>> tmp;
  tmp.reserve(state.max_iterations);

  for (auto _ : state)
  {
    tmp.push_back(Add(x, y));
  }
}

BENCHMARK(Construct_Binary_Add_Int);

static void Construct_Conditional_If_Int(benchmark::State& state)
{
  Engine engine;

  const auto x = Var(true);
  const auto y = Var(5);
  const auto z = Var(9);

  std::vector<ref<int>> tmp;
  tmp.reserve(state.max_iterations);

  for (auto _ : state)
  {
    tmp.push_back(If(x, y, z));
  }
}

BENCHMARK(Construct_Conditional_If_Int);

static void Update_Unary_Incr_Int(benchmark::State& state)
{
  Engine engine;

  auto x = Var(1);

  std::vector<ref<int>> tmp(1, x);

  for (benchmark::IterationCount i = 0; i < state.max_iterations; ++i)
  {
    const auto a = tmp.front();
    tmp.pop_back();
    tmp.push_back(Incr(a));
  }

  const auto y = *tmp.front();

  while (state.KeepRunningBatch(state.max_iterations))
  {
    x = 2;
  }
}

BENCHMARK(Update_Unary_Incr_Int);

static void Destroy_NoArgs_Const_Int(benchmark::State& state)
{
  Engine engine;

  std::vector<ref<int>> tmp;

  for (benchmark::IterationCount i = 0; i < state.max_iterations; ++i)
  {
    tmp.push_back(Const(0));
  }

  for (auto _ : state)
  {
    tmp.pop_back();
  }
}

BENCHMARK(Destroy_NoArgs_Const_Int);

static void Destroy_NoArgs_Var_Int(benchmark::State& state)
{
  Engine engine;

  std::vector<ref<int>> tmp;

  for (benchmark::IterationCount i = 0; i < state.max_iterations; ++i)
  {
    tmp.push_back(Var(0));
  }

  for (auto _ : state)
  {
    tmp.pop_back();
  }
}

BENCHMARK(Destroy_NoArgs_Var_Int);

static void Destroy_Unary_Incr_Int(benchmark::State& state)
{
  Engine engine;

  const auto x = Var(1);

  std::vector<ref<int>> tmp;

  for (benchmark::IterationCount i = 0; i < state.max_iterations; ++i)
  {
    tmp.push_back(Incr(x));
  }

  for (auto _ : state)
  {
    tmp.pop_back();
  }
}

BENCHMARK(Destroy_Unary_Incr_Int);

static void Destroy_Binary_Add_Int(benchmark::State& state)
{
  Engine engine;

  const auto x = Var(1);
  const auto y = Var(1);

  std::vector<ref<int>> tmp;

  for (benchmark::IterationCount i = 0; i < state.max_iterations; ++i)
  {
    tmp.push_back(Add(x, y));
  }

  for (auto _ : state)
  {
    tmp.pop_back();
  }
}

BENCHMARK(Destroy_Binary_Add_Int);

static void Destroy_Conditional_If_Int(benchmark::State& state)
{
  Engine engine;

  const auto x = Var(true);
  const auto y = Var(5);
  const auto z = Var(9);

  std::vector<ref<int>> tmp;

  for (benchmark::IterationCount i = 0; i < state.max_iterations; ++i)
  {
    tmp.push_back(If(x, y, z));
  }

  for (auto _ : state)
  {
    tmp.pop_back();
  }
}

BENCHMARK(Destroy_Conditional_If_Int);

int main(int argc, char** argv)
{
  benchmark::Initialize(&argc, argv);

  if (benchmark::ReportUnrecognizedArguments(argc, argv))
    return 1;

  benchmark::RunSpecifiedBenchmarks();
}
