
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

#if !defined(DATAFLOW___PRELUDE_H)
#error '.inl' file can't be included directly. Use 'prelude.h' instead
#endif

#include "internal/config.h"
#include "internal/node_activator.h"
#include "internal/nodes.h"

#include <cstdlib> // std::abs
#include <string>

#define DATAFLOW___DEFINE_UNARY_FUNCTION_VIA_PREFIX_OPERATOR(func, op, name)   \
  template <typename T> dataflow::ref<T> dataflow::func(const ref<T>& x)       \
  {                                                                            \
    struct policy                                                              \
    {                                                                          \
      static std::string label()                                               \
      {                                                                        \
        return name;                                                           \
      }                                                                        \
      static T calculate(const T& v)                                           \
      {                                                                        \
        return op v;                                                           \
      };                                                                       \
    };                                                                         \
    return Lift<policy>(x);                                                    \
  }

#define DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(func, op, name, U)    \
  template <typename T>                                                        \
  dataflow::ref<U> dataflow::func(const ref<T>& x, const ref<T>& y)            \
  {                                                                            \
    struct policy                                                              \
    {                                                                          \
      static std::string label()                                               \
      {                                                                        \
        return name;                                                           \
      }                                                                        \
      static U calculate(const T& lhs, const T& rhs)                           \
      {                                                                        \
        return lhs op rhs;                                                     \
      };                                                                       \
    };                                                                         \
    return Lift<policy>(x, y);                                                 \
  }

#define DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_INL(func, op, name, T)  \
  inline dataflow::ref<T> dataflow::func(const ref<T>& x, const ref<T>& y)     \
  {                                                                            \
    struct policy                                                              \
    {                                                                          \
      static std::string label()                                               \
      {                                                                        \
        return name;                                                           \
      }                                                                        \
      static T calculate(const T& lhs, const T& rhs)                           \
      {                                                                        \
        return lhs op rhs;                                                     \
      };                                                                       \
    };                                                                         \
    return Lift<policy>(x, y);                                                 \
  }

#define DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR(func, op, name)         \
  DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(func, op, name, T)

#define DATAFLOW___DEFINE_PREFIX_OPERATOR(op, func)                            \
  template <typename T>                                                        \
  dataflow::ref<T> dataflow::operator op(const ref<T>& x)                      \
  {                                                                            \
    return func(x);                                                            \
  }

#define DATAFLOW___DEFINE_POSTFIX_OPERATOR(op, func)                           \
  template <typename T>                                                        \
  dataflow::ref<T> dataflow::operator op(const ref<T>& x, int)                 \
  {                                                                            \
    return func(x);                                                            \
  }

#define DATAFLOW___DEFINE_BINARY_OPERATOR_T(op, func, U)                       \
  template <typename T>                                                        \
  dataflow::ref<U> dataflow::operator op(const ref<T>& x, const ref<T>& y)     \
  {                                                                            \
    return func(x, y);                                                         \
  }                                                                            \
  template <typename T>                                                        \
  dataflow::ref<U> dataflow::operator op(const ref<T>& x, const T& y)          \
  {                                                                            \
    return func(x, Const<T>(y));                                               \
  }                                                                            \
  template <typename T>                                                        \
  dataflow::ref<U> dataflow::operator op(const T& x, const ref<T>& y)          \
  {                                                                            \
    return func(Const<T>(x), y);                                               \
  }

#define DATAFLOW___DEFINE_BINARY_OPERATOR(op, func)                            \
  DATAFLOW___DEFINE_BINARY_OPERATOR_T(op, func, T)

