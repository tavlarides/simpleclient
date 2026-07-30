import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  id: root

  required property var appController
  required property var messageModel

  property string statusColor: appController.connectionStatus === "Connected" ? "#2ecc71" : appController.connectionStatus === "Connecting" ? "#f39c12" : "#e74c3c"

  height: 680
  title: "QML MQTT Telemetry Demo"
  visible: true
  width: 900

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 20
    spacing: 16

    RowLayout {
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

    RowLayout {
      spacing: 12

      TextField {
        id: messageField

        Layout.fillWidth: true
        placeholderText: "Type a message"
      }

      Button {
        text: "Publish"

        onClicked: root.appController.publishMessage(messageField.text)
      }

      Button {
        text: "Subscribe"

        onClicked: root.appController.subscribeToTopic(topicField.text)
      }

      Button {
        text: "Clear"

        onClicked: root.appController.clearMessages()
      }
    }

    Frame {
      Layout.fillHeight: true
      Layout.fillWidth: true

      ListView {
        anchors.fill: parent
        clip: true
        model: root.messageModel
        spacing: 8

        delegate: Rectangle {
          id: messageDelegate
          required property int index
          required property string topic
          required property string payload
          required property string timestamp

          border.color: "#dcdcdc"
          color: index % 2 === 0 ? "#f7f7f7" : "#ffffff"
          height: 54
          radius: 4
          width: ListView.view.width

          Column {
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            Text {
              font.bold: true
              text: "[" + messageDelegate.timestamp + "] " + messageDelegate.topic
            }

            Text {
                text: messageDelegate.payload
            }
          }
        }
      }
    }

    Label {
      color: "#b00020"
      text: root.appController.errorMessage
      wrapMode: Text.WordWrap
    }
  }
}
