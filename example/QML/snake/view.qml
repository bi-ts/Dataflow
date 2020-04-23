
import QtQuick.Controls 1.4 // ApplicationWindow
import QtQuick 2.14

ApplicationWindow {
  id: window
  width: 310
  height: 310
  visible: true

  Repeater {
      model: [Qt.point(15, 15), Qt.point(15, 16), Qt.point(15, 17)]
      delegate: Rectangle {
          x: modelData.x * 10
          y: modelData.y * 10

          width: 9
          height: 9
          color: "lightgray"
          border.color: "black"
          border.width: 1

          Rectangle {
              x: 2
              y: 2
              width: 5
              height: 5
              color: "black"
          }
      }
  }
}

