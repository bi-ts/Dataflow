
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

#include "qobject_factory.h"

#include "qobject_tracker.h"

#include <iostream>

namespace dataflow
{
namespace qt
{
namespace internal
{
void qobject_factory::on_shutdown()
{
#ifndef NDEBUG
  if (qobject_tracker::counter())
  {
    std::cerr << "[dataflow2qt] Memory leak detected. "
              << qt::internal::qobject_tracker::counter()
              << " QObjects are still alive." << std::endl;

    qobject_tracker::print_all(std::cerr);

    std::exit(-1);
  }
#endif
}

void qobject_factory::debug_track_(QObject* p_qobject)
{
#ifndef NDEBUG
  qobject_tracker::track(p_qobject);
#endif
}
}
}
}
