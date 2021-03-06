
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

#include "lambda_connector.h"

#include <utility>

namespace dataflow_test
{
QMetaObject::Connection
lambda_connector::connect(QObject* p_sender,
                          const char* signal_name,
                          handler_type handler,
                          Qt::ConnectionType connection_type)
{
  const auto p_qobject = new lambda_connector(p_sender, std::move(handler));

  return QObject::connect(
    p_sender, signal_name, p_qobject, SLOT(handler_slot()), connection_type);
}

lambda_connector::lambda_connector(QObject* p_parent, handler_type handler)
: QObject{p_parent}
, handler_{std::move(handler)}
{
}

void lambda_connector::handler_slot() const
{
  handler_();
}
}
