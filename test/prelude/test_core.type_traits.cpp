
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
#include "../tools/test_tools.h"

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

using not_flowable_types = boost::mpl::list<no_default_constructor,
                                            no_copy_constructor,
                                            no_copy_assignment,
                                            no_stream_output,
                                            no_equality_test,
                                            no_inequality_test,
                                            bad_equality_test,
                                            bad_inequality_test,
                                            ref_based>;

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

  BOOST_CHECK_EQUAL(core::is_ref<void>::value, false);
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

  BOOST_CHECK_EQUAL(core::is_function_of_time<void>::value, false);
}

template <typename T>
using transition_function_types =
  boost::mpl::list<std::function<ref<T>(ref<T>)>,
                   std::function<val<T>(ref<T>)>,
                   std::function<var<T>(ref<T>)>,
                   ref<T> (*)(ref<T>),
                   val<T> (*)(ref<T>),
                   var<T> (*)(ref<T>),
                   std::function<std::function<ref<T>(const Time&)>(ref<T>)>,
                   std::function<std::function<val<T>(const Time&)>(ref<T>)>,
                   std::function<std::function<var<T>(const Time&)>(ref<T>)>,
                   ref<T> (*(*)(ref<T>))(const Time&),
                   val<T> (*(*)(ref<T>))(const Time&),
                   var<T> (*(*)(ref<T>))(const Time&)>;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_transition_function_int,
                              T,
                              transition_function_types<int>)
{
  BOOST_CHECK_EQUAL((core::is_transition_function<T, int>::value), true);

  BOOST_CHECK_EQUAL((core::is_transition_function<T, char>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, float>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, void>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, std::string>::value),
                    false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, const char*>::value),
                    false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_transition_function_float,
                              T,
                              transition_function_types<float>)
{
  BOOST_CHECK_EQUAL((core::is_transition_function<T, float>::value), true);

  BOOST_CHECK_EQUAL((core::is_transition_function<T, char>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, int>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, void>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, std::string>::value),
                    false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, const char*>::value),
                    false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_is_transition_function_string,
                              T,
                              transition_function_types<std::string>)
{
  BOOST_CHECK_EQUAL((core::is_transition_function<T, std::string>::value),
                    true);

  BOOST_CHECK_EQUAL((core::is_transition_function<T, char>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, float>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, void>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, int>::value), false);
  BOOST_CHECK_EQUAL((core::is_transition_function<T, const char*>::value),
                    false);
}

