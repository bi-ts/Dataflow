
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

#include "context_builder_impl.h"

#include <dataflow/utility/std_future.h>

namespace dataflow
{
namespace qt
{
namespace internal
{
context_builder_impl::context_builder_impl()
: p_context_(std14::make_unique<QQmlPropertyMap>())
{
}

context_builder_impl::~context_builder_impl()
{
}

void context_builder_impl::add_property(const std::string& name,
                                        const QVariant& initial_value)
{
  p_context_->insert(QString::fromUtf8(name.c_str()), initial_value);
}

void context_builder_impl::add_property(
  const std::string& name,
  const QVariant& initial_value,
  const std::function<void(const QVariant&)>& change_handler)
{
  add_property(name, initial_value);

  p_context_->connect(
    p_context_.get(),
    &QQmlPropertyMap::valueChanged,
    [name, change_handler](const QString& n, const QVariant& value) {
      if (n == QString::fromUtf8(name.c_str()))
        change_handler(value);
    });
}

std::shared_ptr<QObject> context_builder_impl::build()
{
  return std::move(p_context_);
}
}
}
}
