
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

#include "../tools/io_fixture.h"

#include <dataflow/prelude/core.h>

#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
struct flowable
{
  bool operator==(const flowable&) const;
  bool operator!=(const flowable&) const;

  friend std::ostream& operator<<(std::ostream&, const flowable&);
};

struct no_default_constructor
{
  no_default_constructor() = delete;
  bool operator==(const no_default_constructor&) const;
  bool operator!=(const no_default_constructor&) const;

  friend std::ostream& operator<<(std::ostream&, const no_default_constructor&);
};

struct no_copy_constructor
{
  no_copy_constructor(const no_copy_constructor&) = delete;
  bool operator==(const no_copy_constructor&) const;
  bool operator!=(const no_copy_constructor&) const;

  friend std::ostream& operator<<(std::ostream&, const no_copy_constructor&);
};

struct no_copy_assignment
{
  no_copy_assignment& operator=(const no_copy_assignment&) = delete;
  bool operator==(const no_copy_assignment&) const;
  bool operator!=(const no_copy_assignment&) const;

  friend std::ostream& operator<<(std::ostream&, const no_copy_assignment&);
};

struct no_stream_output
{
  bool operator==(const no_stream_output&) const;
  bool operator!=(const no_stream_output&) const;
};

struct no_equality_test
{
  bool operator!=(const no_equality_test&) const;

  friend std::ostream& operator<<(std::ostream&, const no_equality_test&);
};

struct no_inequality_test
{
  bool operator==(const no_inequality_test&) const;

  friend std::ostream& operator<<(std::ostream&, const no_inequality_test&);
};

struct ref_based : internal::ref
{
  ref_based();
  ref_based& operator=(const ref_based&);
  bool operator==(const ref_based&) const;
  bool operator!=(const ref_based&) const;

  friend std::ostream& operator<<(std::ostream&, const ref_based&);
};

BOOST_AUTO_TEST_SUITE(test_core_type_traits)

BOOST_AUTO_TEST_CASE(test_is_flowable)
{
  BOOST_CHECK_EQUAL(core::is_flowable<flowable>::value, true);

  BOOST_CHECK_EQUAL(core::is_flowable<no_default_constructor>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<no_copy_constructor>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<no_copy_assignment>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<no_stream_output>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<no_equality_test>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<no_inequality_test>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<ref_based>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<const int*>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<const int&>::value, false);
}

BOOST_AUTO_TEST_SUITE_END()
}
