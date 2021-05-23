
cmake_minimum_required(VERSION 3.15)

project(dataflow++)

enable_testing()

include(GenerateExportHeader)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(conan)

conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP KEEP_RPATHS CMAKE_TARGETS)

if(NOT TARGET dst)
  include(get_dst)
  get_dst(0.3.1 6e7d41e6d838c5ed0c8274e7ca3a3b28)
endif()

include(get_immer)
get_immer(0.6.2 4e3b26e398de68ec3bc67aece43b1db3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/WX)
else()
    add_compile_options(-Werror)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin)

add_library(${PROJECT_NAME} SHARED
  include/dataflow/behavior.h
  include/dataflow/behavior.inl
  include/dataflow/geometry.h
  include/dataflow/geometry.inl
  include/dataflow/introspect.h
  include/dataflow/introspect.inl
  include/dataflow/io.h
  include/dataflow/io.inl
  include/dataflow/list.h
  include/dataflow/list.inl
  include/dataflow/list/internal/list_diff.h
  include/dataflow/macro.h
  include/dataflow/macro.inl
  include/dataflow/macro/internal/cat.h
  include/dataflow/macro/internal/comma_if.h
  include/dataflow/macro/internal/expand.h
  include/dataflow/macro/internal/invoke.h
  include/dataflow/macro/internal/stringize.h
  include/dataflow/macro/internal/tuple_elem.h
  include/dataflow/macro/internal/tuple_for_each_i.h
  include/dataflow/macro/internal/tuple_size.h
  include/dataflow/maybe.h
  include/dataflow/maybe.inl
  include/dataflow/pair.h
  include/dataflow/pair.inl
  include/dataflow/prelude.h
  include/dataflow/prelude/arithmetic.h
  include/dataflow/prelude/arithmetic.inl
  include/dataflow/prelude/comparison.h
  include/dataflow/prelude/comparison.inl
  include/dataflow/prelude/conditional.h
  include/dataflow/prelude/conditional.inl
  include/dataflow/prelude/core.h
  include/dataflow/prelude/core.inl
  include/dataflow/prelude/core/dtime.h
  include/dataflow/prelude/core/engine_options.h
  include/dataflow/prelude/core/internal/config.h
  include/dataflow/prelude/core/internal/node.h
  include/dataflow/prelude/core/internal/node_compound.h
  include/dataflow/prelude/core/internal/node_const.h
  include/dataflow/prelude/core/internal/node_if.h
  include/dataflow/prelude/core/internal/node_if_activator.h
  include/dataflow/prelude/core/internal/node_main.h
  include/dataflow/prelude/core/internal/node_n_ary.h
  include/dataflow/prelude/core/internal/node_patcher_n_ary.h
  include/dataflow/prelude/core/internal/node_recursion.h
  include/dataflow/prelude/core/internal/node_recursion_activator.h
  include/dataflow/prelude/core/internal/node_selector.h
  include/dataflow/prelude/core/internal/node_selector_activator.h
  include/dataflow/prelude/core/internal/node_signal.h
  include/dataflow/prelude/core/internal/node_since.h
  include/dataflow/prelude/core/internal/node_since_activator.h
  include/dataflow/prelude/core/internal/node_snapshot.h
  include/dataflow/prelude/core/internal/node_snapshot_activator.h
  include/dataflow/prelude/core/internal/node_t.h
  include/dataflow/prelude/core/internal/node_var.h
  include/dataflow/prelude/core/internal/nodes_factory.h
  include/dataflow/prelude/core/internal/ref.h
  include/dataflow/prelude/core/internal/type_traits.h
  include/dataflow/prelude/logical.h
  include/dataflow/prelude/logical.inl
  include/dataflow/prelude/stateful.h
  include/dataflow/prelude/stateful.inl
  include/dataflow/prelude/stateful/internal/memo_value.h
  include/dataflow/prelude/stateful/internal/transition.h
  include/dataflow/random.h
  include/dataflow/random.inl
  include/dataflow/string.h
  include/dataflow/string.inl
  include/dataflow/tuple.h
  include/dataflow/tuple.inl
  include/dataflow/utility/std_future.h

  src/behavior.cpp
  src/geometry.cpp
  src/introspect.cpp
  src/io.cpp
  src/list.cpp
  src/pair.cpp
  src/prelude/arithmetic.cpp
  src/prelude/comparison.cpp
  src/prelude/conditional.cpp
  src/prelude/core.cpp
  src/prelude/core/dtime.cpp
  src/prelude/core/internal/config.h
  src/prelude/core/internal/converter.h
  src/prelude/core/internal/discrete_time.h
  src/prelude/core/internal/engine.cpp
  src/prelude/core/internal/engine.h
  src/prelude/core/internal/engine.inl
  src/prelude/core/internal/graph.h
  src/prelude/core/internal/node.cpp
  src/prelude/core/internal/node_compound.cpp
  src/prelude/core/internal/node_if_activator.cpp
  src/prelude/core/internal/node_recursion.cpp
  src/prelude/core/internal/node_recursion_activator.cpp
  src/prelude/core/internal/node_selector_activator.cpp
  src/prelude/core/internal/node_signal.cpp
  src/prelude/core/internal/node_since_activator.cpp
  src/prelude/core/internal/node_snapshot_activator.cpp
  src/prelude/core/internal/node_time.h
  src/prelude/core/internal/nodes_factory.cpp
  src/prelude/core/internal/pumpa.cpp
  src/prelude/core/internal/pumpa.h
  src/prelude/core/internal/ref.cpp
  src/prelude/core/internal/vd_handle.h
  src/prelude/core/internal/vd_handle.inl
  src/prelude/logical.cpp
  src/prelude/stateful/internal/transition.cpp
  src/random.cpp
  src/string.cpp

)

