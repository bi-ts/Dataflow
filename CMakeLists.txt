
cmake_minimum_required(VERSION 3.4)

project(dataflow++)

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

add_executable(dataflow_test src/main.cpp)

