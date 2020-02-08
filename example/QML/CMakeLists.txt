
project(drag_n_drop)

add_executable(${PROJECT_NAME}
  Context.h 
  main.cpp 
  main.qml 
  resources.qrc
)

target_link_libraries(${PROJECT_NAME}
  dataflow++
  Qt5::Core
  Qt5::Qml
  Qt5::Quick
  Qt5::Widgets
)

