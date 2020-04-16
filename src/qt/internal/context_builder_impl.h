
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

#include <dataflow/qt/internal/qobject_deleter.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtQml/QQmlPropertyMap>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace dataflow
{
namespace qt
{
namespace internal
{
class context_builder_impl final
{
public:
  context_builder_impl();
  ~context_builder_impl();

  void add_property(const std::string& name, const QVariant& initial_value);

  void add_property(const std::string& name,
                    const QVariant& initial_value,
                    const std::function<void(const QVariant&)>& change_handler);

  std::unique_ptr<QQmlPropertyMap, qobject_deleter> build();

private:
  std::unordered_map<QString, std::function<void(const QVariant&)>>
    change_handlers_;
  std::unique_ptr<QQmlPropertyMap, qobject_deleter> p_context_;
};
}
}
} // dataflow
