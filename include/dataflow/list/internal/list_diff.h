
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

#pragma once

#include <assert.h>

#include <algorithm>
#include <utility>

namespace dataflow
{
namespace list_internal
{
struct fill_table_result
{
  std::size_t skip;
  std::size_t rows;
  std::size_t cols;
};

template <typename ForwardIterator, typename T>
fill_table_result fill_table(ForwardIterator seq_a_from,
                             ForwardIterator seq_a_to,
                             std::size_t seq_a_length,
                             ForwardIterator seq_b_from,
                             ForwardIterator seq_b_to,
                             std::size_t seq_b_length,
                             T* p_table,
                             std::size_t stride)
{
  assert((std::distance(seq_b_from, seq_b_to) + 1) <= stride);

  assert(std::all_of(p_table, p_table + stride, [](T v) { return v == 0; }));

  std::size_t skip = 0;
  while (seq_a_from != seq_a_to && seq_b_from != seq_b_to &&
         *seq_a_from == *seq_b_from)
  {
    ++seq_a_from;
    ++seq_b_from;
    ++skip;
  }

  const auto table = [&](int i, int j) -> T& {
    return p_table[i * stride + j];
  };

  std::size_t i = 1;
  for (auto it_i = seq_a_from; it_i != seq_a_to; ++it_i, ++i)
  {
    assert(table(i, 0) == 0);

    std::size_t j = 1;
    for (auto it_j = seq_b_from; it_j != seq_b_to; ++it_j, ++j)
    {
      if (*it_i == *it_j)
        table(i, j) = table(i - 1, j - 1) + 1;
      else
        table(i, j) = std::max(table(i, j - 1), table(i - 1, j));
    }
  }

  return {skip, seq_a_length - skip + 1, seq_b_length - skip + 1};
}
} // list_internal
} // dataflow
