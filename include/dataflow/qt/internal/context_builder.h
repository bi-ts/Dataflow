
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

#include <QtCore/QObject>

#include <memory>

namespace dataflow
{
namespace qt
{
namespace internal
{
class DATAFLOW_QT_EXPORT context_builder final
{
public:
  context_builder();
  ~context_builder();

  void add(const std::string& name, const QVariant& value);

  std::shared_ptr<QObject> build();

private:
  std::unique_ptr<class context_builder_impl> p_impl_;
};
}
}
} // dataflow
