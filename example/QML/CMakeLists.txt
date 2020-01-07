
project(drag_n_drop)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Qml Quick Widgets REQUIRED)

add_executable(${PROJECT_NAME}
  main.cpp 
  main.qml 
  Input.h 
  Output.h 
  resources.qrc
)

target_link_libraries(${PROJECT_NAME}
  dataflow++
  Qt5::Core
  Qt5::Qml
  Qt5::Quick
  Qt5::Widgets
)

