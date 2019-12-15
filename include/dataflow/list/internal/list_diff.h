
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

#include <algorithm>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
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

  const T* operator[](Size idx) const
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

template <typename ForwardIterator, typename T, typename Size>
void print_table(std::ostream& out,
                 ForwardIterator seq_a_from,
                 ForwardIterator seq_a_to,
                 ForwardIterator seq_b_from,
                 ForwardIterator seq_b_to,
                 const table<T, Size>& table)
{
  using value_type = typename ForwardIterator::value_type;

  class helper
  {
  public:
    static std::size_t str_length(const value_type& v)
    {
      std::stringstream ss;
      ss << v;
      return ss.str().size();
    }

    static bool cmp_length(const value_type& lhs, const value_type& rhs)
    {
      return str_length(lhs) < str_length(rhs);
    }

    static std::size_t max_length(const ForwardIterator& from,
                                  const ForwardIterator& to)
    {
      const auto it = std::max_element(from, to, cmp_length);

      if (it == to)
        return 1;

      return str_length(*it);
    }
  };

  const auto original_seq_a_from = seq_a_from;

  std::tie(seq_a_from, seq_b_from) =
    std::mismatch(seq_a_from, seq_a_to, seq_b_from, seq_b_to);

  out << std::endl;
  out << std::string(72, '-') << std::endl;

  if (original_seq_a_from != seq_a_from)
  {
    out << "Common prefix: ";
    for (auto it = original_seq_a_from; it != seq_a_from; ++it)
    {
      out << *it << " ";
    }
    out << std::endl;
    out << std::endl;
  }

  const auto width_a = helper::max_length(seq_a_from, seq_a_to);
  const auto width_b = helper::max_length(seq_b_from, seq_b_to);

  out << std::string(width_a, ' ') << "|" << std::string(width_b + 1, ' ');

  std::stringstream ss;
  for (auto it = seq_b_from; it != seq_b_to; ++it)
  {
    ss << std::setw(width_b) << " " << *it;
  }
  out << ss.str() << std::endl;

  out << std::string(width_a, '-') << "+" << std::string(width_b + 1, '-')
      << std::string(ss.str().size(), '-') << std::endl;

  out << std::string(width_a, ' ') << "|";

  for (int j = 0; j < table.cols(); ++j)
  {
    out << std::setw(width_b) << " " << table[0][j];
  }

  out << std::endl;

  auto it = seq_a_from;
  for (int i = 1; i < table.rows(); ++i, ++it)
  {
    out << std::setw(width_a) << *it << "|";

    for (int j = 0; j < table.cols(); ++j)
    {
      out << std::setw(width_b) << " " << table[i][j];
    }

    out << std::endl;
  }

  out << std::string(72, '-') << std::endl;
}
} // list_internal
} // dataflow
