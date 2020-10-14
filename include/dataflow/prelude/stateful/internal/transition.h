
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

#pragma once

#include "dataflow++_export.h"

#include <dataflow/prelude/core.h>

#include <ostream>

namespace dataflow
{
namespace stateful
{
namespace internal
{
class DATAFLOW___EXPORT transition
{
public:
  transition();
  transition(integer idx, const dtimestamp& when);

  bool operator==(const transition& other) const;
  bool operator!=(const transition& other) const;

  integer index() const;
  const dtimestamp& when() const;

private:
  integer idx_;
  dtimestamp t_;
};

DATAFLOW___EXPORT std::ostream& operator<<(std::ostream& out,
                                           const transition& value);

DATAFLOW___EXPORT ref<transition> Transition(const arg<integer>& idx, dtime t);

DATAFLOW___EXPORT ref<integer> Index(const ref<transition>& tr);

DATAFLOW___EXPORT ref<dtimestamp> When(const ref<transition>& tr);

} // internal
} // stateful
} // dataflow
