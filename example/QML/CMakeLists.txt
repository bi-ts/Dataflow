
project(drag_n_drop)

add_executable(${PROJECT_NAME}
  main.cpp
  resources.qrc
  view1.qml
  view2.qml
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Qml
  Qt5::Quick
  Qt5::Widgets
  dataflow++
  dataflow-qt
)

