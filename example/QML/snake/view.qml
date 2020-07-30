
import QtQuick.Controls 1.4 // ApplicationWindow
import QtQuick 2.14

ApplicationWindow {
  id: window
  width: (view_context.fieldSize.x + 2) * 10
  height: (view_context.fieldSize.y + 2) * 10
  visible: true
  
  Item {
    anchors.fill: parent
    focus: true
  
    Keys.onDownPressed: view_context.turnSouth();
    Keys.onLeftPressed: view_context.turnWest();
    Keys.onRightPressed: view_context.turnEast();
    Keys.onUpPressed: view_context.turnNorth();
  }

  Text {
    text: "The end"
    visible: view_context.gameOver
    anchors.centerIn: parent
  }

  Rectangle {
    visible: !view_context.gameOver
    width: view_context.fieldSize.x * 10
    height: view_context.fieldSize.y * 10
    border { color: "black"; width: 1 }

    anchors.centerIn: parent

    Repeater {
      model: view_context.snakeBody
      delegate: Rectangle {
        x: model.display.x * 10
        y: model.display.y * 10
        width: 9
        height: 9
        color: "lightgray"
        border { color: "black"; width: 1 }

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
}

