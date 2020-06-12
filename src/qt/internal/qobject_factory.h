
//  Copyright (c) 2014 - 2020 Maksym V. Bilinets.
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

#include "qobject_deleter.h"

#include <QtCore/QObject>

#include <memory>

namespace dataflow
{
namespace qt
{
namespace internal
{
class qobject_factory
{
public:
  template <typename Object, typename... Args>
  static std::shared_ptr<Object> create(Args&&... args)
  {
    return std::shared_ptr<Object>{new Object{std::forward<Args>(args)...},
                                   qobject_deleter{}};
  }

  template <typename Object>
  static std::shared_ptr<Object> make_shared(Object* p_qobject)
  {
    return std::shared_ptr<Object>{p_qobject, qobject_deleter{}};
  }
};
}
}
}
