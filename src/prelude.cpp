
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include <dataflow/prelude.h>

#include "internal/engine.h"

namespace dataflow
{
Engine::Engine()
{
  internal::engine::start();
}

Engine::~Engine()
{
  internal::engine::stop();
}
} // dataflow

dataflow::ref<std::string> dataflow::Const(const char* v)
{
  return ref<std::string>(
    internal::node_const<std::string>::create(std::string(v)));
}

dataflow::var<std::string> dataflow::Var(const char* v)
{
  return Var(std::string(v));
}

