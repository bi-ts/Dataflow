
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

#include <dataflow/qt.h>

#include "example_qobject.h"
#include "lambda_connector.h"

#include <boost/test/unit_test.hpp>

#include <QPointF>
#include <QtCore/QCoreApplication>
#include <QtQml/QJSEngine>
#include <QtQml/QQmlEngine>

namespace dataflow2qt = dataflow::qt;

namespace dataflow_test
{
bool emit_signal(QObject* p_qobject, int global_signal_idx)
{
  const QMetaMethod method = p_qobject->metaObject()->method(global_signal_idx);
  return method.invoke(p_qobject);
}

namespace
{
char app_name[] = "test_qobject_builder";
char* g_argv[] = {app_name, nullptr};
int g_argc = static_cast<int>((sizeof(g_argv) / sizeof(g_argv[0])) - 1);
}

BOOST_AUTO_TEST_SUITE(test_dataflow2qt_internal_qobject_builder)

BOOST_AUTO_TEST_CASE(test_dataflow_test_example_qobject)
{
  example_qobject obj{};

  BOOST_CHECK_EQUAL(obj.metaObject()->className(),
                    "dataflow_test::example_qobject");

  BOOST_CHECK_EQUAL(obj.metaObject()->propertyCount(), 3);
  BOOST_CHECK_EQUAL(obj.metaObject()->methodCount(), 8);

  const auto p_call_count = std::make_shared<int>(0);

  lambda_connector::connect(&obj,
                            SIGNAL(objectNameChanged(const QString&)),
                            [p_call_count]() { (*p_call_count)++; });

  BOOST_CHECK_EQUAL(*p_call_count, 0);

  obj.setProperty("objectName", "someName");

  BOOST_CHECK_EQUAL(*p_call_count, 1);

  obj.setProperty("objectName", "otherName");

  BOOST_CHECK_EQUAL(*p_call_count, 2);
}

BOOST_AUTO_TEST_CASE(test_qt_internal_qobject_builder_minimal_qobject)
{
  QCoreApplication app{g_argc, g_argv};

  dataflow::EngineQml engine{app};

  dataflow2qt::internal::qobject_builder builder;

  const auto p_qobject = builder.build();

  BOOST_REQUIRE(p_qobject != nullptr);

  BOOST_CHECK_EQUAL(p_qobject->metaObject()->className(),
                    "dataflow::qt::internal::dynamic_qobject");

  BOOST_CHECK_EQUAL(p_qobject->metaObject()->propertyCount(), 1);
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->methodCount(), 5);
}

BOOST_AUTO_TEST_CASE(test_qt_internal_qobject_builder_readonly_property)
{
  QCoreApplication app{g_argc, g_argv};

  dataflow::EngineQml engine{app};

  dataflow2qt::internal::qobject_builder builder;

  const auto p_x = std::make_shared<int>(1);
  const auto p_x_change_count = std::make_shared<int>(0);

  const auto x_changed_signal_idx =
    builder.add_property("x", [p_x]() { return *p_x; });

  const auto p_qobject = builder.build();

  BOOST_REQUIRE(p_qobject != nullptr);

  BOOST_REQUIRE_EQUAL(p_qobject->metaObject()->propertyCount(), 2);
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->property(1).name(), "x");
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->property(1).typeName(), "int");
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->property(1).isWritable(), false);

  BOOST_REQUIRE_EQUAL(p_qobject->metaObject()->methodCount(), 6);
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->method(5).methodType(),
                    QMetaMethod::Signal);

  BOOST_CHECK_EQUAL(p_qobject->property("x").toInt(), 1);

  lambda_connector::connect(p_qobject.get(),
                            SIGNAL(xChanged()),
                            [p_x_change_count]() { (*p_x_change_count)++; });

  BOOST_CHECK_EQUAL(*p_x, 1);
  BOOST_CHECK_EQUAL(*p_x_change_count, 0);

  BOOST_CHECK_EQUAL(p_qobject->setProperty("x", 10), false);

  BOOST_CHECK_EQUAL(*p_x, 1);
  BOOST_CHECK_EQUAL(*p_x_change_count, 0);

  *p_x = 33;

  BOOST_CHECK_EQUAL(p_qobject->property("x").toInt(), 33);
  BOOST_CHECK_EQUAL(*p_x, 33);
  BOOST_CHECK_EQUAL(*p_x_change_count, 0);

  BOOST_CHECK(emit_signal(p_qobject.get(), x_changed_signal_idx));

  BOOST_CHECK_EQUAL(*p_x_change_count, 1);
}

BOOST_AUTO_TEST_CASE(test_qt_internal_qobject_builder_rw_property)
{
  QCoreApplication app{g_argc, g_argv};

  dataflow::EngineQml engine{app};

  dataflow2qt::internal::qobject_builder builder;

  const auto p_x = std::make_shared<int>(1);
  const auto p_x_change_count = std::make_shared<int>(0);

  builder.add_property(
    "x",
    [p_x]() { return *p_x; },
    [p_x](int x) {
      if (*p_x != x)
      {
        *p_x = x;
        return true;
      }
      return false;
    });

  const auto p_qobject = builder.build();

  BOOST_REQUIRE(p_qobject != nullptr);

  BOOST_REQUIRE_EQUAL(p_qobject->metaObject()->propertyCount(), 2);
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->property(1).name(), "x");
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->property(1).typeName(), "int");
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->property(1).isWritable(), true);

  BOOST_REQUIRE_EQUAL(p_qobject->metaObject()->methodCount(), 6);
  BOOST_CHECK_EQUAL(p_qobject->metaObject()->method(5).methodType(),
                    QMetaMethod::Signal);

  BOOST_CHECK_EQUAL(p_qobject->property("x").toInt(), 1);

  lambda_connector::connect(p_qobject.get(),
                            SIGNAL(xChanged()),
                            [p_x_change_count]() { (*p_x_change_count)++; });

  BOOST_CHECK_EQUAL(*p_x, 1);
  BOOST_CHECK_EQUAL(*p_x_change_count, 0);

  BOOST_CHECK_EQUAL(p_qobject->setProperty("x", 10), true);

  BOOST_CHECK_EQUAL(*p_x, 10);
  BOOST_CHECK_EQUAL(*p_x_change_count, 1);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
