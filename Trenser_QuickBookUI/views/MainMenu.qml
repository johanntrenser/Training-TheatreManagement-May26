/*
 * File: MainMenu.qml
 * Description: Implements the main menu interface for the Theatre Management System.
 *              Provides entry points for user authentication and registration, as well
 *              as an option to exit the application. The layout centers the content
 *              with a prominent title and subtitle, styled according to the global
 *              theme palette. Includes three primary actions:
 *              - Login: Navigates to the Login.qml page.
 *              - Register: Navigates to the Register.qml page for new user creation.
 *              - Exit: Closes the application.
 *              Designed with responsive layouts, consistent styling, and hover/press
 *              states for interactive buttons.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: mainMenuPage

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: 350

        // Logo / Title Header
        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 5

            Text {
                text: "QUICK BOOK"
                font.pixelSize: 36
                font.bold: true
                color: window.primaryRed
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "Theatre Management System"
                font.pixelSize: 14
                color: window.textMuted
                Layout.alignment: Qt.AlignHCenter
            }
        }

        // Spacer
        Item { Layout.preferredHeight: 20 }

        // Option 1: Login Button
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            contentItem: Text {
                text: "Login"
                color: "white"
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.down ? "#D33653" : (parent.hovered ? "#FA5D7C" : window.primaryRed)
                radius: 6
            }
            onClicked: mainStack.push("Login.qml")
        }

        // Option 2: Register Button
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            contentItem: Text {
                text: "Register as New User"
                color: window.primaryRed
                font.pixelSize: 16
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.down ? "#EAEAEA" : (parent.hovered ? "#F1F1F1" : "transparent")
                border.color: window.primaryRed
                border.width: 1
                radius: 6
            }
            onClicked: mainStack.push("Register.qml")
        }

        // Option 3: Exit Button
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            contentItem: Text {
                text: "Exit Application"
                color: window.textDark
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: parent.down ? "#DCDCDC" : (parent.hovered ? "#EFEFEF" : "transparent")
                radius: 6
            }
            onClicked: Qt.quit()
        }
    }
}