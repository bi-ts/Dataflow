
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

#include <dataflow/prelude/core.h>

#include "../tools/io_fixture.h"

#include <boost/mpl/list.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/unit_test.hpp>

using namespace dataflow;

namespace dataflow_test
{
template <typename T> struct test_type_traits
{
  static bool is_default_constructible()
  {
    return true;
  }

  static bool is_copy_constructible()
  {
    return true;
  }

  static bool is_copy_assignable()
  {
    return true;
  }

  static bool is_streamable()
  {
    return true;
  }

  static bool is_equality_comparable()
  {
    return true;
  }
};

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

template <>
bool test_type_traits<no_default_constructor>::is_default_constructible()
{
  return false;
}

struct no_copy_constructor
{
  no_copy_constructor() = default;
  no_copy_constructor(const no_copy_constructor&) = delete;
  bool operator==(const no_copy_constructor&) const;
  bool operator!=(const no_copy_constructor&) const;

  friend std::ostream& operator<<(std::ostream&, const no_copy_constructor&);
};

template <> bool test_type_traits<no_copy_constructor>::is_copy_constructible()
{
  return false;
}

struct no_copy_assignment
{
  no_copy_assignment& operator=(const no_copy_assignment&) = delete;
  bool operator==(const no_copy_assignment&) const;
  bool operator!=(const no_copy_assignment&) const;

  friend std::ostream& operator<<(std::ostream&, const no_copy_assignment&);
};

template <> bool test_type_traits<no_copy_assignment>::is_copy_assignable()
{
  return false;
}

struct no_stream_output
{
  bool operator==(const no_stream_output&) const;
  bool operator!=(const no_stream_output&) const;
};

template <> bool test_type_traits<no_stream_output>::is_streamable()
{
  return false;
}

struct no_equality_test
{
  bool operator!=(const no_equality_test&) const;

  friend std::ostream& operator<<(std::ostream&, const no_equality_test&);
};

template <> bool test_type_traits<no_equality_test>::is_equality_comparable()
{
  return false;
}

struct no_inequality_test
{
  bool operator==(const no_inequality_test&) const;

  friend std::ostream& operator<<(std::ostream&, const no_inequality_test&);
};

template <> bool test_type_traits<no_inequality_test>::is_equality_comparable()
{
  return false;
}

struct bad_equality_test
{
  std::string operator==(const bad_equality_test&) const;
  bool operator!=(const bad_equality_test&) const;

  friend std::ostream& operator<<(std::ostream&, const bad_equality_test&);
};

template <> bool test_type_traits<bad_equality_test>::is_equality_comparable()
{
  return false;
}

struct bad_inequality_test
{
  bool operator==(const bad_inequality_test&) const;
  std::string operator!=(const bad_inequality_test&) const;

  friend std::ostream& operator<<(std::ostream&, const bad_inequality_test&);
};

template <> bool test_type_traits<bad_inequality_test>::is_equality_comparable()
{
  return false;
}

struct convertible_to_bool_equality_test
{
  int operator==(const convertible_to_bool_equality_test&) const;
  int operator!=(const convertible_to_bool_equality_test&) const;

  friend std::ostream& operator<<(std::ostream&,
                                  const convertible_to_bool_equality_test&);
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

typedef boost::mpl::list<flowable, int> flowable_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_flowable_true, T, flowable_types)
{
  BOOST_CHECK_EQUAL(core::is_flowable<T>::value, true);
}

typedef boost::mpl::list<no_default_constructor,
                         no_copy_constructor,
                         no_copy_assignment,
                         no_stream_output,
                         no_equality_test,
                         no_inequality_test,
                         bad_equality_test,
                         bad_inequality_test,
                         ref_based>
  not_flowable_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_flowable_false, T, not_flowable_types)
{
  BOOST_CHECK_EQUAL(std::is_default_constructible<T>::value,
                    test_type_traits<T>::is_default_constructible());
  BOOST_CHECK_EQUAL(std::is_copy_constructible<T>::value,
                    test_type_traits<T>::is_copy_constructible());
  BOOST_CHECK_EQUAL(std::is_copy_assignable<T>::value,
                    test_type_traits<T>::is_copy_assignable());
  BOOST_CHECK_EQUAL(internal::is_streamable<T>::value,
                    test_type_traits<T>::is_streamable());
  BOOST_CHECK_EQUAL(internal::is_equality_comparable<T>::value,
                    test_type_traits<T>::is_equality_comparable());

  BOOST_CHECK_EQUAL(core::is_flowable<T>::value, false);
}

