
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

#ifndef DATAFLOW___MACRO_H
#define DATAFLOW___MACRO_H

#include "prelude.h"
#include "tuple.h"

#ifdef DATAFLOW___NO_BOOST
// TODO: implement without boost
#else
#include <boost/preprocessor.hpp>

#define DATAFLOW___COMMA_IF BOOST_PP_COMMA_IF
#define DATAFLOW___TUPLE_ELEM BOOST_PP_TUPLE_ELEM
#define DATAFLOW___STRINGIZE BOOST_PP_STRINGIZE
#define DATAFLOW___TUPLE_TO_LIST BOOST_PP_TUPLE_TO_LIST
#define DATAFLOW___LIST_FOR_EACH_I BOOST_PP_LIST_FOR_EACH_I
#endif

/// \defgroup macro
/// \{

#define DATAFLOW___DECLARE_ARGUMENTS(r, _, idx, field_type_name)               \
  DATAFLOW___COMMA_IF(idx)                                                     \
  const DATAFLOW___TUPLE_ELEM(2, 0, field_type_name) &                         \
    DATAFLOW___TUPLE_ELEM(2, 1, field_type_name)

#define DATAFLOW___PASS_ARGUMENTS(r, _, idx, field_type_name)                  \
  DATAFLOW___COMMA_IF(idx) DATAFLOW___TUPLE_ELEM(2, 1, field_type_name)

#define DATAFLOW___PASS_FIELDS_TYPES(r, _, idx, field_type_name)               \
  DATAFLOW___COMMA_IF(idx) DATAFLOW___TUPLE_ELEM(2, 0, field_type_name)

#define DATAFLOW___DEFINE_SELECTOR(r, name, idx, field_type_name)              \
  friend ::dataflow::ref<::dataflow::core::argument_data_type_t<               \
    DATAFLOW___TUPLE_ELEM(2, 0, field_type_name)>>                             \
    DATAFLOW___TUPLE_ELEM(2, 1, field_type_name)(                              \
      const ::dataflow::arg<name>& x)                                          \
  {                                                                            \
    struct policy                                                              \
    {                                                                          \
      static std::string label()                                               \
      {                                                                        \
        return #name "-" DATAFLOW___STRINGIZE(                                 \
          DATAFLOW___TUPLE_ELEM(2, 1, field_type_name));                       \
      }                                                                        \
      static DATAFLOW___TUPLE_ELEM(2, 0, field_type_name)                      \
        calculate(const name& v)                                               \
      {                                                                        \
        return ::dataflow::get<idx>(v.data_);                                  \
      }                                                                        \
    };                                                                         \
    return ::dataflow::core::LiftSelector<policy>(x);                          \
  }

#define DATAFLOW_DATA(name, ...)                                               \
  class name                                                                   \
  : public ::dataflow::core::data_type_tag_t<DATAFLOW___LIST_FOR_EACH_I(       \
      DATAFLOW___PASS_FIELDS_TYPES,                                            \
      _,                                                                       \
      DATAFLOW___TUPLE_TO_LIST((__VA_ARGS__)))>                                \
  {                                                                            \
  public:                                                                      \
    explicit name() = default;                                                 \
                                                                               \
    explicit name(                                                             \
      DATAFLOW___LIST_FOR_EACH_I(DATAFLOW___DECLARE_ARGUMENTS,                 \
                                 _,                                            \
                                 DATAFLOW___TUPLE_TO_LIST((__VA_ARGS__))))     \
    : data_(                                                                   \
        DATAFLOW___LIST_FOR_EACH_I(DATAFLOW___PASS_ARGUMENTS,                  \
                                   _,                                          \
                                   DATAFLOW___TUPLE_TO_LIST((__VA_ARGS__))))   \
    {                                                                          \
    }                                                                          \
                                                                               \
    bool operator==(const name& other) const                                   \
    {                                                                          \
      return data_ == other.data_;                                             \
    }                                                                          \
                                                                               \
    bool operator!=(const name& other) const                                   \
    {                                                                          \
      return !(*this == other);                                                \
    }                                                                          \
                                                                               \
    friend std::ostream& operator<<(std::ostream& out, const name& value)      \
    {                                                                          \
      out << #name;                                                            \
      return out;                                                              \
    }                                                                          \
                                                                               \
    DATAFLOW___LIST_FOR_EACH_I(DATAFLOW___DEFINE_SELECTOR,                     \
                               name,                                           \
                               DATAFLOW___TUPLE_TO_LIST((__VA_ARGS__)))        \
                                                                               \
  private:                                                                     \
    ::dataflow::tuple<DATAFLOW___LIST_FOR_EACH_I(                              \
      DATAFLOW___PASS_FIELDS_TYPES,                                            \
      _,                                                                       \
      DATAFLOW___TUPLE_TO_LIST((__VA_ARGS__)))>                                \
      data_;                                                                   \
  }

/// \}

#include "macro.inl"

#endif // DATAFLOW___MACRO_H
