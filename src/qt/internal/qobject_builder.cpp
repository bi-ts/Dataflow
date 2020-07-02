
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

#include <dataflow/qt/internal/qobject_builder.h>

#include "dynamic_qobject.h"
#include "qobject_factory.h"

#include <QtCore/private/qmetaobjectbuilder_p.h>

namespace dataflow
{
namespace qt
{
namespace internal
{
class qobject_builder_data
{
public:
  QMetaObjectBuilder meta_object_builder;
  dynamic_qobject::properties_container properties;
  dynamic_qobject::methods_container methods;
};

qobject_builder::qobject_builder()
: p_data_{new qobject_builder_data}
{
  p_data_->meta_object_builder.setClassName(
    "dataflow::qt::internal::dynamic_qobject");
}

qobject_builder::~qobject_builder()
{
}

std::shared_ptr<QObject> qobject_builder::build()
{
  if (!p_data_)
    throw std::logic_error("qobject_builder can't be reused");

  // QMetaObjectBuilder::toMetaObject() converts the meta object builder into
  // a concrete QMetaObject. The return value should be deallocated using free()
  // once it is no longer needed.
  dynamic_qobject::qmeta_object_ptr p_meta_object{
    p_data_->meta_object_builder.toMetaObject(), std::free};

  const auto p_qobject =
    qobject_factory::create<dynamic_qobject>(nullptr,
                                             std::move(p_meta_object),
                                             std::move(p_data_->properties),
                                             std::move(p_data_->methods));

  p_data_ = nullptr;

  return p_qobject;
}

int qobject_builder::add_property_(
  const std::string& name,
  int meta_type_id,
  const std::function<void(void*)>& getter,
  const std::function<bool(const void*)>& setter)
{
  if (getter == nullptr)
    throw std::logic_error("getter can't be nullptr");

  if (!p_data_)
    throw std::logic_error("qobject_builder can't be reused");

  const auto signal_signature = name + "Changed()";

  QMetaMethodBuilder signal_builder =
    p_data_->meta_object_builder.addSignal(signal_signature.c_str());

  QMetaPropertyBuilder property_builder =
    p_data_->meta_object_builder.addProperty(
      name.c_str(), QVariant::typeToName(meta_type_id));

  property_builder.setWritable(setter != nullptr);

  property_builder.setNotifySignal(signal_builder);

  p_data_->properties.emplace_back(getter, setter);

  return p_data_->meta_object_builder.superClass()->methodCount() +
         signal_builder.index();
}

int qobject_builder::add_slot_(
  const std::string& method_signature,
  const std::function<void(const void* const*)>& func)
{
  if (func == nullptr)
    throw std::logic_error("function can't be nullptr");

  if (!p_data_)
    throw std::logic_error("qobject_builder can't be reused");

  QMetaMethodBuilder method_builder =
    p_data_->meta_object_builder.addSlot(method_signature.c_str());

  const auto local_idx = method_builder.index();

  p_data_->methods[local_idx] = func;

  return p_data_->meta_object_builder.superClass()->methodCount() + local_idx;
}
}
}
}