BOOST_AUTO_TEST_CASE(test_is_flowable)
{
  BOOST_CHECK_EQUAL(core::is_flowable<int>::value, true);
  BOOST_CHECK_EQUAL(core::is_flowable<float>::value, true);
  BOOST_CHECK_EQUAL(core::is_flowable<std::string>::value, true);

  BOOST_CHECK_EQUAL(
    internal::is_equality_comparable<convertible_to_bool_equality_test>::value,
    true);
  BOOST_CHECK_EQUAL(core::is_flowable<convertible_to_bool_equality_test>::value,
                    true);

  BOOST_CHECK_EQUAL(core::is_flowable<const int*>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<const int&>::value, false);
  BOOST_CHECK_EQUAL(core::is_flowable<void>::value, false);
}

BOOST_AUTO_TEST_CASE(test_is_ref)
{
  BOOST_CHECK_EQUAL(core::is_ref<ref<int>>::value, true);
  BOOST_CHECK_EQUAL(core::is_ref<ref<int>&>::value, true);
  BOOST_CHECK_EQUAL(core::is_ref<const ref<int>&>::value, true);

  BOOST_CHECK_EQUAL(core::is_ref<val<int>>::value, true);
  BOOST_CHECK_EQUAL(core::is_ref<val<int>&>::value, true);
  BOOST_CHECK_EQUAL(core::is_ref<const val<int>&>::value, true);

  BOOST_CHECK_EQUAL(core::is_ref<var<int>>::value, true);
  BOOST_CHECK_EQUAL(core::is_ref<var<int>&>::value, true);
  BOOST_CHECK_EQUAL(core::is_ref<const var<int>&>::value, true);

  BOOST_CHECK_EQUAL(core::is_ref<int>::value, false);
}

BOOST_AUTO_TEST_CASE(test_is_function_of_time_type)
{
  BOOST_CHECK_EQUAL(core::is_function_of_time<int>::value, false);
  BOOST_CHECK_EQUAL(core::is_function_of_time<void>::value, false);

  const auto good_function_of_time = [](const Time&) { return Const<int>(); };

  BOOST_CHECK_EQUAL(
    core::is_function_of_time<decltype(good_function_of_time)>::value, true);

  const auto bad_function_of_time = [](const Time&) { return 0; };

  BOOST_CHECK_EQUAL(
    core::is_function_of_time<decltype(bad_function_of_time)>::value, false);

  BOOST_CHECK_EQUAL(core::is_function_of_time<ref<int>>::value, false);
}

BOOST_AUTO_TEST_CASE(test_is_transition_function)
{
  BOOST_CHECK_EQUAL(
    (core::is_transition_function<std::function<ref<int>(ref<int>)>,
                                  int>::value),
    true);

  BOOST_CHECK_EQUAL(
    (core::is_transition_function<
      std::function<std::function<ref<int>(const Time&)>(ref<int>)>,
      int>::value),
    true);

  BOOST_CHECK_EQUAL(
    (core::is_transition_function<
      std::function<std::function<val<int>(const Time&)>(ref<int>)>,
      int>::value),
    true);

  BOOST_CHECK_EQUAL(
    (core::is_transition_function<
      std::function<std::function<var<int>(const Time&)>(ref<int>)>,
      int>::value),
    true);

  BOOST_CHECK_EQUAL(
    (core::is_transition_function<ref<int> (*)(ref<int>), int>::value), true);

  BOOST_CHECK_EQUAL(
    (core::is_transition_function<std::function<ref<int>(ref<int>)>,
                                  char>::value),
    false);

  BOOST_CHECK_EQUAL((core::is_transition_function<int, int>::value), false);
}

