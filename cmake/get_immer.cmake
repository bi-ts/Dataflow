
cmake_minimum_required(VERSION 3.4)

set(_input_file "${CMAKE_CURRENT_LIST_DIR}/get_immer/CMakeLists.txt.in")

macro(get_immer immer_version immer_md5)
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/ext-projects/immer/src/immer/CMakeLists.txt")
    message(STATUS "Getting immer...")

    set(EXT_PROJECTS_DIR "${CMAKE_BINARY_DIR}/ext-projects")
    set(IMMER_VERSION ${immer_version})
    set(IMMER_PACKAGE_MD5 ${immer_md5})

    configure_file(
      "${_input_file}" 
      "${CMAKE_BINARY_DIR}/get-immer/CMakeLists.txt"
      @ONLY
    )

    unset(IMMER_VERSION)
    unset(IMMER_PACKAGE_MD5)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-immer"
    )

    execute_process(
      COMMAND ${CMAKE_COMMAND} --build .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-immer"
    )

  endif()

  add_library(immer INTERFACE IMPORTED)

  target_include_directories(immer INTERFACE "${CMAKE_BINARY_DIR}/ext-projects/immer/src/immer/")

endmacro()

