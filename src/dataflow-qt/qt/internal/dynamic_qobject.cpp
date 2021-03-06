
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

#include "dynamic_qobject.h"

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>

namespace dataflow2qt
{
namespace internal
{
static const char* g_class_name = "dataflow::qt::internal::dynamic_qobject";

dynamic_qobject::dynamic_qobject(QObject* p_parent,
                                 qmeta_object_ptr p_meta_object,
                                 properties_container properties,
                                 methods_container methods)
: QObject{p_parent}
, p_meta_object_{std::move(p_meta_object)}
, properties_{std::move(properties)}
, methods_{std::move(methods)}
{
}

const QMetaObject* dynamic_qobject::metaObject() const
{
  return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject()
                                    : p_meta_object_.get();
}

void* dynamic_qobject::qt_metacast(const char* class_name)
{
  if (!class_name)
    return nullptr;

  if (std::string(class_name) == std::string(g_class_name))
    return static_cast<void*>(this);

  return QObject::qt_metacast(class_name);
}

int dynamic_qobject::qt_metacall(QMetaObject::Call call,
                                 int global_idx,
                                 void** pp_args)
{
  const auto local_idx = QObject::qt_metacall(call, global_idx, pp_args);

  if (local_idx < 0)
    return local_idx;

  if (call == QMetaObject::ReadProperty)
  {
    if (local_idx < properties_.size())
    {
      void* p_arg0 = pp_args[0];

      const auto& getter = properties_.at(local_idx).first;

      getter(p_arg0);
    }

    return local_idx - properties_.size();
  }

  if (call == QMetaObject::WriteProperty)
  {
    if (local_idx < properties_.size())
    {
      const void* p_arg0 = pp_args[0];

      const auto& setter = properties_.at(local_idx).second;

      if (setter(p_arg0))
      {
        const QMetaProperty property = metaObject()->property(global_idx);

        if (property.hasNotifySignal())
        {
          property.notifySignal().invoke(this);
        }
      }
    }

    return local_idx - properties_.size();
  }

  if (call == QMetaObject::InvokeMetaMethod)
  {
    if (local_idx < properties_.size() + methods_.size())
    {
      const QMetaMethod method = metaObject()->method(global_idx);

      if (method.methodType() == QMetaMethod::Signal)
      {
        // emit signal
        QMetaObject::activate(this, metaObject(), local_idx, nullptr);
      }
      else
      {
        const auto it = methods_.find(local_idx);

        if (it != methods_.end())
        {
          const auto& method = it->second;
          const void* const* const pp_const_args = pp_args;
          method(pp_const_args);
        }
      }
    }

    return local_idx - properties_.size() - methods_.size();
  }

  if (call == QMetaObject::RegisterMethodArgumentMetaType)
  {
    if (local_idx < properties_.size() + methods_.size())
      *reinterpret_cast<int*>(pp_args[0]) = -1;

    return local_idx - properties_.size() - methods_.size();
  }

  return local_idx;
}
}
}
