import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    required property var incidents

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            text: qsTr("Live incidents")
            font.bold: true
            font.pixelSize: 18
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: incidents
            spacing: 8
            reuseItems: true

            delegate: Rectangle {
                required property string cameraId
                required property string eventType
                required property string severity
                required property string zoneId
                required property string occurredAt
                required property double latitude
                required property double longitude

                width: ListView.view.width
                implicitHeight: 92
                radius: 6
                color: severity === "critical" ? "#ffe8e8" : severity === "high" ? "#fff2d9" : "#edf5ff"

                Column {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 4

                    Text {
                        text: cameraId + " · " + eventType + " · " + severity.toUpperCase()
                        font.bold: true
                    }

                    Text {
                        text: zoneId + " · " + occurredAt
                    }

                    Text {
                        text: qsTr("GIS: %1, %2").arg(latitude.toFixed(5)).arg(longitude.toFixed(5))
                    }
                }
            }
        }
    }
}
