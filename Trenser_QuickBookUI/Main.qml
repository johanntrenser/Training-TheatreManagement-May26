import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 768
    title: "Theatre Management System"

    // -------------------------------------------------------------------------
    // LIGHT THEME COLOR PALETTE (Referenced across all child QML pages)
    // -------------------------------------------------------------------------
    property color bgLight: "#F8F9FA"
    property color cardBg: "#FFFFFF"
    property color primaryRed: "#F84464" // BookMyShow inspired red
    property color textDark: "#333333"
    property color textMuted: "#666666"
    property color borderColor: "#E5E5E5"

    background: Rectangle {
        color: window.bgLight
    }

    // -------------------------------------------------------------------------
    // LISTEN TO GLOBAL C++ "controller" CONTEXT PROPERTY SIGNALS
    // -------------------------------------------------------------------------
    Connections {
        target: controller

        // Return user to MainMenu automatically whenever logout occurs
        function onAuthenticationChanged() {
            if (!controller.isAuthenticated && mainStack.depth > 1) {
                mainStack.replace("views/MainMenu.qml")
            }
        }
    }

    // -------------------------------------------------------------------------
    // MAIN NAVIGATION STACK
    // -------------------------------------------------------------------------
    StackView {
        id: mainStack
        anchors.fill: parent
        // Starts on the Main Menu screen
        initialItem: "views/MainMenu.qml"
    }
}