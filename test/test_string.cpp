
//  Copyright (c) 2014 - 2019 Maksym V. Bilinets.
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

#include <dataflow/string.h>

#include <dataflow/introspect.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{

class test_string_basic
{
protected:
  test_string_basic()
  {
  }

private:
  Engine engine_;
};

BOOST_AUTO_TEST_SUITE(test_string)

BOOST_FIXTURE_TEST_CASE(test_ToString_int, test_string_basic)
{
  auto x = Var<int>(42);
  auto y = ToString(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "ToString");

  BOOST_CHECK_EQUAL(f(), "42");

  x = 53;

  BOOST_CHECK_EQUAL(f(), "53");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_string, test_string_basic)
{
  auto x = Var("str");
  auto y = ToString(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "var");

  BOOST_CHECK_EQUAL(f(), "str");

  x = "long string";

  BOOST_CHECK_EQUAL(f(), "long string");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_int_leteral, test_string_basic)
{
  auto y = ToString(5);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), "5");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_string_leteral, test_string_basic)
{
  auto y = ToString("string literal");
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), "string literal");
}

BOOST_FIXTURE_TEST_CASE(test_ToString_concatenation, test_string_basic)
{
  auto x = Var<int>(42);
  auto y = Var<float>(42.5);
  auto z = ToString(x, " != ", y);
  auto f = *z;

  BOOST_CHECK_EQUAL(f(), "42 != 42.5");

  x = 43;

  BOOST_CHECK_EQUAL(f(), "43 != 42.5");
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int, test_string_basic)
{
  auto x = Var("42");
  auto y = FromString<int>(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "FromString");

  BOOST_CHECK_EQUAL(f(), 42);

  x = "55";

  BOOST_CHECK_EQUAL(f(), 55);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int_default, test_string_basic)
{
  auto x = Var("NAN");
  auto y = FromString<int>(x, 5);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "FromString");

  BOOST_CHECK_EQUAL(f(), 5);

  x = "55";

  BOOST_CHECK_EQUAL(f(), 55);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_int_from_empty_string,
                        test_string_basic)
{
  auto x = Var("");
  auto y = FromString<int>(x, 5);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "FromString");

  BOOST_CHECK_EQUAL(f(), 5);

  x = "55";

  BOOST_CHECK_EQUAL(f(), 55);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_string, test_string_basic)
{
  auto x = Var("str");
  auto y = FromString<std::string>(x);
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "var");

  BOOST_CHECK_EQUAL(f(), "str");

  x = "long string";

  BOOST_CHECK_EQUAL(f(), "long string");
}

BOOST_FIXTURE_TEST_CASE(test_FromString_string_default, test_string_basic)
{
  auto x = Var("str");
  auto y = FromString<std::string>(x, "never used");
  auto f = *y;

  BOOST_CHECK_EQUAL(introspect::label(y), "var");

  BOOST_CHECK_EQUAL(f(), "str");

  x = "long string";

  BOOST_CHECK_EQUAL(f(), "long string");
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_string_literal, test_string_basic)
{
  auto x = FromString<int>("54");
  auto f = *x;

  BOOST_CHECK_EQUAL(introspect::label(x), "const");

  BOOST_CHECK_EQUAL(f(), 54);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_string_literal_default,
                        test_string_basic)
{
  auto x = FromString<int>("NAN", 3);
  auto y = FromString<int>("39", 10);
  auto f = *(x + y);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), 42);
}

BOOST_FIXTURE_TEST_CASE(test_FromString_from_empty_string_literal,
                        test_string_basic)
{
  auto x = FromString<int>("", 13);
  auto y = FromString<int>("29", 10);
  auto f = *(x + y);

  BOOST_CHECK_EQUAL(introspect::label(x), "const");
  BOOST_CHECK_EQUAL(introspect::label(y), "const");

  BOOST_CHECK_EQUAL(f(), 42);
}

BOOST_AUTO_TEST_SUITE_END()

} // dataflow_test
