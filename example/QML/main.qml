
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

    Binding {
      target: context;
      property: "mousePressed";
      value: mouse.pressed;
    }
    
    Binding {
      target: context;
      property: "mousePos";
      value: Qt.point(mouse.mouseX, mouse.mouseY);
    }
  }

  Rectangle {
    radius: context.circleRadius
    x: context.circlePos.x - radius
    y: context.circlePos.y - radius
    width: 2 * radius
    height: 2 * radius
    color: "darkgray"
  }
}

