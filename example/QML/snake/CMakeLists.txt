
project(snake)

set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
  main.cpp
  resources.qrc
  view.qml
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Qml
  Qt5::Quick
  Qt5::Widgets
  dataflow++
  dataflow-qt
)

