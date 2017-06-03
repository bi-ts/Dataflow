
cmake_minimum_required(VERSION 3.4)

project(dataflow_test)

find_package(Boost 1.61.0 REQUIRED EXACT QUIET)

add_executable(${PROJECT_NAME}
  main.cpp

  tools/graph_invariant.cpp
  tools/graph_invariant.h
  tools/io_fixture.cpp
  tools/io_fixture.h

  test_comparison.cpp
  test_conditional.cpp
  test_core.cpp
  test_integration.cpp
  test_introspect.cpp
  test_io.cpp
  test_logical.cpp
  test_prelude.cpp
  test_string.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE "${Boost_INCLUDE_DIRS}")

target_link_libraries(${PROJECT_NAME} dataflow++)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

