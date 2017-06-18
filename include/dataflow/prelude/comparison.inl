
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

#if !defined(DATAFLOW___COMPARISON_H)
#error '.inl' file can't be included directly. Use 'comparison.h' instead
#endif

template <typename T>
dataflow::ref<bool> dataflow::Eq(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "==";
    }
    bool calculate(const T& x, const T& y)
    {
      return x == y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::NotEq(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "!=";
    }
    bool calculate(const T& x, const T& y)
    {
      return x != y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::Gr(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return ">";
    }
    bool calculate(const T& x, const T& y)
    {
      return x > y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::Less(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "<";
    }
    bool calculate(const T& x, const T& y)
    {
      return x < y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::GrEq(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return ">=";
    }
    bool calculate(const T& x, const T& y)
    {
      return x >= y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::LessEq(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "<=";
    }
    bool calculate(const T& x, const T& y)
    {
      return x <= y;
    }
  };
  return core::Lift<policy>(x, y);
}

// Equal to

template <typename T>
dataflow::ref<bool> dataflow::operator==(const ref<T>& x, const ref<T>& y)
{
  return Eq(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator==(const ref<T>& x, const T& y)
{
  return x == Const<T>(y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator==(const T& x, const ref<T>& y)
{
  return Const<T>(x) == y;
}

// Not equal to

template <typename T>
dataflow::ref<bool> dataflow::operator!=(const ref<T>& x, const ref<T>& y)
{
  return NotEq(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator!=(const ref<T>& x, const T& y)
{
  return x != Const<T>(y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator!=(const T& x, const ref<T>& y)
{
  return Const<T>(x) != y;
}

// Greater than

template <typename T>
dataflow::ref<bool> dataflow::operator>(const ref<T>& x, const ref<T>& y)
{
  return Gr(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator>(const ref<T>& x, const T& y)
{
  return x > Const<T>(y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator>(const T& x, const ref<T>& y)
{
  return Const<T>(x) > y;
}

// Less than

template <typename T>
dataflow::ref<bool> dataflow::operator<(const ref<T>& x, const ref<T>& y)
{
  return Less(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator<(const ref<T>& x, const T& y)
{
  return x < Const<T>(y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator<(const T& x, const ref<T>& y)
{
  return Const<T>(x) < y;
}

// Greater than or equal to

template <typename T>
dataflow::ref<bool> dataflow::operator>=(const ref<T>& x, const ref<T>& y)
{
  return GrEq(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator>=(const ref<T>& x, const T& y)
{
  return x >= Const<T>(y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator>=(const T& x, const ref<T>& y)
{
  return Const<T>(x) >= y;
}

// Less than or equal to

template <typename T>
dataflow::ref<bool> dataflow::operator<=(const ref<T>& x, const ref<T>& y)
{
  return LessEq(x, y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator<=(const ref<T>& x, const T& y)
{
  return x <= Const<T>(y);
}

template <typename T>
dataflow::ref<bool> dataflow::operator<=(const T& x, const ref<T>& y)
{
  return Const<T>(x) <= y;
}
