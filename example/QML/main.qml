
import QtQuick.Controls 1.4 // ApplicationWindow
import QtQuick 2.7          // MouseArea, Rectangle

ApplicationWindow {
  id: window
  width: 640
  height: 480
  visible: true

  MouseArea {
    id: mouse
    anchors.fill: parent
    hoverEnabled: true

    onPositionChanged: {
      input.mousePos = Qt.point(mouse.x, mouse.y)
    }

    onPressed: {
      input.leftMouseButtonPressed = true;
    }

    onReleased: {
      input.leftMouseButtonPressed = false;
    }
  }

  Rectangle {
    radius: width / 2
    x: output.circlePos.x - radius
    y: output.circlePos.y - radius
    width: 100
    height: 100
    color: "darkgray"
  }
}

