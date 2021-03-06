
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

#include "js_engine.h"

#include <QtQml/QQmlEngine>

#include <stdexcept>

namespace dataflow_test
{
js_engine::js_engine(const std::string& object_name, QObject* p_qobject)
: js_engine_{}
{
  QJSValue js_object = js_engine_.newQObject(p_qobject);

  QQmlEngine::setObjectOwnership(p_qobject, QQmlEngine::CppOwnership);

  js_engine_.globalObject().setProperty(object_name.c_str(), js_object);
}

void js_engine::eval(const std::string& code)
{
  const QJSValue result = js_engine_.evaluate(code.c_str());

  if (result.isError())
  {
    throw std::runtime_error(result.toString().toStdString());
  }
}
} // dataflow_test
