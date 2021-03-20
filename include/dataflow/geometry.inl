
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

#if !defined(DATAFLOW___GEOMETRY_H)
#error "'.inl' file can't be included directly. Use 'geometry.h' instead"
#endif

namespace dataflow
{
template <typename T>
vec2<T>::vec2()
: x_()
, y_()
{
}

template <typename T>
vec2<T>::vec2(const T& x, const T& y)
: x_(x)
, y_(y)
{
}

template <typename T> bool vec2<T>::operator==(const vec2& other) const
{
  return x() == other.x() && y() == other.y();
}

template <typename T> bool vec2<T>::operator!=(const vec2& other) const
{
  return !(*this == other);
}

template <typename T> const T& vec2<T>::x() const
{
  return x_;
}

template <typename T> const T& vec2<T>::y() const
{
  return y_;
}

template <typename T> vec2<T> vec2<T>::operator-(const vec2& other) const
{
  return {x() - other.x(), y() - other.y()};
}

template <typename T> vec2<T> vec2<T>::operator+(const vec2& other) const
{
  return {x() + other.x(), y() + other.y()};
}

template <typename T> vec2<T> vec2<T>::operator-() const
{
  return {-x_, -y_};
}

template <typename T> const vec2<T>& vec2<T>::operator+() const
{
  return *this;
}

template <typename T> T vec2<T>::squared_norm() const
{
  return x() * x() + y() * y();
}
}

template <typename T>
std::ostream& dataflow::operator<<(std::ostream& out, const vec2<T>& v)
{
  return out << "vec2( " << v.x() << " " << v.y() << " )";
}

namespace dataflow
{
template <typename T>
ref<vec2<T>>::ref(core::ref_base<vec2<T>> base)
: core::ref_base<vec2<T>>(base)
{
}

template <typename T> ref<vec2<T>> ref<vec2<T>>::operator()(dtime t) const
{
  return this->snapshot_(t);
}

template <typename T> ref<T> ref<vec2<T>>::x() const
{
  struct policy
  {
    static std::string label()
    {
      return "vec2-x";
    }
    static const T& calculate(const vec2<T>& v)
    {
      return v.x();
    }
  };

  return core::Lift<policy>(*this);
}

template <typename T> ref<T> ref<vec2<T>>::y() const
{
  struct policy
  {
    static std::string label()
    {
      return "vec2-y";
    }
    static const T& calculate(const vec2<T>& v)
    {
      return v.y();
    }
  };

  return core::Lift<policy>(*this);
}
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<dataflow::vec2<T>> dataflow::Vec2(const ArgX& x, const ArgY& y)
{
  struct policy
  {
    static std::string label()
    {
      return "vec2";
    }
    static vec2<T> calculate(const T& x, const T& y)
    {
      return {x, y};
    }
  };

  // TODO: Extend core::Lift to create a constant if all arguments are constants
  //       or literals
  return core::Lift<policy>(core::make_argument(x), core::make_argument(y));
}

template <typename ArgV, typename..., typename T>
dataflow::ref<T> dataflow::SquaredNorm(const ArgV& v)
{
  struct policy
  {
    static std::string label()
    {
      return "squared-norm";
    }
    static T calculate(const vec2<T>& v)
    {
      return v.squared_norm();
    }
  };

  return core::Lift<policy>(core::make_argument(v));
}

template <typename ArgV1,
          typename ArgV2,
          typename ArgR,
          typename...,
          typename T,
          typename>
dataflow::ref<bool>
dataflow::PointsClose(const ArgV1& v1, const ArgV2& v2, const ArgR& radius)
{
  return SquaredNorm(v1 - v2) <= radius * radius;
}

template <typename T>
dataflow::vec2<T>
dataflow::to_vec2(dir2 dir, const vec2<T>& east_dir, const vec2<T>& north_dir)
{
  switch (dir)
  {
  case dir2::north:
    return north_dir;
  case dir2::east:
    return east_dir;
  case dir2::south:
    return -north_dir;
  case dir2::west:
    return -east_dir;
  };

  return vec2<T>(0, 0);
}

template <typename T>
dataflow::ref<dataflow::vec2<T>> dataflow::ToVec2(const arg<dir2>& dir)
{
  struct policy
  {
    static std::string label()
    {
      return "to_vec2";
    }
    static vec2<T> calculate(dir2 d)
    {
      return to_vec2<T>(d);
    }
  };

  return core::Lift<policy>(dir);
}

template <typename..., typename ArgEastDir, typename ArgNorthDir, typename T>
dataflow::ref<dataflow::vec2<T>> dataflow::ToVec2(const arg<dir2>& dir,
                                                  const ArgEastDir& east_dir,
                                                  const ArgNorthDir& north_dir)
{
  struct policy
  {
    static std::string label()
    {
      return "to_vec2";
    }
    static vec2<T> calculate(dir2 d, const vec2<T>& e, const vec2<T>& n)
    {
      return to_vec2(d, e, n);
    }
  };

  return core::Lift<policy>(
    dir, core::make_argument(east_dir), core::make_argument(north_dir));
}
