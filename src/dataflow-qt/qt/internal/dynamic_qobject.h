
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

#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dataflow2qt
{
namespace internal
{
class dynamic_qobject : public QObject
{
public:
  using qmeta_object_ptr =
    std::unique_ptr<QMetaObject, std::function<void(QMetaObject*)>>;

  using get_function_type = std::function<void(void*)>;
  using set_function_type = std::function<bool(const void*)>;
  using method_type = std::function<void(const void* const*)>;

  using properties_container =
    std::vector<std::pair<get_function_type, set_function_type>>;
  using methods_container = std::unordered_map<int, method_type>;

public:
  dynamic_qobject(QObject* p_parent,
                  qmeta_object_ptr p_meta_object,
                  properties_container properties,
                  methods_container methods);

public:
  virtual const QMetaObject* metaObject() const override;
  virtual void* qt_metacast(const char* class_name) override;
  virtual int qt_metacall(QMetaObject::Call, int, void**) override;

private:
  const qmeta_object_ptr p_meta_object_;
  properties_container properties_;
  methods_container methods_;
};
}
}
