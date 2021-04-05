
import QtQuick.Controls 1.4 // ApplicationWindow
import QtQuick 2.14

ApplicationWindow {
  id: window
  readonly property int gameWindowScale: 12

  width: (view_context.fieldSize.x + 2) * gameWindowScale
  height: (view_context.fieldSize.y + 2) * gameWindowScale
  visible: true
  
  Item {
    anchors.fill: parent
    focus: true
  
    Keys.onDownPressed: view_context.turnSouth();
    Keys.onLeftPressed: view_context.turnWest();
    Keys.onRightPressed: view_context.turnEast();
    Keys.onUpPressed: view_context.turnNorth();
    Keys.onReturnPressed: view_context.togglePause();
  }

  Shortcut {
    sequence: "Ctrl+R"
    onActivated: view_context.restartGame();
  }

  Text {
    text: "The end"
    visible: view_context.gameOver
    anchors.centerIn: parent
  }

  Rectangle {
    visible: !view_context.gameOver
    width: view_context.fieldSize.x * gameWindowScale
    height: view_context.fieldSize.y * gameWindowScale
    border { color: "black"; width: 1 }

    anchors.centerIn: parent

    Repeater {
      model: view_context.snakeBody
      delegate: Rectangle {
        x: model.display.x * gameWindowScale
        y: model.display.y * gameWindowScale
        width: gameWindowScale - 1
        height: gameWindowScale - 1
        color: "lightgray"
        border { color: "black"; width: 1 }

        Rectangle {
          x: gameWindowScale / 5
          y: gameWindowScale / 5
          width: gameWindowScale - (2 * gameWindowScale / 5) - 1
          height: gameWindowScale - (2 * gameWindowScale / 5) - 1
          color: "black"
        }
      }
    }
  }
}

