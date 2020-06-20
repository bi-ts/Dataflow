
cmake_minimum_required(VERSION 3.4)

project(dataflow_test)

find_package(Boost 1.72.0 REQUIRED EXACT QUIET)

add_library(testing_utilities STATIC
  main.cpp
  tools/graph_invariant.cpp
  tools/graph_invariant.h
  tools/io_fixture.cpp
  tools/io_fixture.h
  tools/test_tools.h
)

target_include_directories(testing_utilities PUBLIC "${Boost_INCLUDE_DIRS}")

target_link_libraries(testing_utilities PUBLIC dataflow++)

function(dataflow_add_test_project name)
  set(_options)
  set(_one_value_args)
  set(_multi_value_args SOURCES DEFINITIONS LIBRARIES TARGET_PROPERTIES)

  cmake_parse_arguments(_test_project
    "${_options}" "${_one_value_args}" "${_multi_value_args}" ${ARGN}
  )

  if(NOT _test_project_SOURCES
     AND NOT _test_project_DEFINITIONS
     AND NOT _test_project_LIBRARIES
     AND NOT _test_project_TARGET_PROPERTIES)
    set(src "${ARGN}")
  endif()

  if(_test_project_SOURCES)
    set(src "${_test_project_SOURCES}")
  endif()

  if(NOT src)
    set(src "test_${name}.cpp")
  endif()

  add_executable(test_${name} ${src})

  target_compile_definitions(test_${name} PRIVATE ${_test_project_DEFINITIONS})

  target_link_libraries(test_${name} PRIVATE
    testing_utilities
    ${_test_project_LIBRARIES}
  )

  if(_test_project_TARGET_PROPERTIES)
    set_target_properties(test_${name}
      PROPERTIES ${_test_project_TARGET_PROPERTIES}
    )
  endif()

  add_test(NAME test_${name} COMMAND test_${name})
endfunction()

dataflow_add_test_project(test_tools)

dataflow_add_test_project(core
  prelude/core/test_dtime.cpp
  prelude/test_core.cpp
  prelude/test_core.patcher.cpp
  prelude/test_core.type_traits.cpp
)

dataflow_add_test_project(prelude
  test_prelude.cpp
  prelude/test_arithmetic.cpp
  prelude/test_comparison.cpp
  prelude/test_conditional.cpp
  prelude/test_logical.cpp
  prelude/test_stateful.cpp
)

dataflow_add_test_project(behavior)
dataflow_add_test_project(geometry)
dataflow_add_test_project(introspect)
dataflow_add_test_project(io)
dataflow_add_test_project(macro)

dataflow_add_test_project(macro-boost
  SOURCES test_macro.cpp
  DEFINITIONS -DDATAFLOW_CONFIG_HEADER_MACRO_BACKEND_BOOST
)

dataflow_add_test_project(maybe)
dataflow_add_test_project(pair)
dataflow_add_test_project(string)
dataflow_add_test_project(tuple)

dataflow_add_test_project(list
  test_list.cpp
  list/test_list_list.cpp
  list/test_list_patch.cpp
)

dataflow_add_test_project(regression)

dataflow_add_test_project(integration
  integration/conditional/test_conditional.cpp
  integration/state_machine/test_state_machine.cpp
)

dataflow_add_test_project(qt
  SOURCES test_qt.cpp
          qt/internal/example_qobject.cpp
          qt/internal/example_qobject.h
          qt/internal/lambda_connector.cpp
          qt/internal/lambda_connector.h
  LIBRARIES dataflow-qt
  TARGET_PROPERTIES AUTOMOC ON
)

dataflow_add_test_project(utility)

