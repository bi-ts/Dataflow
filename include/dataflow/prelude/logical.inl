
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

#if !defined(DATAFLOW___LOGICAL_H)
#error "'.inl' file can't be included directly. Use 'logical.h' instead"
#endif

namespace dataflow
{
namespace logical
{
namespace detail
{
inline ref<bool> And(const ref<bool>& x)
{
  return x;
}

inline ref<bool> And(const ref<bool>& x, const ref<bool>& y)
{
  return If(x, y, false);
}

template <typename ArgX, typename ArgY, typename ArgZ, typename... Args>
ref<bool> And(const ArgX& x, const ArgY& y, const ArgZ& z, const Args&... args)
{
  return And(x, And(y, z, args...));
}

inline ref<bool> Or(const ref<bool>& x)
{
  return x;
}

inline ref<bool> Or(const ref<bool>& x, const ref<bool>& y)
{
  return If(x, true, y);
}

template <typename ArgX, typename ArgY, typename ArgZ, typename... Args>
ref<bool> Or(const ArgX& x, const ArgY& y, const ArgZ& z, const Args&... args)
{
  return Or(x, Or(y, z, args...));
}

template <typename T> using bool_type_t = bool;

inline bool all_of()
{
  return true;
}

template <typename Arg, typename... Args>
bool all_of(const Arg& x, const Args&... args)
{
  return static_cast<bool>(x) && all_of(args...);
}

inline bool any_of()
{
  return false;
}

template <typename Arg, typename... Args>
bool any_of(const Arg& x, const Args&... args)
{
  return static_cast<bool>(x) || any_of(args...);
}
}
}
}

template <typename Arg, typename... Args, typename>
dataflow::ref<bool> dataflow::And(const Arg& x, const Args&... xs)
{
  return logical::detail::And(core::make_argument(x),
                              core::make_argument(xs)...);
}

template <typename Arg, typename... Args, typename>
dataflow::ref<bool> dataflow::Or(const Arg& x, const Args&... xs)
{
  return logical::detail::Or(core::make_argument(x),
                             core::make_argument(xs)...);
}

template <typename Arg, typename... Args, typename>
dataflow::ref<bool> dataflow::AndE(const Arg& x, const Args&... xs)
{
  struct policy
  {
    static std::string label()
    {
      return "&";
    }
    bool calculate(bool x, logical::detail::bool_type_t<Args>... xs)
    {
      return logical::detail::all_of(x, xs...);
    }
  };
  return core::Lift<policy>(core::make_argument(x), core::make_argument(xs)...);
}

template <typename Arg, typename... Args, typename>
dataflow::ref<bool> dataflow::OrE(const Arg& x, const Args&... xs)
{
  struct policy
  {
    static std::string label()
    {
      return "|";
    }
    bool calculate(bool x, logical::detail::bool_type_t<Args>... xs)
    {
      return logical::detail::any_of(x, xs...);
    }
  };
  return core::Lift<policy>(core::make_argument(x), core::make_argument(xs)...);
}

inline dataflow::ref<bool> dataflow::operator!(const ref<bool>& x)
{
  return Not(x);
}

template <typename ArgX, typename ArgY, typename..., typename>
dataflow::ref<bool> dataflow::operator&&(const ArgX& x, const ArgY& y)
{
  return And(x, y);
}

template <typename ArgX, typename ArgY, typename..., typename>
dataflow::ref<bool> dataflow::operator||(const ArgX& x, const ArgY& y)
{
  return Or(x, y);
}

template <typename ArgX, typename ArgY, typename..., typename>
dataflow::ref<bool> dataflow::operator&(const ArgX& x, const ArgY& y)
{
  return AndE(x, y);
}

template <typename ArgX, typename ArgY, typename..., typename>
dataflow::ref<bool> dataflow::operator|(const ArgX& x, const ArgY& y)
{
  return OrE(x, y);
}
