
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
template <typename T, typename Size> class table
{
public:
  explicit table(T* p_data, Size stride, Size size)
  : p_data_(p_data)
  , stride_(stride)
  , size_(size)
  , rows_(1)
  , cols_(1)
  {
    for (Size i = 0; i < size; i += stride)
      p_data_[i] = 0;

    for (Size i = 0; i < stride; ++i)
      p_data_[i] = 0;
  }

  T* operator[](Size idx)
  {
    return p_data_ + idx * stride_;
  }

  Size rows() const
  {
    return rows_;
  }

  Size cols() const
  {
    return cols_;
  }

  template <typename ForwardIterator>
  Size fill(ForwardIterator seq_a_from,
            ForwardIterator seq_a_to,
            ForwardIterator seq_b_from,
            ForwardIterator seq_b_to,
            Size seq_a_length,
            Size seq_b_length)
  {
    Size skip = 0;
    while (seq_a_from != seq_a_to && seq_b_from != seq_b_to &&
           *seq_a_from == *seq_b_from)
    {
      ++seq_a_from;
      ++seq_b_from;
      ++skip;
    }

    auto& table = *this;

    Size i = 1;
    for (auto it_i = seq_a_from; it_i != seq_a_to; ++it_i, ++i)
    {
      Size j = 1;
      for (auto it_j = seq_b_from; it_j != seq_b_to; ++it_j, ++j)
      {
        if (*it_i == *it_j)
          table[i][j] = table[i - 1][j - 1] + 1;
        else
          table[i][j] = std::max(table[i][j - 1], table[i - 1][j]);
      }
    }

    rows_ = seq_a_length - skip + 1;
    cols_ = seq_b_length - skip + 1;

    return skip;
  }

private:
  T* p_data_;
  Size stride_;
  Size size_;
  Size rows_;
  Size cols_;
};
} // list_internal
} // dataflow
