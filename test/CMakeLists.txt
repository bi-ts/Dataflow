
cmake_minimum_required(VERSION 3.4)

project(dataflow_test)

find_package(Boost 1.61.0 REQUIRED EXACT QUIET)

add_executable(${PROJECT_NAME}
  main.cpp

  tools/graph_invariant.cpp
  tools/graph_invariant.h
  tools/io_fixture.cpp
  tools/io_fixture.h
  
  integration/test_state_machine.cpp
  
  prelude/test_arithmetic.cpp
  prelude/test_comparison.cpp
  prelude/test_conditional.cpp
  prelude/test_core.cpp
  prelude/test_core.type_traits.cpp
  prelude/test_logical.cpp
  test_integration.cpp
  test_introspect.cpp
  test_io.cpp
  test_pair.cpp
  test_prelude.cpp
  test_string.cpp
  test_tuple.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${Boost_INCLUDE_DIRS}")

target_link_libraries(${PROJECT_NAME} dataflow++)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

