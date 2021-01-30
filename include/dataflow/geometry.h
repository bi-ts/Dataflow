
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

#ifndef DATAFLOW___GEOMETRY_H
#define DATAFLOW___GEOMETRY_H

#include "prelude.h"

namespace dataflow
{
template <typename T> class vec2 : public core::composite_base
{
  static_assert(core::is_regular_data_type<T>::value,
                "Only regular flowable data types allowed in vec2<T>");

public:
  vec2();
  vec2(const T& x, const T& y);

  bool operator==(const vec2& other) const;
  bool operator!=(const vec2& other) const;

  const T& x() const;
  const T& y() const;

  vec2 operator-(const vec2& other) const;
  vec2 operator+(const vec2& other) const;

  vec2 operator-() const;
  const vec2& operator+() const;

  T squared_norm() const;

private:
  T x_;
  T y_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const vec2<T>& value);

template <typename Vec> struct vec_data_type
{
};

template <typename T> struct vec_data_type<vec2<T>>
{
  using type = T;
};

template <typename T> using vec_data_type_t = typename vec_data_type<T>::type;

template <typename T> class ref<vec2<T>> : public core::ref_base<vec2<T>>
{
public:
  ref(core::ref_base<vec2<T>> base);

  ref<vec2<T>> operator()(dtime t) const;

  ref<T> x() const;
  ref<T> y() const;
};

template <typename ArgX,
          typename ArgY,
          typename...,
          typename T = core::common_argument_data_type_t<ArgX, ArgY>>
ref<vec2<T>> Vec2(const ArgX& x, const ArgY& y);

template <typename ArgV,
          typename...,
          typename T = vec_data_type_t<core::argument_data_type_t<ArgV>>>
ref<T> SquaredNorm(const ArgV& v);

template <
  typename ArgV1,
  typename ArgV2,
  typename ArgR,
  typename...,
  typename T = vec_data_type_t<core::common_argument_data_type_t<ArgV1, ArgV2>>,
  typename = core::enable_for_argument_data_type_t<ArgR, T>>
ref<bool> PointsClose(const ArgV1& v1, const ArgV2& v2, const ArgR& radius);

enum class dir2
{
  north = 0,
  east,
  south,
  west,
};

DATAFLOW___EXPORT std::ostream& operator<<(std::ostream& out, dir2 v);

DATAFLOW___EXPORT dir2 operator-(dir2 dir);

template <typename T>
ref<vec2<T>> ToVec2(const arg<dir2>& dir,
                    const ref<vec2<T>>& north_dir,
                    const ref<vec2<T>>& east_dir);

template <typename T> ref<vec2<T>> ToVec2(const arg<dir2>& dir);

}

#include "geometry.inl"

#endif // DATAFLOW___GEOMETRY_H
