
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

#include <dataflow/prelude.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{
namespace
{
char app_name[] = "test_qt";
char* g_argv[] = {app_name, nullptr};
int g_argc = static_cast<int>((sizeof(g_argv) / sizeof(g_argv[0])) - 1);
}

BOOST_AUTO_TEST_SUITE(test_qt)

BOOST_AUTO_TEST_CASE(test_EngineQml_instance_throws_if_no_engine)
{
  BOOST_CHECK_THROW(dataflow::EngineQml::instance(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_EngineQml_instance_throws_if_wrong_engine_type)
{
  dataflow::Engine engine;

  BOOST_CHECK_THROW(dataflow::EngineQml::instance(), std::logic_error);
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

  const auto context = dataflow::qt::QmlContext(
    dataflow::qt::RW(dataflow::qt::QmlPropertyRW("x", x),
                     dataflow::qt::QmlPropertyRW("y", y),
                     dataflow::qt::QmlPropertyRW("z", z)),
    dataflow::qt::QmlProperty("a", a),
    dataflow::qt::QmlProperty("b", b),
    dataflow::qt::QmlProperty("c", c));

  const auto m = Main(context);

  BOOST_CHECK(*m != nullptr);

  BOOST_CHECK((*m)->property("x").isValid());
  BOOST_CHECK((*m)->property("y").isValid());
  BOOST_CHECK((*m)->property("z").isValid());
  BOOST_CHECK((*m)->property("a").isValid());
  BOOST_CHECK((*m)->property("b").isValid());
  BOOST_CHECK((*m)->property("c").isValid());

  BOOST_CHECK_EQUAL((*m)->property("x").toInt(), 1);
  BOOST_CHECK_EQUAL((*m)->property("y").toString().toStdString(), "str");
  BOOST_CHECK_EQUAL((*m)->property("z").toFloat(), 3.14f);
  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 3);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "str-str");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 6.28f);

  x = 3;

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "str-str");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 6.28f);

  y = "abc";

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "abc-abc");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 6.28f);

  z = 2.17f;

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 9);
  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "abc-abc");
  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 4.34f);

  (*m)->setProperty("x", 4);

  BOOST_CHECK_EQUAL((*m)->property("a").toInt(), 12);

  (*m)->setProperty("y", "123");

  BOOST_CHECK_EQUAL((*m)->property("b").toString().toStdString(), "123-123");

  (*m)->setProperty("z", 4.14f);

  BOOST_CHECK_EQUAL((*m)->property("c").toFloat(), 8.28f);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
