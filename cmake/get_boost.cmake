
cmake_minimum_required(VERSION 3.4)

set(_input_file "${CMAKE_CURRENT_LIST_DIR}/get_boost/CMakeLists.txt.in")

macro(get_boost boost_version boost_md5)
  set(BOOST_ROOT "${CMAKE_BINARY_DIR}/ext-projects/boost/src/boost")
  set(Boost_NO_SYSTEM_PATHS ON)

  find_package(Boost ${boost_version} EXACT QUIET)

  if(NOT Boost_FOUND)
    message(STATUS "Getting Boost...")

    set(EXT_PROJECTS_DIR "${CMAKE_BINARY_DIR}/ext-projects")
    set(BOOST_VERSION ${boost_version})
    set(BOOST_PACKAGE_MD5 ${boost_md5})

    string(REPLACE "." "_" _version_underscore ${BOOST_VERSION})

    set(BOOST_PACKAGE_NAME boost_${_version_underscore}.zip)

    configure_file(
      "${_input_file}" 
      "${CMAKE_BINARY_DIR}/get-boost/CMakeLists.txt"
      @ONLY
    )

    unset(EXT_PROJECTS_DIR)
    unset(BOOST_VERSION)
    unset(BOOST_PACKAGE_MD5)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-boost"
    )

    execute_process(
      COMMAND ${CMAKE_COMMAND} --build .
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/get-boost"
    )

  endif()

  find_package(Boost ${boost_version} REQUIRED EXACT QUIET)

endmacro()