BOOST_AUTO_TEST_CASE(test_is_transition_function_false)
{
  BOOST_CHECK_EQUAL((core::is_transition_function<int, int>::value), false);

  BOOST_CHECK_EQUAL((core::is_transition_function<void, int>::value), false);
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

BOOST_AUTO_TEST_CASE(test_is_generic_patch)
{
  struct custom_patch_type
  {
  };

  BOOST_CHECK_EQUAL((core::is_generic_patch<custom_patch_type>::value), false);

  BOOST_CHECK_EQUAL((core::is_generic_patch<core::generic_patch<int>>::value),
                    true);

  BOOST_CHECK_EQUAL(
    (core::is_generic_patch<const core::generic_patch<int>>::value), true);

  BOOST_CHECK_EQUAL(
    (core::is_generic_patch<volatile core::generic_patch<int>>::value), true);

  BOOST_CHECK_EQUAL(
    (core::is_generic_patch<const volatile core::generic_patch<int>>::value),
    true);

  BOOST_CHECK_EQUAL((core::is_generic_patch<core::generic_patch<int>&>::value),
                    true);

  BOOST_CHECK_EQUAL(
    (core::is_generic_patch<const core::generic_patch<int>&>::value), true);

  BOOST_CHECK_EQUAL(
    (core::is_generic_patch<volatile core::generic_patch<int>&>::value), true);

  BOOST_CHECK_EQUAL(
    (core::is_generic_patch<const volatile core::generic_patch<int>&>::value),
    true);

  BOOST_CHECK_EQUAL(
    (std::is_same<core::is_generic_patch<core::generic_patch<int>>,
                  std::true_type>::value),
    true);

  BOOST_CHECK_EQUAL((std::is_same<core::is_generic_patch<custom_patch_type>,
                                  std::false_type>::value),
                    true);
}

BOOST_AUTO_TEST_CASE(test_is_trivially_patcheable)
{
  struct custom_patch_type
  {
  };

  struct custom_type
  {
    using patch_type = custom_patch_type;
  };

  BOOST_CHECK_EQUAL((core::is_trivially_patcheable<int>::value), true);

  BOOST_CHECK_EQUAL((core::is_trivially_patcheable<custom_type>::value), false);
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
  static core::enable_if_any_t<bool, Args...> enabled_if_any(int)
  {
    return true;
  }

  template <typename... Args> static bool enabled_if_any(...)
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

BOOST_AUTO_TEST_CASE(test_enable_if_any)
{
  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_any<std::true_type, std::true_type>(0)),
    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_any<std::true_type,
                                                           std::true_type,
                                                           std::true_type,
                                                           std::true_type>(0)),
                    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_any<std::true_type, std::false_type>(0)),
    true);

  BOOST_CHECK_EQUAL(
    (enable_if_test_helper::enabled_if_any<std::false_type, std::true_type>(0)),
    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_any<std::true_type,
                                                           std::true_type,
                                                           std::false_type,
                                                           std::true_type>(0)),
                    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_any<std::true_type,
                                                           std::true_type,
                                                           std::true_type,
                                                           std::false_type>(0)),
                    true);

  BOOST_CHECK_EQUAL((enable_if_test_helper::enabled_if_any<std::false_type,
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

class common_argument_data_type_helper
{
public:
  template <typename... Args> static bool enabled()
  {
    return enabled_<Args...>(0);
  }

private:
  template <typename Arg,
            typename... Args,
            typename T = core::common_argument_data_type_t<Arg, Args...>>
  static bool enabled_(int)
  {
    return true;
  }

  template <typename... Args> static bool enabled_(...)
  {
    return false;
  }
};

BOOST_AUTO_TEST_CASE(test_common_argument_data_type_t)
{
  BOOST_CHECK((std::is_same<core::common_argument_data_type_t<decltype(""),
                                                              ref<std::string>,
                                                              std::string>,
                            std::string>::value));

  BOOST_CHECK_EQUAL((common_argument_data_type_helper::
                       enabled<decltype(""), ref<std::string>, std::string>()),
                    true);

  BOOST_CHECK_EQUAL(
    (common_argument_data_type_helper::enabled<std::string, ref<int>>()),
    false);
}

class aggregate final : public core::aggregate_base
{
public:
  bool operator==(const aggregate&) const;
  bool operator!=(const aggregate&) const;

  friend std::ostream& operator<<(std::ostream&, const aggregate&);
};

BOOST_AUTO_TEST_CASE(test_is_aggregate_data_type)
{
  BOOST_CHECK_EQUAL(core::is_aggregate_data_type<int>::value, false);
  BOOST_CHECK_EQUAL(core::is_aggregate_data_type<flowable>::value, false);
  BOOST_CHECK_EQUAL(core::is_aggregate_data_type<aggregate>::value, true);
}

BOOST_AUTO_TEST_CASE(test_enable_if_aggregate_data_type)
{
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_aggregate_data_type,
                                     int>()),
    false);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_aggregate_data_type,
                                     flowable>()),
    false);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_aggregate_data_type,
                                     aggregate>()),
    true);
}

BOOST_AUTO_TEST_CASE(test_enable_if_aggregate_data_type_t)
{
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_aggregate_data_type_t,
                                     int>()),
    false);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_aggregate_data_type_t,
                                     flowable>()),
    false);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_aggregate_data_type_t,
                                     aggregate>()),
    true);
}

BOOST_AUTO_TEST_CASE(test_is_regular_data_type)
{
  BOOST_CHECK_EQUAL(core::is_regular_data_type<int>::value, true);
  BOOST_CHECK_EQUAL(core::is_regular_data_type<flowable>::value, true);
  BOOST_CHECK_EQUAL(core::is_regular_data_type<aggregate>::value, false);
}

BOOST_AUTO_TEST_CASE(test_enable_if_regular_data_type)
{
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_regular_data_type, int>()),
    true);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_regular_data_type,
                                     flowable>()),
    true);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_regular_data_type,
                                     aggregate>()),
    false);
}

BOOST_AUTO_TEST_CASE(test_enable_if_regular_data_type_t)
{
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_regular_data_type_t,
                                     int>()),
    true);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_regular_data_type_t,
                                     flowable>()),
    true);
  BOOST_CHECK_EQUAL(
    (test_tools::is_enabled_for_type<core::enable_if_regular_data_type_t,
                                     aggregate>()),
    false);
}

BOOST_AUTO_TEST_SUITE_END()
}
