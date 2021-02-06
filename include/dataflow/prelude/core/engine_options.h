
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

namespace dataflow
{
/// \defgroup core
/// \ingroup prelude
/// \{

enum class engine_options
{
  nothing = 0x00,
  straight_update_optimization = 0x01,
  fully_optimized = 0x01,
};

inline engine_options operator|(engine_options lhs, engine_options rhs)
{
  using type = std::underlying_type<engine_options>::type;

  return static_cast<engine_options>(static_cast<type>(lhs) |
                                     static_cast<type>(rhs));
}

inline engine_options& operator|=(engine_options& lhs, engine_options rhs)
{
  return lhs = lhs | rhs;
}

inline engine_options operator&(engine_options lhs, engine_options rhs)
{
  using type = std::underlying_type<engine_options>::type;

  return static_cast<engine_options>(static_cast<type>(lhs) &
                                     static_cast<type>(rhs));
}

inline engine_options& operator&=(engine_options& lhs, engine_options rhs)
{
  return lhs = lhs & rhs;
}

/// \}
}
