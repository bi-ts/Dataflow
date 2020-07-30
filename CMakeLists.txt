
cmake_minimum_required(VERSION 3.15)

project(dataflow++)

enable_testing()

include(GenerateExportHeader)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CONFIGURATION_TYPES Debug Release)

find_package(Boost 1.72.0 EXACT QUIET)

if(NOT Boost_FOUND)
  include(get_boost)
  get_boost(1.72.0 93cf8511f2e9b4456e5178cb07fc829d)
endif()

if(NOT TARGET dst)
  include(get_dst)
  get_dst(0.2.0 fa115e5bfcf5b3cdbac0201c4872ffb1)
endif()

include(get_google_benchmark)
get_google_benchmark(1.5.0 1af48846917b9730bee1848f6e38f563)

include(get_immer)
get_immer(0.6.2 4e3b26e398de68ec3bc67aece43b1db3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
  include/dataflow/prelude/core/internal/config.h
  include/dataflow/prelude/core/internal/node.h
  include/dataflow/prelude/core/internal/node_compound.h
  include/dataflow/prelude/core/internal/node_const.h
  include/dataflow/prelude/core/internal/node_if.h
  include/dataflow/prelude/core/internal/node_if_activator.h
  include/dataflow/prelude/core/internal/node_main.h
  include/dataflow/prelude/core/internal/node_n_ary.h
  include/dataflow/prelude/core/internal/node_patcher_n_ary.h
  include/dataflow/prelude/core/internal/node_previous.h
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
  include/dataflow/prelude/stateful/internal/transition.h
  include/dataflow/string.h
  include/dataflow/string.inl
  include/dataflow/tuple.h
  include/dataflow/tuple.inl
  include/dataflow/utility/std_future.h
  include/dataflow/utility/utility.h

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
  src/prelude/core/internal/tick_count.h
  src/prelude/core/internal/vd_handle.h
  src/prelude/core/internal/vd_handle.inl
  src/prelude/logical.cpp
  src/prelude/stateful/internal/transition.cpp
  src/string.cpp

)

target_link_libraries(${PROJECT_NAME}
  PUBLIC immer
  PRIVATE dst
)

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${Boost_INCLUDE_DIRS}
)

find_package(Qt5 COMPONENTS Core Gui Qml Quick Widgets REQUIRED)

add_library(dataflow-qt SHARED
  include/dataflow/qt.h
  include/dataflow/qt.inl
  include/dataflow/qt/conversion.h
  include/dataflow/qt/internal/context_builder.h
  include/dataflow/qt/internal/qobject_builder.h
  include/dataflow/qt/qml_data.h
  src/qt.cpp
  src/qt/conversion.cpp
  src/qt/internal/context_builder.cpp
  src/qt/internal/context_builder_impl.cpp
  src/qt/internal/context_builder_impl.h
  src/qt/internal/dynamic_qobject.cpp
  src/qt/internal/dynamic_qobject.h
  src/qt/internal/qobject_builder.cpp
  src/qt/internal/qobject_deleter.cpp
  src/qt/internal/qobject_deleter.h
  src/qt/internal/qobject_factory.cpp
  src/qt/internal/qobject_factory.h
  src/qt/internal/qobject_tracker.cpp
  src/qt/internal/qobject_tracker.h
  src/qt/internal/qvariant_list_model.cpp
  src/qt/internal/qvariant_list_model.h
  src/qt/qml_data.cpp
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
