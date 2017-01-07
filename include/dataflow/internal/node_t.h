
//  Copyright (c) 2014 - 2016 Maksym V. Bilinets.
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

#include <sstream>

namespace dataflow
{
namespace internal
{

template <typename T> std::string node_value_to_string(const T& value)
{
  std::stringstream out;

  out << value;

  return out.str();
}

template <> inline std::string node_value_to_string<bool>(const bool& value)
{
  std::stringstream out;

  out << std::boolalpha << value;

  return out.str();
}

template <>
inline std::string node_value_to_string<std::string>(const std::string& value)
{
  std::stringstream out;

  out << "\"" << value << "\"";

  return out.str();
}

template <typename T> class node_t : public node
{
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

  bool set_value_(const T& v)
  {
    if (value_ == v)
      return false;

    value_ = v;

    return true;
  }

private:
  virtual std::string to_string_() const override final
  {
    return node_value_to_string(value_);
  }

private:
  T value_;
};

template <typename T> const T& extract_node_value(const node* p_node)
{
  DF_CHECK_ARGUMENT(dynamic_cast<const node_t<T>*>(p_node));

  return static_cast<const node_t<T>*>(p_node)->value();
}
}
}
