
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

#include <dataflow/introspect.h>
#include <dataflow/list.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
BOOST_AUTO_TEST_SUITE(test_list)

BOOST_AUTO_TEST_CASE(test_listA_ctor)
{
  Engine engine;

  const auto a = listA<std::string>{
    "first", std::string("second"), Var("third"), Const("fourth")};
}

BOOST_AUTO_TEST_CASE(test_listA_make_listA)
{
  Engine engine;

  const auto b =
    make_listA("first", std::string("second"), Var("third"), Const("fourth"));
  const auto c =
    make_listA(Const("first"), std::string("second"), Var("third"), "fourth");
  const auto d =
    make_listA(Var("first"), std::string("second"), "third", Const("fourth"));
  const auto e =
    make_listA(std::string("first"), "second", Var("third"), Const("fourth"));
}

BOOST_AUTO_TEST_CASE(test_ListA_Length)
{
  Engine engine;

  var<std::string> a = Var("third");
  ref<std::string> b = Var("fifth");

  const auto c = ListA("first", std::string("second"), a, "fourth", b);

  const auto d = Length(c);

  const auto e = *d;

  BOOST_CHECK_EQUAL(e(), 5);
}

BOOST_AUTO_TEST_SUITE_END()
} // dataflow_test