BOOST_AUTO_TEST_CASE(test_patch_type)
{
  struct custom_patch_type
  {
  };

  struct custom_type
  {
    using patch_type = custom_patch_type;
  };

  BOOST_CHECK(
    (std::is_same<core::patch_type_t<custom_type>, custom_patch_type>::value));

  BOOST_CHECK(
    (std::is_same<core::patch_type_t<int>, core::generic_patch<int>>::value));
}

struct enable_if_test_helper
{
  template <typename... Args>
  static core::enable_if_all_t<bool, Args...> enabled_if_all(int)
  {
    return true;
  }

  template <typename... Args> static bool enabled_if_all(...)
  {
    return false;
  }

  template <typename... Args>
  static core::enable_if_some_t<bool, Args...> enabled_if_some(int)
  {
    return true;
  }

  template <typename... Args> static bool enabled_if_some(...)
  {
    return false;
  }

  template <typename... Args>
  static core::enable_if_none_t<bool, Args...> enabled_if_none(int)
  {
    return true;
  }

  template <typename... Args> static bool enabled_if_none(...)
  {
    return false;
  }
};

BOOST_AUTO_TEST_CASE(test_enable_if_all)
{
  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_all<std::true_type, std::true_type>(0)),
    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_all<std::true_type,
                                                           std::true_type,
                                                           std::true_type,
                                                           std::true_type>(0)),
                    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_all<std::true_type, std::false_type>(0)),
    false);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_all<std::false_type, std::true_type>(0)),
    false);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_all<std::true_type,
                                                           std::true_type,
                                                           std::false_type,
                                                           std::true_type>(0)),
                    false);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_all<std::true_type,
                                                           std::true_type,
                                                           std::true_type,
                                                           std::false_type>(0)),
                    false);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_all<std::false_type,
                                                           std::false_type,
                                                           std::false_type,
                                                           std::false_type>(0)),
                    false);
}

BOOST_AUTO_TEST_CASE(test_enable_if_some)
{
  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_some<std::true_type, std::true_type>(0)),
    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_some<std::true_type,
                                                            std::true_type,
                                                            std::true_type,
                                                            std::true_type>(0)),
                    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_some<std::true_type, std::false_type>(
      0)),
    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_some<std::false_type, std::true_type>(
      0)),
    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_some<std::true_type,
                                                            std::true_type,
                                                            std::false_type,
                                                            std::true_type>(0)),
                    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_some<std::true_type,
                                            std::true_type,
                                            std::true_type,
                                            std::false_type>(0)),
    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_some<std::false_type,
                                            std::false_type,
                                            std::false_type,
                                            std::false_type>(0)),
    false);
}

BOOST_AUTO_TEST_CASE(test_enable_if_none)
{
  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_none<std::true_type, std::true_type>(0)),
    false);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_none<std::true_type,
                                                            std::true_type,
                                                            std::true_type,
                                                            std::true_type>(0)),
                    false);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_none<std::true_type, std::false_type>(
      0)),
    false);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_none<std::false_type, std::true_type>(
      0)),
    false);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_none<std::true_type,
                                                            std::true_type,
                                                            std::false_type,
                                                            std::true_type>(0)),
                    false);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_none<std::true_type,
                                            std::true_type,
                                            std::true_type,
                                            std::false_type>(0)),
    false);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_none<std::false_type,
                                            std::false_type,
                                            std::false_type,
                                            std::false_type>(0)),
    true);
}

BOOST_AUTO_TEST_SUITE_END()
}
