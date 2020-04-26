
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

#include "qobject_deleter.h"

#include <QtGui/QWindow>

namespace dataflow
{
namespace qt
{
namespace internal
{
void qobject_deleter::operator()(QObject* p_qobject)
{
  if (const auto p_window = qobject_cast<QWindow*>(p_qobject))
    p_window->destroy();

  p_qobject->disconnect();

  p_qobject->deleteLater();
}
}
}
}
