cmake_minimum_required(VERSION 3.4)

project(dataflow_benchmark)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} dataflow++)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

