
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

#include <dataflow/logical.h>

dataflow::ref<bool> dataflow::Not(const ref<bool>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "not";
    }
    bool calculate(const bool& x)
    {
      return !x;
    }
  };
  return core::Lift<policy>(x);
}

dataflow::ref<bool> dataflow::And(const ref<bool>& x, const ref<bool>& y)
{
  return core::Conditional(x, y, Const<bool>(false));
}

dataflow::ref<bool> dataflow::Or(const ref<bool>& x, const ref<bool>& y)
{
  return core::Conditional(x, Const<bool>(true), y);
}

dataflow::ref<bool> dataflow::AndE(const ref<bool>& x, const ref<bool>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "&";
    }
    bool calculate(const bool& x, const bool& y)
    {
      return x && y;
    }
  };
  return core::Lift<policy>(x, y);
}

dataflow::ref<bool> dataflow::OrE(const ref<bool>& x, const ref<bool>& y)
{
  struct policy
  {
    static std::string label()
    {
      return "|";
    }
    bool calculate(const bool& x, const bool& y)
    {
      return x || y;
    }
  };
  return core::Lift<policy>(x, y);
}
