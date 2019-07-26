
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

#if !defined(DATAFLOW___ARITHMETIC_H)
#error '.inl' file can't be included directly. Use 'arithmetic.h' instead
#endif

#include <cstdlib>

template <typename T> dataflow::ref<T> dataflow::Abs(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "abs";
    }
    T calculate(const T& x)
    {
      return std::abs(x);
    }
  };
  return core::Lift<policy>(x);
}

template <typename T>
dataflow::ref<T> dataflow::Add(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "+";
    }
    T calculate(const T& x, const T& y)
    {
      return x + y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::Sub(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "-";
    }
    T calculate(const T& x, const T& y)
    {
      return x - y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T> dataflow::ref<T> dataflow::Plus(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "(+)";
    }
    T calculate(const T& x)
    {
      return +x;
    }
  };
  return core::Lift<policy>(x);
}

template <typename T> dataflow::ref<T> dataflow::Inv(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "(-)";
    }
    T calculate(const T& x)
    {
      return -x;
    }
  };
  return core::Lift<policy>(x);
}

template <typename T>
dataflow::ref<T> dataflow::Mult(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "*";
    }
    T calculate(const T& x, const T& y)
    {
      return x * y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::Div(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "/";
    }
    T calculate(const T& x, const T& y)
    {
      return x / y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::Mod(const ref<T>& x, const ref<T>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "mod";
    }
    T calculate(const T& x, const T& y)
    {
      return x % y;
    }
  };
  return core::Lift<policy>(x, y);
}

template <typename T> dataflow::ref<T> dataflow::Incr(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "incr";
    }
    T calculate(T x)
    {
      return ++x;
    }
  };
  return core::Lift<policy>(x);
}

template <typename T> dataflow::ref<T> dataflow::Decr(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "decr";
    }
    T calculate(T x)
    {
      return --x;
    }
  };
  return core::Lift<policy>(x);
}

// Addition

template <typename T>
dataflow::ref<T> dataflow::operator+(const ref<T>& x, const ref<T>& y)
{
  return Add(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::operator+(const ref<T>& x, const T& y)
{
  return x + Const<T>(y);
}

template <typename T>
dataflow::ref<T> dataflow::operator+(const T& x, const ref<T>& y)
{
  return Const<T>(x) + y;
}

// Subtraction

template <typename T>
dataflow::ref<T> dataflow::operator-(const ref<T>& x, const ref<T>& y)
{
  return Sub(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::operator-(const ref<T>& x, const T& y)
{
  return x - Const<T>(y);
}

template <typename T>
dataflow::ref<T> dataflow::operator-(const T& x, const ref<T>& y)
{
  return Const<T>(x) - y;
}

// Unary plus

template <typename T> dataflow::ref<T> dataflow::operator+(const ref<T>& x)
{
  return Plus(x);
}

// Unary minus (additive inverse)

template <typename T> dataflow::ref<T> dataflow::operator-(const ref<T>& x)
{
  return Inv(x);
}

// Multiplication

template <typename T>
dataflow::ref<T> dataflow::operator*(const ref<T>& x, const ref<T>& y)
{
  return Mult(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::operator*(const ref<T>& x, const T& y)
{
  return x * Const<T>(y);
}

template <typename T>
dataflow::ref<T> dataflow::operator*(const T& x, const ref<T>& y)
{
  return Const<T>(x) * y;
}

// Division

template <typename T>
dataflow::ref<T> dataflow::operator/(const ref<T>& x, const ref<T>& y)
{
  return Div(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::operator/(const ref<T>& x, const T& y)
{
  return x / Const<T>(y);
}

template <typename T>
dataflow::ref<T> dataflow::operator/(const T& x, const ref<T>& y)
{
  return Const<T>(x) / y;
}

// Modulo (integer remainder)

template <typename T>
dataflow::ref<T> dataflow::operator%(const ref<T>& x, const ref<T>& y)
{
  return Mod(x, y);
}

template <typename T>
dataflow::ref<T> dataflow::operator%(const ref<T>& x, const T& y)
{
  return x % Const<T>(y);
}

template <typename T>
dataflow::ref<T> dataflow::operator%(const T& x, const ref<T>& y)
{
  return Const<T>(x) % y;
}

// Increment

template <typename T> dataflow::ref<T> dataflow::operator++(const ref<T>& x)
{
  return Incr(x);
}

template <typename T>
dataflow::ref<T> dataflow::operator++(const ref<T>& x, int)
{
  return Incr(x);
}

// Decrement

template <typename T> dataflow::ref<T> dataflow::operator--(const ref<T>& x)
{
  return Decr(x);
}

template <typename T>
dataflow::ref<T> dataflow::operator--(const ref<T>& x, int)
{
  return Decr(x);
}
