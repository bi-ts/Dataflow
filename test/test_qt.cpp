
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

#include <dataflow/qt.h>

#include "qt/internal/example_qobject.h"
#include "qt/internal/lambda_connector.h"

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

namespace dataflow2qt = dataflow::qt;

namespace dataflow_test
{
namespace
{
char app_name[] = "test_qt";
char* g_argv[] = {app_name, nullptr};
int g_argc = static_cast<int>((sizeof(g_argv) / sizeof(g_argv[0])) - 1);
}

BOOST_AUTO_TEST_SUITE(test_qt)

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

BOOST_AUTO_TEST_CASE(test_conversion_type_traits)
{
  BOOST_CHECK_EQUAL((dataflow2qt::is_convertible_to_qml_type<int>::value),
                    true);
  BOOST_CHECK_EQUAL(
    (dataflow2qt::is_convertible_to_qml_type<const volatile int&>::value),
    true);
  BOOST_CHECK_EQUAL((dataflow2qt::is_convertible_to_qml_type<void>::value),
                    false);
}

BOOST_AUTO_TEST_CASE(test_QmlData_int)
{
  dataflow::Engine engine;

  auto x = dataflow::Var(1);

  const auto y = dataflow2qt::QmlData(x);

  BOOST_CHECK_EQUAL(dataflow::introspect::label(y), "qml-data");

  const auto z = Main(y);

  BOOST_CHECK_EQUAL(static_cast<QVariant>(*z).toInt(), 1);

  x = 3;

  BOOST_CHECK_EQUAL(static_cast<QVariant>(*z).toInt(), 3);
}

BOOST_AUTO_TEST_CASE(test_QmlData_QObject)
{
  dataflow::Engine engine;

  const auto p_qobject_a = std::shared_ptr<QObject>{new QObject{nullptr}};
  const auto p_qobject_b = std::shared_ptr<QObject>{new QObject{nullptr}};

  auto x = dataflow::Var(p_qobject_a);

  const auto y = dataflow2qt::QmlData(x);

  BOOST_CHECK_EQUAL(dataflow::introspect::label(y), "qml-data");

  const auto z = Main(y);

  BOOST_CHECK_EQUAL((*z).to_qobject(), p_qobject_a);
  BOOST_CHECK_EQUAL(qvariant_cast<QObject*>(*z), p_qobject_a.get());

  x = p_qobject_b;

  BOOST_CHECK_EQUAL((*z).to_qobject(), p_qobject_b);
  BOOST_CHECK_EQUAL(qvariant_cast<QObject*>(*z), p_qobject_b.get());
}

BOOST_AUTO_TEST_CASE(test_EngineQml_instance_throws_if_no_engine)
{
  BOOST_CHECK_THROW(dataflow::EngineQml::instance(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_EngineQml_instance_throws_if_wrong_engine_type)
{
  dataflow::Engine engine;

  BOOST_CHECK_THROW(dataflow::EngineQml::instance(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_QmlContext_minimal)
{
  QCoreApplication app(g_argc, g_argv);

  dataflow::EngineQml engine(app);

  const auto context = dataflow2qt::QmlContext();

  BOOST_CHECK_EQUAL(dataflow::introspect::label(context), "const");

  const auto m = Main(context);

  BOOST_CHECK(*m != nullptr);
}

BOOST_AUTO_TEST_CASE(test_QmlContext)
{
  QCoreApplication app(g_argc, g_argv);

  dataflow::EngineQml engine(app);

  auto x = dataflow::Var(1);
  auto y = dataflow::Var("str");
  auto z = dataflow::Var(3.14f);

  const auto a = x * 3;
  const auto b = y + "-" + y;
  const auto c = z * 2.0f;
  const auto d = x;

  const auto context =
    dataflow2qt::QmlContext(dataflow2qt::QmlPropertyRW("x", x),
                            dataflow2qt::QmlPropertyRW("y", y),
                            dataflow2qt::QmlPropertyRW("z", z),
                            dataflow2qt::QmlProperty("a", a),
                            dataflow2qt::QmlProperty("b", b),
                            dataflow2qt::QmlProperty("c", c),
                            dataflow2qt::QmlProperty("d", d));

  BOOST_CHECK_EQUAL(dataflow::introspect::label(context), "qml-context");

  const auto m = Main(context);

  BOOST_CHECK(*m != nullptr);

  BOOST_CHECK((*m)->property("x").isValid());
  BOOST_CHECK((*m)->property("y").isValid());
  BOOST_CHECK((*m)->property("z").isValid());
  BOOST_CHECK((*m)->property("a").isValid());
  BOOST_CHECK((*m)->property("b").isValid());
  BOOST_CHECK((*m)->property("c").isValid());
  BOOST_CHECK((*m)->property("d").isValid());

  BOOST_CHECK_EQUAL((*m)->property("x").toInt(), 1);
  BOOST_CHECK_EQUAL((*m)->property("y").toString().toStdString(), "str");
  BOOST_CHECK_EQUAL((*m)->property("z").toFloat(), 3.14f);
  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 3);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "str-str");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 6.28f);
  BOOST_CHECK_EQUAL((*m)->property("d").toInt(), 1);

  x = 3;

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "str-str");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 6.28f);
  BOOST_CHECK_EQUAL((*m)->property("d").toInt(), 3);

  y = "abc";

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "abc-abc");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 6.28f);
  BOOST_CHECK_EQUAL((*m)->property("d").toInt(), 3);

  z = 2.17f;

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "abc-abc");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 4.34f);
  BOOST_CHECK_EQUAL((*m)->property("d").toInt(), 3);

  (*m)->setProperty("x", 4);

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 12);

  (*m)->setProperty("y", "123");

  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "123-123");

  (*m)->setProperty("z", 4.14f);

  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 8.28f);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
