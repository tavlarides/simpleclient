import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  id: root

  required property var appController
  required property var messageModel

  height: 680
  title: "QML MQTT Telemetry Demo"
  visible: true
  width: 900

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 12

    spacing: 12
    

    HostInput {
      id: hostInfo

      Layout.fillWidth: true
      appController: root.appController
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

        onClicked: root.appController.subscribeToTopic(hostInfo.topic)
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
          required property string payload
          required property string timestamp
          required property string topic

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
