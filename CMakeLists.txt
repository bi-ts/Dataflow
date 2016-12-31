
cmake_minimum_required(VERSION 3.4)

project(dataflow++)

enable_testing()

include(GenerateExportHeader)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Boost 1.61.0 EXACT QUIET)                                          
                                                                                
if(NOT Boost_FOUND)                                                             
  include(get_boost)                                                            
  get_boost(1.61.0 015ae4afa6f3e597232bfe1dab949ace)                            
endif()                                                                         
                                                                                
if(NOT TARGET dst)                                                              
  include(get_dst)                                                              
  get_dst(0.1.1 4e9a8215e1165bf8aaec002620d4db4d)                               
endif()                   

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")                              
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")        
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")        
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8")                                       
endif()

add_library(${PROJECT_NAME} SHARED 
  include/dataflow/introspect.h
  include/dataflow/introspect.inl
  include/dataflow/prelude.h
  include/dataflow/prelude.inl

  include/dataflow/internal/config.h
  include/dataflow/internal/node.h
  include/dataflow/internal/nodes_factory.h
  include/dataflow/internal/node_t.h
  include/dataflow/internal/nodes.h
  include/dataflow/internal/ref.h

  src/introspect.cpp
  src/prelude.cpp

  src/internal/converter.h
  src/internal/engine.cpp 
  src/internal/engine.h 
  src/internal/graph.h
  src/internal/node_main_activator.h
  src/internal/node_activator.cpp
  src/internal/nodes_factory.cpp
  src/internal/ref.cpp
  src/internal/tick_count.h
  src/internal/utility.h
)

target_link_libraries(${PROJECT_NAME} dst)

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${Boost_INCLUDE_DIRS} 
)

add_subdirectory(test)

