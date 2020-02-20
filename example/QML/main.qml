
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

    Binding {
      target: context;
      property: "leftMouseButtonPressed";
      value: mouse.pressed;
    }
    
    Binding {
      target: context;
      property: "mousePos";
      value: Qt.point(mouse.mouseX, mouse.mouseY);
    }
  }

  Rectangle {
    radius: width / 2
    x: context.circlePos.x - radius
    y: context.circlePos.y - radius
    width: 100
    height: 100
    color: "darkgray"
  }
}

