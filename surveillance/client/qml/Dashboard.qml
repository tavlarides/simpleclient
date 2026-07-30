import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    required property var controller

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Label {
            text: qsTr("Security Operations Monitor")
            font.pixelSize: 24
            font.bold: true
        }

        Label {
            text: controller.connectionStatus
            color: "#52616b"
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16

            IncidentList {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 620
                incidents: controller.incidentModel
            }

            Frame {
                Layout.fillHeight: true
                Layout.preferredWidth: 400

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Label {
                        text: qsTr("GIS overview")
                        font.bold: true
                        font.pixelSize: 18
                    }

                    Label {
                        Layout.fillWidth: true
                        text: qsTr("The live incident list includes latitude, longitude, and the resolved security zone. A map provider can replace this panel without changing the incident event model.")
                        wrapMode: Text.WordWrap
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#e8f0f2"
                        radius: 6

                        Label {
                            anchors.centerIn: parent
                            text: qsTr("Map layer placeholder")
                            color: "#52616b"
                        }
                    }
                }
            }
        }
    }
}
