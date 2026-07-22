/*
 * File: CustomButton.qml
 * Description: Defines a reusable styled button component for the application.
 *              Extends the standard QtQuick Button with custom properties for
 *              background color, text color, and corner radius. Provides hover
 *              and press states with dynamic color adjustments, and ensures
 *              consistent sizing and typography across the UI. Designed to
 *              match the application's branding (default BookMyShow Red theme).
 * Author: Trenser
 * Created: 22 July 2026
 */
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