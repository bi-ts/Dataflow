
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

#pragma once

#ifndef DATAFLOW___RANDOM_H
#define DATAFLOW___RANDOM_H

#include "dataflow++_export.h"

#include "prelude.h"

#include <cstdint>

namespace dataflow
{
namespace random
{
using random_number = std::uint32_t;

DATAFLOW___EXPORT ref<random_number> DefaultSeed();

/// A linear congruential random number generator (LCG).
///
/// Yields the number from the pseudo-randomized sequence that succeeds the
/// `seed` number. If the `seed` number is not provided, `DefaultSeed()` is
/// used instead.
/// \{
DATAFLOW___EXPORT ref<random_number>
GenerateLCG(const arg<random_number>& seed);

DATAFLOW___EXPORT ref<random_number> GenerateLCG();
/// \}

}
}

#include "random.inl"

#endif // DATAFLOW___RANDOM_H
