
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

#include <dataflow/prelude/core.h>

#include <ostream>

namespace dataflow
{
namespace stateful
{
namespace internal
{
template <typename M, typename V> class memo_value
{
public:
  memo_value() = default;
  memo_value(const M& memo, const V& value)
  : memo_{memo}
  , value_{value}
  {
  }

  bool operator==(const memo_value& other) const
  {
    return memo_ == other.memo_ && value_ == other.value_;
  }

  bool operator!=(const memo_value& other) const
  {
    return !(*this == other);
  }

  const M& memo() const
  {
    return memo_;
  }

  const V& value() const
  {
    return value_;
  }

private:
  M memo_;
  V value_;
};

template <typename M, typename V>
std::ostream& operator<<(std::ostream& out, const memo_value<M, V>& value)
{
  return out << "{ " << value.memo() << "; " << value.value() << "}";
}

template <typename ArgMemo,
          typename ArgValue,
          typename...,
          typename M = core::argument_data_type_t<ArgMemo>,
          typename V = core::argument_data_type_t<ArgValue>>
ref<memo_value<M, V>> MemoValue(const ArgMemo& memo, const ArgValue& value)
{
  struct policy
  {
    static std::string label()
    {
      return "memo_value";
    }
    static memo_value<M, V> calculate(const M& x, const V& y)
    {
      return memo_value<M, V>(x, y);
    }
  };

  return core::Lift<policy>(core::make_argument(memo),
                            core::make_argument(value));
}

template <typename M, typename V> ref<M> Memo(const ref<memo_value<M, V>>& v)
{
  struct policy
  {
    static std::string label()
    {
      return "memo_value-memo";
    }
    static const M& calculate(const memo_value<M, V>& v)
    {
      return v.memo();
    }
  };

  return core::Lift<policy>(v);
}

template <typename M, typename V> ref<V> Value(const ref<memo_value<M, V>>& v)
{
  struct policy
  {
    static std::string label()
    {
      return "memo_value-value";
    }
    static const V& calculate(const memo_value<M, V>& v)
    {
      return v.value();
    }
  };

  return core::Lift<policy>(v);
}
} // internal
} // stateful
} // dataflow
