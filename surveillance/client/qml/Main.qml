import QtQuick
import QtQuick.Controls
import Surveillance.Operator

ApplicationWindow {
    id: root

    visible: true
    width: 1100
    height: 720
    title: qsTr("Security Operations Monitor")

    OperatorController {
        id: controller
    }

    Dashboard {
        anchors.fill: parent
        controller: controller
    }
}
