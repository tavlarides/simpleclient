import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
  id: root

  required property var appController
  property alias topic: topicField.text
  property string statusColor: appController.connectionStatus === "Connected" ? "#2ecc71" : appController.connectionStatus === "Connecting" ? "#f39c12" : "#e74c3c"

  RowLayout {
    id: hostRow

    spacing: 12

    Label {
      text: "Host"
    }

    TextField {
      id: hostField

      text: root.appController.host

      onEditingFinished: root.appController.host = text
    }

    Label {
      text: "Port"
    }

    SpinBox {
      from: 1
      to: 65535
      value: root.appController.port

      onValueChanged: root.appController.port = value
    }

    Label {
      text: "Topic"
    }

    TextField {
      id: topicField

      text: root.appController.topic

      onEditingFinished: root.appController.topic = text
    }

    Button {
      text: "Connect"

      onClicked: root.appController.connectToBroker()
    }

    Button {
      text: "Disconnect"

      onClicked: root.appController.disconnectFromBroker()
    }
  }

  RowLayout {
    spacing: 12

    Label {
      text: "Status:"
    }

    Rectangle {
      color: root.statusColor
      height: 16
      radius: 8
      width: 16
    }

    Label {
      font.bold: true
      text: root.appController.connectionStatus
    }

    Label {
      text: "Messages: " + root.appController.messageCount
    }

    Label {
      elide: Text.ElideRight
      text: "Last payload: " + root.appController.lastPayload
    }
  }
}
