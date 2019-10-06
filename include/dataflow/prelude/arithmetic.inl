
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

template <typename ArgT, typename..., typename T>
dataflow::ref<T> dataflow::Abs(const ArgT& x)
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
  return core::Lift<policy>(core::make_argument(x));
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::Add(const ArgX& x, const ArgY& y)
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
  return core::Lift<policy>(core::make_argument(x), core::make_argument(y));
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::Sub(const ArgX& x, const ArgY& y)
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
  return core::Lift<policy>(core::make_argument(x), core::make_argument(y));
}

template <typename ArgT, typename..., typename T>
dataflow::ref<T> dataflow::Plus(const ArgT& x)
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
  return core::Lift<policy>(core::make_argument(x));
}

template <typename ArgT, typename..., typename T>
dataflow::ref<T> dataflow::Inv(const ArgT& x)
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
  return core::Lift<policy>(core::make_argument(x));
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::Mult(const ArgX& x, const ArgY& y)
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
  return core::Lift<policy>(core::make_argument(x), core::make_argument(y));
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::Div(const ArgX& x, const ArgY& y)
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
  return core::Lift<policy>(core::make_argument(x), core::make_argument(y));
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::Mod(const ArgX& x, const ArgY& y)
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
  return core::Lift<policy>(core::make_argument(x), core::make_argument(y));
}

template <typename ArgT, typename..., typename T>
dataflow::ref<T> dataflow::Incr(const ArgT& x)
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
  return core::Lift<policy>(core::make_argument(x));
}

template <typename ArgT, typename..., typename T>
dataflow::ref<T> dataflow::Decr(const ArgT& x)
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
  return core::Lift<policy>(core::make_argument(x));
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::operator+(const ArgX& x, const ArgY& y)
{
  return Add(x, y);
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::operator-(const ArgX& x, const ArgY& y)
{
  return Sub(x, y);
}

template <typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::operator+(const ArgX& x)
{
  return Plus(x);
}

template <typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::operator-(const ArgX& x)
{
  return Inv(x);
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::operator*(const ArgX& x, const ArgY& y)
{
  return Mult(x, y);
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::operator/(const ArgX& x, const ArgY& y)
{
  return Div(x, y);
}

template <typename ArgX, typename ArgY, typename..., typename T>
dataflow::ref<T> dataflow::operator%(const ArgX& x, const ArgY& y)
{
  return Mod(x, y);
}

template <typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::operator++(const ArgX& x)
{
  return Incr(x);
}

template <typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::operator++(const ArgX& x, int)
{
  return Incr(x);
}

template <typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::operator--(const ArgX& x)
{
  return Decr(x);
}

template <typename ArgX, typename..., typename T>
dataflow::ref<T> dataflow::operator--(const ArgX& x, int)
{
  return Decr(x);
}
