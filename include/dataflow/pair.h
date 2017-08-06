
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

#pragma once

#ifndef DATAFLOW___PAIR_H
#define DATAFLOW___PAIR_H

#include "dataflow++_export.h"

#include "prelude.h"

namespace dataflow
{
/// \defgroup pair
/// \{

template <typename T, typename U> class pair final
{
private:
  using impl = std::pair<ref<T>, ref<U>>;

public:
  pair() = default;

  explicit pair(const ref<T>& first, const ref<U>& second);

  bool operator==(const pair& other) const;
  bool operator!=(const pair& other) const;

  friend ref<T> First(const ref<pair>& x)
  {
    struct policy
    {
      static std::string label()
      {
        return "first";
      }
      static const ref<T>& calculate(const pair& v)
      {
        return v.p_impl_->first;
      }
    };

    return core::LiftSelector<policy>(x);
  }

  friend ref<U> Second(const ref<pair>& x)
  {
    struct policy
    {
      static std::string label()
      {
        return "second";
      }
      static const ref<T>& calculate(const pair& v)
      {
        return v.p_impl_->second;
      }
    };

    return core::LiftSelector<policy>(x);
  }

private:
  std::shared_ptr<impl> p_impl_;
};

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, const pair<T, U>& value);

template <typename T, typename U>
pair<T, U> make_pair(const ref<T>& first, const ref<U>& second);

template <typename T, typename U>
ref<pair<T, U>> Pair(const ref<T>& first, const ref<U>& second);

template <typename T, typename U, typename FwT = core::convert_to_flowable_t<T>>
ref<pair<FwT, U>> Pair(const T& first, const ref<U>& second);

template <typename T, typename U, typename FwU = core::convert_to_flowable_t<U>>
ref<pair<T, FwU>> Pair(const ref<T>& first, const U& second);

template <typename T,
          typename U,
          typename FwT = core::convert_to_flowable_t<T>,
          typename FwU = core::convert_to_flowable_t<U>>
ref<pair<FwT, FwU>> Pair(const T& first, const U& second);

/// \}
} // dataflow

#include "pair.inl"

#endif // DATAFLOW___PAIR_H
