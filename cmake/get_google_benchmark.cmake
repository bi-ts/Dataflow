
cmake_minimum_required(VERSION 3.4)

set(_input_file "${CMAKE_CURRENT_LIST_DIR}/get_google_benchmark/CMakeLists.txt.in")

macro(get_google_benchmark google_benchmark_version google_benchmark_md5)
  set(import_file_path
      "${CMAKE_BINARY_DIR}/ext-projects/google_benchmark/src/google_benchmark-install/lib/cmake/benchmark/benchmarkConfig.cmake")

  if(NOT EXISTS "${import_file_path}")
    message(STATUS "Getting Google Benchmark...")

    set(EXT_PROJECTS_DIR "${CMAKE_BINARY_DIR}/ext-projects")
    set(GOOGLE_BENCHMARK_VERSION ${google_benchmark_version})
    set(GOOGLE_BENCHMARK_PACKAGE_MD5 ${google_benchmark_md5})

    configure_file(
      "${_input_file}"
      "${CMAKE_BINARY_DIR}/get-google_benchmark/CMakeLists.txt"
      @ONLY
    )

    unset(GOOGLE_BENCHMARK_VERSION)
    unset(GOOGLE_BENCHMARK_PACKAGE_MD5)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-google_benchmark"
    )

    execute_process(
      COMMAND ${CMAKE_COMMAND} --build .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-google_benchmark"
    )

  endif()

  include("${import_file_path}")

  if(NOT TARGET benchmark::benchmark)
    message(FATAL_ERROR "Failed to import target 'benchmark::benchmark'")
  endif()

  if(NOT TARGET benchmark::benchmark_main)
    message(FATAL_ERROR "Failed to import target 'benchmark::benchmark_main'")
  endif()
endmacro()

