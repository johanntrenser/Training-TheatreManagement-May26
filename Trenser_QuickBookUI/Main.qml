/*
 * File: Main.qml
 * Description: Defines the main application window for the Theatre Management System.
 *              Provides the global layout, theme palette, and navigation stack for
 *              the application. Integrates with the C++ ControllerAdapter backend
 *              through context property signals to handle authentication changes
 *              (e.g., redirecting to the Main Menu on logout). Establishes a light
 *              theme color palette that is referenced across child QML pages, ensuring
 *              consistent styling. Hosts the main StackView navigation system, starting
 *              with the MainMenu view.
 * Author: Trenser
 * Created: 22 July 2026
 */
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