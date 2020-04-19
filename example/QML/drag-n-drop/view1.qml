
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
      target: view_context;
      property: "mousePressed";
      value: mouse.pressed;
    }

    Binding {
      target: view_context;
      property: "mousePos";
      value: Qt.point(mouse.mouseX, mouse.mouseY);
    }
  }

  Shortcut {
    sequence: "Ctrl+T"
    onActivated: {
      view_context.view = "view2.qml";
    }
  }

  Rectangle {
    radius: view_context.circleRadius
    x: view_context.circlePos.x - radius
    y: view_context.circlePos.y - radius
    width: 2 * radius
    height: 2 * radius
    color: "darkgray"
  }
}

