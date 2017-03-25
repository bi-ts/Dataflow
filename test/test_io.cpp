
//  Copyright (c) 2014 - 2017 Maksym V. Bilinets.
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

#include "tools/io_fixture.h"

#include <boost/test/unit_test.hpp>

namespace dataflow_test
{

class test_io_fixture : public io_fixture
{
protected:
  test_io_fixture()
  {
  }

  ~test_io_fixture()
  {
  }
};

BOOST_AUTO_TEST_SUITE(test_io)

BOOST_FIXTURE_TEST_CASE(test_dummy, test_io_fixture)
{
  capture_output();

  std::cerr << "text to cerr" << std::endl << "more text to cerr";

  reset_output();

  BOOST_CHECK_EQUAL(str(StreamIndex::StdErr), "text to cerr;more text to cerr");

  capture_output();

  std::clog << "text to clog" << std::endl << "second line to clog";

  reset_output();

  BOOST_CHECK_EQUAL(str(StreamIndex::StdLog),
                    "text to clog;second line to clog");

  capture_output();

  std::cout << "text to cout";

  reset_output();

  BOOST_CHECK_EQUAL(str(StreamIndex::StdOut), "text to cout");
}

BOOST_AUTO_TEST_SUITE_END()
}