target_link_libraries(${PROJECT_NAME}
  PUBLIC immer
  PRIVATE dst CONAN_PKG::boost
)

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

find_package(Qt5 COMPONENTS Core Gui Qml Quick Widgets REQUIRED)

add_library(dataflow-qt SHARED
  include/dataflow-qt/qt.h
  include/dataflow-qt/qt.inl
  include/dataflow-qt/qt/conversion.h
  include/dataflow-qt/qt/internal/qobject_builder.h
  include/dataflow-qt/qt/qhandle.h
  include/dataflow-qt/qt/qvariant.h
  src/dataflow-qt/qt.cpp
  src/dataflow-qt/qt/conversion.cpp
  src/dataflow-qt/qt/internal/dynamic_qobject.cpp
  src/dataflow-qt/qt/internal/dynamic_qobject.h
  src/dataflow-qt/qt/internal/qobject_builder.cpp
  src/dataflow-qt/qt/internal/qobject_deleter.cpp
  src/dataflow-qt/qt/internal/qobject_deleter.h
  src/dataflow-qt/qt/internal/qobject_factory.cpp
  src/dataflow-qt/qt/internal/qobject_factory.h
  src/dataflow-qt/qt/internal/qobject_tracker.cpp
  src/dataflow-qt/qt/internal/qobject_tracker.h
  src/dataflow-qt/qt/internal/qvariant_list_model.cpp
  src/dataflow-qt/qt/internal/qvariant_list_model.h
  src/dataflow-qt/qt/qvariant.cpp
)

set_target_properties(dataflow-qt PROPERTIES AUTOMOC ON)

generate_export_header(dataflow-qt)

target_include_directories(dataflow-qt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(dataflow-qt
  PUBLIC dataflow++ Qt5::Qml
  PRIVATE Qt5::Core Qt5::CorePrivate Qt5::Gui
)

add_subdirectory(example)
add_subdirectory(test)
add_subdirectory(benchmark)
