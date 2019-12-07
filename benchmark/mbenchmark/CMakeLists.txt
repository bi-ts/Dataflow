project(dataflow_mbenchmark)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} dataflow++ benchmark::benchmark)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

