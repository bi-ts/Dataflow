
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

#include "qobject_deleter.h"

#include <dataflow-qt/qt/qhandle.h>

namespace dataflow2qt
{
namespace internal
{
class qobject_factory
{
public:
  template <typename TQObject>
  static qhandle<TQObject> make_shared(TQObject* p_qobject)
  {
    debug_track_(p_qobject);

    return qhandle<TQObject>{
      std::shared_ptr<TQObject>{p_qobject, qobject_deleter{}}};
  }

  template <typename TQObject, typename... Args>
  static qhandle<TQObject> create(Args&&... args)
  {
    return make_shared<TQObject>(new TQObject{std::forward<Args>(args)...});
  }

  static void on_shutdown();

private:
  static void debug_track_(QObject* p_qobject);
};
}
}
