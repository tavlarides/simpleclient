import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: root

  required property var controller

  implicitHeight: mainLayout.implicitHeight
  implicitWidth: mainLayout.implicitWidth

  ColumnLayout {
    id: mainLayout

    anchors.fill: parent
    anchors.margins: 20
    spacing: 16

    Label {
      font.bold: true
      font.pixelSize: 24
      text: qsTr("Security Operations Monitor")
    }

    Label {
      color: "#52616b"
      text: root.controller.connectionStatus
    }

    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      spacing: 16

      IncidentList {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredWidth: 620
        incidents: root.controller.incidentModel
      }

      Frame {
        Layout.fillHeight: true
        Layout.preferredWidth: 400

        ColumnLayout {
          anchors.fill: parent
          spacing: 12

          Label {
            font.bold: true
            font.pixelSize: 18
            text: qsTr("GIS overview")
          }

          Label {
            Layout.fillWidth: true
            text: qsTr("The live incident list includes latitude, longitude, and the resolved security zone. A map provider can replace this panel without changing the incident event model.")
            wrapMode: Text.WordWrap
          }

          Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#e8f0f2"
            radius: 6

            Label {
              anchors.centerIn: parent
              color: "#52616b"
              text: qsTr("Map layer placeholder")
            }
          }
        }
      }
    }
  }
}
