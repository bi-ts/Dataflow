
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

#if !defined(DATAFLOW___STRING_H)
#error '.inl' file can't be included directly. Use 'string.h' instead
#endif

// String functions

template <typename X>
dataflow::ref<std::string> dataflow::ToString(const ref<X>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "ToString";
    }
    static std::string calculate(const X& v)
    {
      std::stringstream out;
      out << v;
      return out.str();
    }
  };

  return core::Lift<policy>(x);
}

template <typename T, typename>
dataflow::ref<std::string> dataflow::ToString(const T& v)
{
  std::stringstream out;
  out << v;
  return Const(out.str());
}

template <typename A, typename B, typename... Args>
dataflow::ref<std::string>
dataflow::ToString(const A& a, const B& b, const Args&... args)
{
  return ToString(a) + ToString(b, args...);
}

template <typename T>
dataflow::ref<T> dataflow::FromString(const ref<std::string>& str, const T& d)
{
  struct policy
  {
    static std::string label()
    {
      return "FromString";
    }
    T calculate(const std::string& str) const
    {
      std::stringstream ss(str);

      T v = d;

      ss >> v;

      if (!ss.eof())
        return d;

      return v;
    };
    const T d;
  };

  return core::Lift<policy>(str, {d});
}

template <>
inline dataflow::ref<std::string>
dataflow::FromString(const ref<std::string>& x, const std::string&)
{
  return x;
}

template <typename T>
dataflow::ref<T> dataflow::FromString(const char* str, const T& d)
{
  std::stringstream ss(str);

  T v = d;

  ss >> v;

  if (!ss.eof())
    return Const(d);

  return Const(v);
}
