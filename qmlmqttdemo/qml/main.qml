import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  id: root

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

        text: appController.host

        onEditingFinished: appController.host = text
      }

      Label {
        text: "Port"
      }

      SpinBox {
        from: 1
        to: 65535
        value: appController.port

        onValueChanged: appController.port = value
      }

      Label {
        text: "Topic"
      }

      TextField {
        id: topicField

        text: appController.topic

        onEditingFinished: appController.topic = text
      }

      Button {
        text: "Connect"

        onClicked: appController.connectToBroker()
      }

      Button {
        text: "Disconnect"

        onClicked: appController.disconnectFromBroker()
      }
    }

    RowLayout {
      spacing: 12

      Label {
        text: "Status:"
      }

      Rectangle {
        color: statusColor
        height: 16
        radius: 8
        width: 16
      }

      Label {
        font.bold: true
        text: appController.connectionStatus
      }

      Label {
        text: "Messages: " + appController.messageCount
      }

      Label {
        elide: Text.ElideRight
        text: "Last payload: " + appController.lastPayload
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

        onClicked: appController.publishMessage(messageField.text)
      }

      Button {
        text: "Subscribe"

        onClicked: appController.subscribeToTopic(topicField.text)
      }

      Button {
        text: "Clear"

        onClicked: appController.clearMessages()
      }
    }

    Frame {
      Layout.fillHeight: true
      Layout.fillWidth: true

      ListView {
        anchors.fill: parent
        clip: true
        model: messageModel
        spacing: 8

        delegate: Rectangle {
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
              text: "[" + timestamp + "] " + topic
            }

            Text {
              text: payload
            }
          }
        }
      }
    }

    Label {
      color: "#b00020"
      text: appController.errorMessage
      wrapMode: Text.WordWrap
    }
  }
}
