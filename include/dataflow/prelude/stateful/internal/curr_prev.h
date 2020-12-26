
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

#include <dataflow/prelude/core.h>

#include <ostream>

namespace dataflow
{
namespace stateful
{
namespace internal
{
template <typename T> class curr_prev
{
public:
  curr_prev() = default;
  curr_prev(const T& curr, const T& prev)
  : curr_{curr}
  , prev_{prev}
  {
  }

  bool operator==(const curr_prev& other) const
  {
    return curr_ == other.curr_ && prev_ == other.prev_;
  }

  bool operator!=(const curr_prev& other) const
  {
    return !(*this == other);
  }

  const T& curr() const
  {
    return curr_;
  }

  const T& prev() const
  {
    return prev_;
  }

private:
  T curr_;
  T prev_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const curr_prev<T>& value)
{
  return out << "{ " << value.curr() << "; " << value.prev() << "}";
}

template <typename ArgCurr,
          typename ArgPrev,
          typename...,
          typename T = core::common_argument_data_type_t<ArgCurr, ArgPrev>>
ref<curr_prev<T>> CurrPrev(const ArgCurr& curr, const ArgPrev& prev)
{
  struct policy
  {
    static std::string label()
    {
      return "curr_prev";
    }
    static curr_prev<T> calculate(const T& x, const T& y)
    {
      return curr_prev<T>(x, y);
    }
  };

  return core::Lift<policy>(core::make_argument(curr),
                            core::make_argument(prev));
}

template <typename T> ref<T> Curr(const ref<curr_prev<T>>& v)
{
  struct policy
  {
    static std::string label()
    {
      return "curr_prev-curr";
    }
    static const T& calculate(const curr_prev<T>& v)
    {
      return v.curr();
    }
  };

  return core::Lift<policy>(v);
}

template <typename T> ref<T> Prev(const ref<curr_prev<T>>& v)
{
  struct policy
  {
    static std::string label()
    {
      return "curr_prev-prev";
    }
    static const T& calculate(const curr_prev<T>& v)
    {
      return v.prev();
    }
  };

  return core::Lift<policy>(v);
}
} // internal
} // stateful
} // dataflow
