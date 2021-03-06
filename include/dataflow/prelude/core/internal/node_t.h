
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

#include "config.h"
#include "node.h"
#include "type_traits.h"

#include <algorithm>
#include <sstream>

namespace dataflow
{
namespace internal
{
namespace detail
{
template <typename T> std::string node_value_to_string(const T& value)
{
  std::stringstream out;

  out << value;

  return out.str();
}

inline std::string node_value_to_string(const bool& value)
{
  std::stringstream out;

  out << std::boolalpha << value;

  return out.str();
}

inline std::string node_value_to_string(const std::string& value)
{
  std::stringstream out;

  out << "\"" << value << "\"";

  return out.str();
}
}

template <typename T> class node_t : public node
{
public:
  static_assert(std::is_default_constructible<T>::value,
                "`T` must be default constructible");

  static_assert(std::is_copy_constructible<T>::value,
                "`T` must be copy constructible");

  static_assert(std::is_copy_assignable<T>::value,
                "`T` must be copy assignable");

  static_assert(is_streamable<T>::value, "`T` must be streamable");

  static_assert(is_equality_comparable<T>::value,
                "`T` must be equality comparable");

public:
  const T& value() const
  {
    return value_;
  }

protected:
  explicit node_t(const T& v = T())
  : value_(v)
  {
  }

  ~node_t()
  {
    // If this condition fails, make sure that `perform_deactivation_()` method
    // is called in the derived classes that override `deactivate_(id)` method.
    DATAFLOW___CHECK_CONDITION_DEBUG(value_ == T{});
  }

  update_status set_value_(const T& v)
  {
    if (value_ == v)
      return update_status::nothing;

    value_ = v;

    return update_status::updated;
  }

  void perform_deactivation_()
  {
    value_ = T{};
  }

private:
  virtual std::string to_string_() const override final
  {
    return detail::node_value_to_string(value_);
  }

  virtual void deactivate_(node_id id) override
  {
    perform_deactivation_();
  }

private:
  T value_;
};

template <typename... Bs> static bool check_all_of(Bs... bs)
{
  const auto values = {bs...};

  return std::all_of(values.begin(), values.end(), [](bool b) { return b; });
}

template <typename T> const T& extract_node_value(const node* p_node)
{
  DATAFLOW___CHECK_PRECONDITION(dynamic_cast<const node_t<T>*>(p_node));

  return static_cast<const node_t<T>*>(p_node)->value();
}
}
}
