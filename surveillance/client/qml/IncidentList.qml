import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
  id: root

  required property var incidents

  implicitHeight: mainLayout.implicitHeight
  implicitWidth: mainLayout.implicitWidth

  ColumnLayout {
    id: mainLayout

    anchors.fill: parent
    spacing: 10

    Label {
      font.bold: true
      font.pixelSize: 18
      text: qsTr("Live incidents")
    }

    ListView {
      Layout.fillHeight: true
      Layout.fillWidth: true
      clip: true
      model: root.incidents
      reuseItems: true
      spacing: 8

      delegate: Rectangle {
        id: incidentDelegate

        required property string cameraId
        required property string eventType
        required property double latitude
        required property double longitude
        required property string occurredAt
        required property string severity
        required property string zoneId

        color: severity === "critical" ? "#ffe8e8" : severity === "high" ? "#fff2d9" : "#edf5ff"
        implicitHeight: 92
        radius: 6
        width: ListView.view.width

        Column {
          anchors.fill: parent
          anchors.margins: 10
          spacing: 4

          Text {
            font.bold: true
            text: incidentDelegate.cameraId + " · " + incidentDelegate.eventType + " · " + incidentDelegate.severity.toUpperCase()
          }

          Text {
            text: incidentDelegate.zoneId + " · " + incidentDelegate.occurredAt
          }

          Text {
            text: qsTr("GIS: %1, %2").arg(incidentDelegate.latitude.toFixed(5)).arg(incidentDelegate.longitude.toFixed(5))
          }
        }
      }
    }
  }
}
