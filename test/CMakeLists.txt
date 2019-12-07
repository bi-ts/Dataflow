
cmake_minimum_required(VERSION 3.4)

project(dataflow_test)

find_package(Boost 1.68.0 REQUIRED EXACT QUIET)

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

function(add_dataflow_test_project name)
  set(src "${ARGN}")

  if(NOT src)
    set(src "test_${name}.cpp")
  endif()

  add_executable(test_${name} ${src})

  target_link_libraries(test_${name} PRIVATE testing_utilities)

  add_test(NAME test_${name} COMMAND test_${name})
endfunction()

add_dataflow_test_project(test_tools)

add_dataflow_test_project(core
  prelude/core/test_dtime.cpp
  prelude/test_core.cpp
  prelude/test_core.patcher.cpp
  prelude/test_core.type_traits.cpp
)

add_dataflow_test_project(prelude
  test_prelude.cpp
  prelude/test_arithmetic.cpp
  prelude/test_comparison.cpp
  prelude/test_conditional.cpp
  prelude/test_logical.cpp
  prelude/test_stateful.cpp
)

add_dataflow_test_project(introspect)
add_dataflow_test_project(io)
add_dataflow_test_project(maybe)
add_dataflow_test_project(pair)
add_dataflow_test_project(string)
add_dataflow_test_project(tuple)

add_dataflow_test_project(list
  test_list.cpp
  list/test_list_patch.cpp
)

add_dataflow_test_project(regression)

add_dataflow_test_project(integration
  integration/conditional/test_conditional.cpp
  integration/state_machine/test_state_machine.cpp
)
