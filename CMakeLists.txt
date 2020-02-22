
cmake_minimum_required(VERSION 3.4)

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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_CXX_FLAGS "-std=c++14 -stdlib=libc++")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
endif()

add_library(${PROJECT_NAME} SHARED
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

target_link_libraries(${PROJECT_NAME} dst immer)

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${Boost_INCLUDE_DIRS}
)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Qml Quick Widgets REQUIRED)

add_library(dataflow-qt SHARED
  include/dataflow/qt.h
  include/dataflow/qt.inl
  include/dataflow/qt/internal/context_builder.h
  src/qt.cpp
  src/qt/internal/context_builder.cpp
  src/qt/internal/context_builder_impl.cpp
  src/qt/internal/context_builder_impl.h
)

generate_export_header(dataflow-qt)

target_include_directories(dataflow-qt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(dataflow-qt
  dataflow++
  Qt5::Core
  Qt5::Qml
)

add_subdirectory(example)
add_subdirectory(test)
add_subdirectory(benchmark)