#define DATAFLOW___DEFINE_BINARY_OPERATOR_INL(op, func, T)                     \
  inline dataflow::ref<T> dataflow::operator op(const ref<T>& x,               \
                                                const ref<T>& y)               \
  {                                                                            \
    return func(x, y);                                                         \
  }                                                                            \
  inline dataflow::ref<T> dataflow::operator op(const ref<T>& x, const T& y)   \
  {                                                                            \
    return func(x, Const<T>(y));                                               \
  }                                                                            \
  inline dataflow::ref<T> dataflow::operator op(const T& x, const ref<T>& y)   \
  {                                                                            \
    return func(Const<T>(x), y);                                               \
  }

// ref

template <typename T>
dataflow::ref<T>::ref(const internal::ref& r)
: internal::ref(r)
{
  DF_CHECK_ARGUMENT(r.is_of_type<T>());
}

// eager

template <typename T>
dataflow::eager<T>::eager(const internal::ref& r)
: ref<T>(r)
{
}

template <typename T> const T& dataflow::eager<T>::operator()() const
{
  return this->template value<T>();
}

// var

template <typename T>
dataflow::var<T>::var(const internal::ref& r)
: ref<T>(r)
{
}

template <typename T>
const dataflow::var<T>& dataflow::var<T>::operator=(const T& v) const
{
  DF_CHECK_PRECONDITION(
    dynamic_cast<const internal::node_var<T>*>(this->get_()));

  static_cast<const internal::node_var<T>*>(this->get_())->set_next_value(v);

  this->schedule_();

  return *this;
}

// Basic functions

template <typename T> dataflow::ref<T> dataflow::Const(const T& v)
{
  return ref<T>(internal::node_const<T>::create(v));
}

template <typename T> dataflow::var<T> dataflow::Var(const T& v)
{
  return var<T>(internal::node_var<T>::create(v));
}

template <typename T> dataflow::eager<T> dataflow::Curr(ref<T> x)
{
  return eager<T>(internal::node_current<T>::create(x));
}

// Operators

template <typename T> dataflow::eager<T> dataflow::operator*(ref<T> x)
{
  return Curr(x);
}

// Utility functions

template <typename Policy, typename X, typename T>
dataflow::ref<T> dataflow::Lift(const ref<X>& x, const Policy& policy)
{
  return ref<T>(internal::node_unary<T, X, Policy>::create(x, policy));
}

template <typename F, typename X, typename T>
dataflow::ref<T>
dataflow::Lift(const std::string& label, const ref<X>& x, F func)
{
  class policy
  {
  public:
    policy(const std::string& label, const F& func)
    : label_(label)
    , func_(func)
    {
    }

    std::string label() const
    {
      return label_;
    }

    T calculate(const X& v) const
    {
      return func_(v);
    };

  private:
    std::string label_;
    F func_;
  };

  return Lift<policy>(x, policy(label, func));
}

template <typename Policy, typename X, typename Y, typename T>
dataflow::ref<T>
dataflow::Lift(const ref<X>& x, const ref<Y>& y, const Policy& policy)
{
  return ref<T>(internal::node_binary<T, X, Y, Policy>::create(x, y, policy));
}

template <typename F, typename X, typename Y, typename T>
dataflow::ref<T> dataflow::Lift(const std::string& label,
                                const ref<X>& x,
                                const ref<Y>& y,
                                F func)
{
  class policy
  {
  public:
    policy(const std::string& label, const F& func)
    : label_(label)
    , func_(func)
    {
    }

    const std::string& label() const
    {
      return label_;
    }

    T calculate(const X& x, const Y& y)
    {
      return func_(x, y);
    };

  private:
    std::string label_;
    F func_;
  };

  return Lift(x, y, policy(label, func));
}

// Arithmetic functions

template <typename T> dataflow::ref<T> dataflow::Abs(const ref<T>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "abs";
    }
    T calculate(const T& v)
    {
      return std::abs(v);
    };
  };
  return Lift<policy>(x);
}

DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR(Add, +, "+");
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR(Sub, -, "-");
DATAFLOW___DEFINE_UNARY_FUNCTION_VIA_PREFIX_OPERATOR(Plus, +, "(+)");
DATAFLOW___DEFINE_UNARY_FUNCTION_VIA_PREFIX_OPERATOR(Inv, -, "(-)");
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR(Mult, *, "*");
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR(Div, /, "/");
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR(Mod, %, "mod");
DATAFLOW___DEFINE_UNARY_FUNCTION_VIA_PREFIX_OPERATOR(Incr, 1 +, "incr");
DATAFLOW___DEFINE_UNARY_FUNCTION_VIA_PREFIX_OPERATOR(Decr, (-1) +, "decr");

// Operators

DATAFLOW___DEFINE_BINARY_OPERATOR(+, Add);
DATAFLOW___DEFINE_BINARY_OPERATOR(-, Sub);
DATAFLOW___DEFINE_PREFIX_OPERATOR(+, Plus);
DATAFLOW___DEFINE_PREFIX_OPERATOR(-, Inv);
DATAFLOW___DEFINE_BINARY_OPERATOR(*, Mult);
DATAFLOW___DEFINE_BINARY_OPERATOR(/, Div);
DATAFLOW___DEFINE_BINARY_OPERATOR(%, Mod);
DATAFLOW___DEFINE_PREFIX_OPERATOR(++, Incr);
DATAFLOW___DEFINE_POSTFIX_OPERATOR(++, Incr);
DATAFLOW___DEFINE_PREFIX_OPERATOR(--, Decr);
DATAFLOW___DEFINE_POSTFIX_OPERATOR(--, Decr);

// Comparison functions

DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(Eq, ==, "==", bool);
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(NotEq, !=, "!=", bool);
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(Gr, >, ">", bool);
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(Less, <, "<", bool);
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(GrEq, >=, ">=", bool);
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_T(LessEq, <=, "<=", bool);

// Operators

DATAFLOW___DEFINE_BINARY_OPERATOR_T(==, Eq, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_T(!=, NotEq, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_T(>, Gr, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_T(<, Less, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_T(>=, GrEq, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_T(<=, LessEq, bool);

// Logical functions

inline dataflow::ref<bool> dataflow::Not(const ref<bool>& x)
{
  struct policy
  {
    static std::string label()
    {
      return "not";
    }
    bool calculate(bool v)
    {
      return !v;
    };
  };

  return Lift<policy>(x);
}

inline dataflow::ref<bool> dataflow::And(const ref<bool>& x, const ref<bool>& y)
{
  return If(x, y, false);
}

inline dataflow::ref<bool> dataflow::Or(const ref<bool>& x, const ref<bool>& y)
{
  return If(x, true, y);
}

DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_INL(AndE, &&, "&", bool);
DATAFLOW___DEFINE_BINARY_FUNCTION_VIA_OPERATOR_INL(OrE, ||, "|", bool);

// Operators

inline dataflow::ref<bool> dataflow::operator!(const ref<bool>& x)
{
  return Not(x);
}

DATAFLOW___DEFINE_BINARY_OPERATOR_INL(&&, And, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_INL(||, Or, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_INL(&, AndE, bool);
DATAFLOW___DEFINE_BINARY_OPERATOR_INL(|, OrE, bool);

// Conditional expressions

template <typename T>
dataflow::ref<T>
dataflow::If(const ref<bool>& x, const ref<T>& y, const ref<T>& z)
{
  return ref<T>(
    internal::node_if<T>::create(internal::node_activator::create(x), y, z));
}

template <typename T>
dataflow::ref<T> dataflow::If(const ref<bool>& x, const T& y, const ref<T>& z)
{
  return If(x, Const<T>(y), z);
}

template <typename T>
dataflow::ref<T> dataflow::If(const ref<bool>& x, const ref<T>& y, const T& z)
{
  return If(x, y, Const<T>(z));
}

template <typename T, typename>
dataflow::ref<T> dataflow::If(const ref<bool>& x, const T& y, const T& z)
{
  return If(x, Const<T>(y), Const<T>(z));
}
