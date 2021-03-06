
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

#include <QtCore/QObject>

#include <memory>
#include <ostream>
#include <type_traits>

namespace dataflow2qt
{
namespace internal
{
class qobject_factory;
}

/// A flowable handle for an instance of a class derived from `QObject`.
///
template <typename TQObject> class qhandle
{
  template <typename> friend class qhandle;
  friend class internal::qobject_factory;

  static_assert(std::is_base_of<QObject, TQObject>::value,
                "TQObject must be derived from QObject");

public:
  /// Default constructor. It is required for any type to be flowable.
  ///
  qhandle()
  : p_qobject_{}
  {
  }

  /// Implicit conversion constructor.
  ///
  template <typename TOtherQObject,
            typename = typename std::enable_if<
              std::is_convertible<std::shared_ptr<TOtherQObject>,
                                  std::shared_ptr<TQObject>>::value>::type>
  qhandle(const qhandle<TOtherQObject>& other)
  : p_qobject_{other.p_qobject_}
  {
  }

  /// Returns a raw pointer to the handled object.
  /// The ownership is not passed to the caller's side.
  ///
  TQObject* get() const
  {
    return p_qobject_.get();
  }

  /// Equality comparison.
  ///
  bool operator==(const qhandle& other) const
  {
    return p_qobject_ == other.p_qobject_;
  }

  /// Inequality comparison.
  ///
  bool operator!=(const qhandle& other) const
  {
    return !(*this == other);
  }

private:
  explicit qhandle(std::shared_ptr<TQObject> p_qobject)
  : p_qobject_{std::move(p_qobject)}
  {
  }

private:
  std::shared_ptr<TQObject> p_qobject_;
};

/// Insertion operator.
///
template <typename TQObject>
std::ostream& operator<<(std::ostream& out, const qhandle<TQObject>& v);

/// A flowable handle for an instance of `QObject` class.
///
using qobject = qhandle<QObject>;
}

template <typename TQObject>
std::ostream& dataflow2qt::operator<<(std::ostream& out,
                                      const qhandle<TQObject>& v)
{
  out << TQObject::staticMetaObject.className() << " (";

  if (v.get())
  {
    out << v.get()->metaObject()->className();
  }
  else
  {
    out << "null";
  }

  out << ")";

  return out;
}
