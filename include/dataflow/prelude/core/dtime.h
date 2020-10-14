
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

#include <cstddef>
#include <ostream>

namespace dataflow
{

/// \defgroup core
/// \ingroup prelude
/// \{

namespace internal
{
class tick_count;
class node_time;
}

using dtime = const internal::tick_count&;

class DATAFLOW___EXPORT dtimestamp final
{
  friend class internal::node_time;

public:
  dtimestamp();

  operator std::size_t() const;

  bool operator==(const dtimestamp& other) const;
  bool operator!=(const dtimestamp& other) const;

private:
  explicit dtimestamp(std::size_t ticks);

private:
  std::size_t timestamp_;
};

DATAFLOW___EXPORT std::ostream& operator<<(std::ostream&, const dtimestamp&);
/// \}
}
