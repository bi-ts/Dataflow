
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

#include <dataflow/qt.h>

#include <dataflow/prelude.h>

#include "tools/graph_invariant.h"
#include "tools/io_fixture.h"

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

#include <chrono>
#include <sstream>
#include <thread>

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

BOOST_AUTO_TEST_CASE(test_is_convertible_to_qml_type)
{
  BOOST_CHECK_EQUAL((dataflow2qt::is_convertible_to_qml_type<int>::value),
                    true);
  BOOST_CHECK_EQUAL(
    (dataflow2qt::is_convertible_to_qml_type<const volatile int&>::value),
    true);
  BOOST_CHECK_EQUAL((dataflow2qt::is_convertible_to_qml_type<void>::value),
                    false);
}

BOOST_AUTO_TEST_CASE(test_cast_to_qvariant_int)
{
  const dataflow2qt::qvariant v = dataflow2qt::cast_to_qvariant(13);

  BOOST_CHECK_EQUAL(v.get().toInt(), 13);
}

BOOST_AUTO_TEST_CASE(test_cast_to_qvariant_qobject)
{
  dataflow::Engine engine;

  const dataflow::val<dataflow2qt::qobject> m = Main(dataflow2qt::QmlContext());

  const auto& obj = *m;

  const dataflow2qt::qvariant& v = dataflow2qt::cast_to_qvariant(obj);

  BOOST_CHECK_EQUAL(qvariant_cast<QObject*>(v.get()), obj.get());
}

BOOST_AUTO_TEST_CASE(test_qobject_default_ctor)
{
  dataflow2qt::qobject obj;

  BOOST_CHECK(obj.get() == nullptr);

  std::stringstream ss;
  ss << obj;

  BOOST_CHECK_EQUAL(ss.str(), "QObject (null)");
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

BOOST_AUTO_TEST_CASE(test_QmlData_listC_minimal)
{
  dataflow::Engine engine;

  auto xs = dataflow::Var<dataflow::listC<int>>();

  const auto ys = Main(dataflow::qt::QmlData(xs));
}

BOOST_AUTO_TEST_CASE(test_QmlContext_minimal)
{
  QCoreApplication app(g_argc, g_argv);

  dataflow::EngineQml engine(app);

  const auto context = dataflow2qt::QmlContext();

  // BOOST_CHECK_EQUAL(dataflow::introspect::label(context), "const");

  const auto m = Main(context);

  BOOST_CHECK((*m).get() != nullptr);

  BOOST_CHECK_EQUAL(dataflow::introspect::value(m),
                    "QObject (dataflow::qt::internal::dynamic_qobject)");
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

  // BOOST_CHECK_EQUAL(dataflow::introspect::label(context), "qml-context");

  const auto m = Main(context);

  BOOST_CHECK((*m).get() != nullptr);

  BOOST_CHECK((*m).get()->property("x").isValid());
  BOOST_CHECK((*m).get()->property("y").isValid());
  BOOST_CHECK((*m).get()->property("z").isValid());
  BOOST_CHECK((*m).get()->property("a").isValid());
  BOOST_CHECK((*m).get()->property("b").isValid());
  BOOST_CHECK((*m).get()->property("c").isValid());
  BOOST_CHECK((*m).get()->property("d").isValid());

  BOOST_CHECK_EQUAL((*m).get()->property("x").toInt(), 1);
  BOOST_CHECK_EQUAL((*m).get()->property("y").toString().toStdString(), "str");
  BOOST_CHECK_EQUAL((*m).get()->property("z").toFloat(), 3.14f);
  BOOST_CHECK_EQUAL((*m).get()->property("a").toInt(), 3);
  BOOST_CHECK_EQUAL((*m).get()->property("b").toString().toStdString(),
                    "str-str");
  BOOST_CHECK_EQUAL((*m).get()->property("c").toFloat(), 6.28f);
  BOOST_CHECK_EQUAL((*m).get()->property("d").toInt(), 1);

  x = 3;

  BOOST_CHECK_EQUAL((*m).get()->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m).get()->property("b").toString().toStdString(),
                    "str-str");
  BOOST_CHECK_EQUAL((*m).get()->property("c").toFloat(), 6.28f);
  BOOST_CHECK_EQUAL((*m).get()->property("d").toInt(), 3);

  y = "abc";

  BOOST_CHECK_EQUAL((*m).get()->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m).get()->property("b").toString().toStdString(),
                    "abc-abc");
  BOOST_CHECK_EQUAL((*m).get()->property("c").toFloat(), 6.28f);
  BOOST_CHECK_EQUAL((*m).get()->property("d").toInt(), 3);

  z = 2.17f;

  BOOST_CHECK_EQUAL((*m).get()->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m).get()->property("b").toString().toStdString(),
                    "abc-abc");
  BOOST_CHECK_EQUAL((*m).get()->property("c").toFloat(), 4.34f);
  BOOST_CHECK_EQUAL((*m).get()->property("d").toInt(), 3);

  (*m).get()->setProperty("x", 4);

  BOOST_CHECK_EQUAL((*m).get()->property("a").toInt(), 12);

  (*m).get()->setProperty("y", "123");

  BOOST_CHECK_EQUAL((*m).get()->property("b").toString().toStdString(),
                    "123-123");

  (*m).get()->setProperty("z", 4.14f);

  BOOST_CHECK_EQUAL((*m).get()->property("c").toFloat(), 8.28f);
}

BOOST_AUTO_TEST_CASE(test_Timeout)
{
  QCoreApplication app(g_argc, g_argv);

  dataflow::EngineQml engine(app);

  io_fixture io;

  const int interval_msec = 2000;

  auto timeout = dataflow::Var(interval_msec);

  io.capture_output();

  const auto y = dataflow::Main([=](dataflow::dtime t0) {
    return dataflow::introspect::Log(dataflow::Timeout(timeout, t0), "x");
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(interval_msec / 2));

  timeout = 0; // Should not matter

  QCoreApplication::processEvents();

  io.reset_output();

  BOOST_CHECK_EQUAL(dataflow::introspect::active_node(timeout), false);

  BOOST_CHECK(graph_invariant_holds());
  BOOST_CHECK_EQUAL(*y, false);

  BOOST_CHECK_EQUAL(io.log_string(), "[t=0] x = false;");

  io.capture_output();

  std::this_thread::sleep_for(std::chrono::milliseconds(interval_msec));

  QCoreApplication::processEvents();

  io.reset_output();

  BOOST_CHECK_EQUAL(*y, false);

  BOOST_CHECK_EQUAL(io.log_string(),
                    "[t=0] x = false;[t=2] x = true;[t=3] x = false;");
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
