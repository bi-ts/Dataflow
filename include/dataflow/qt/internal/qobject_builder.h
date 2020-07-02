
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

#include "dataflow-qt_export.h"

#include <dataflow/utility/std_future.h>

#include <QtCore/QObject>

#include <functional>
#include <memory>
#include <string>

namespace dataflow
{
namespace qt
{
namespace internal
{
class DATAFLOW_QT_EXPORT qobject_builder final
{
public:
  qobject_builder();
  ~qobject_builder();

  template <typename Getter,
            typename Setter,
            typename T =
              std20::remove_cvref_t<decltype(std::declval<const Getter&>()())>>
  int add_property(const std::string& name,
                   const Getter& getter,
                   const Setter& setter);

  template <typename Getter,
            typename T =
              std20::remove_cvref_t<decltype(std::declval<const Getter&>()())>>
  int add_property(const std::string& name, const Getter& getter);

  std::shared_ptr<QObject> build();

private:
  int add_property_(const std::string& name,
                    int meta_type_id,
                    const std::function<void(void*)>& getter,
                    const std::function<bool(const void*)>& setter);

private:
  std::unique_ptr<class qobject_builder_data> p_data_;
};

template <typename Getter, typename Setter, typename T>
int qobject_builder::add_property(const std::string& name,
                                  const Getter& getter,
                                  const Setter& setter)
{
  return add_property_(
    name,
    qMetaTypeId<T>(),
    [=](void* p_value) { *reinterpret_cast<T*>(p_value) = getter(); },
    [=](const void* p_value) {
      return setter(*reinterpret_cast<const T*>(p_value));
    });
}

template <typename Getter, typename T>
int qobject_builder::add_property(const std::string& name, const Getter& getter)
{
  return add_property_(
    name,
    qMetaTypeId<T>(),
    [=](void* p_value) { *reinterpret_cast<T*>(p_value) = getter(); },
    {});
}
}
}
} // dataflow
