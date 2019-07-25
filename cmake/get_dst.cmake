
cmake_minimum_required(VERSION 3.4)

set(_input_file "${CMAKE_CURRENT_LIST_DIR}/get_dst/CMakeLists.txt.in")

macro(get_dst dst_version dst_md5)
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/ext-projects/dst/src/dst/cmake/dst.cmake")
    message(STATUS "Getting DST...")

    set(EXT_PROJECTS_DIR "${CMAKE_BINARY_DIR}/ext-projects")
    set(DST_VERSION ${dst_version})
    set(DST_PACKAGE_MD5 ${dst_md5})

    configure_file(
      "${_input_file}"
      "${CMAKE_BINARY_DIR}/get-dst/CMakeLists.txt"
      @ONLY
    )

    unset(DST_VERSION)
    unset(DST_PACKAGE_MD5)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-dst"
    )

    execute_process(
      COMMAND ${CMAKE_COMMAND} --build .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-dst"
    )

  endif()

  include("${CMAKE_BINARY_DIR}/ext-projects/dst/src/dst/cmake/dst.cmake")
endmacro()

