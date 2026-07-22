import QtQuick
import QtQuick.Controls

Button {
    id: control

    // Custom properties with defaults
    property color btnColor: "#F84464" // Default BookMyShow Red
    property color textColor: "#FFFFFF"
    property int btnRadius: 4

    implicitHeight: 38
    implicitWidth: 120

    contentItem: Text {
        text: control.text
        font.pixelSize: 13
        font.bold: true
        color: control.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: control.down ? Qt.darker(control.btnColor, 1.2)
                            : (control.hovered ? Qt.lighter(control.btnColor, 1.1) : control.btnColor)
        radius: control.btnRadius
    }
}