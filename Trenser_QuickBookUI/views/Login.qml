/*
 * File: Login.qml
 * Description: Implements the login interface for the Theatre Management System.
 *              Provides input fields for email and password with validation checks
 *              for empty fields and proper email format. Integrates with the
 *              ControllerAdapter backend to authenticate users and handle login
 *              status codes (e.g., USER_FOUND, INVALID_PASSWORD, USER_NOT_FOUND,
 *              USER_ALREADY_LOGGED_IN). Displays error/status messages for user
 *              feedback and navigates to the Dashboard upon successful login.
 *              Includes a back button to return to the Main Menu. Designed with
 *              responsive layouts, consistent styling, and interactive button states.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Trenser_QuickBookUI

Item {
    id: loginPage

    // Regex Email Validation Helper
    function isEmailValid(emailString) {
        var emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
        return emailRegex.test(emailString);
    }

    Rectangle {
        anchors.centerIn: parent
        width: 420
        height: 520
        color: window.cardBg
        radius: 12
        border.color: window.borderColor
        border.width: 1

        // subtle drop shadow feel
        Rectangle {
            anchors.fill: parent
            anchors.topMargin: 4
            radius: 12
            color: "#00000008"
            z: -1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 36
            spacing: 18

            Text {
                text: "Welcome Back"
                font.pixelSize: 26
                font.bold: true
                color: window.textDark
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Sign in to continue"
                font.pixelSize: 13
                color: window.textMuted
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: -12
            }

            // Error / Status Banner
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: statusText.text !== "" ? 40 : 0
                visible: statusText.text !== ""
                color: "#FDECEA"
                radius: 6

                Text {
                    id: statusText
                    text: ""
                    anchors.centerIn: parent
                    anchors.margins: 8
                    width: parent.width - 16
                    color: "#C5221F"
                    font.pixelSize: 12
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            // Email Input Field
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text: "Email Address"
                    color: window.textMuted
                    font.pixelSize: 12
                    font.bold: true
                }

                TextField {
                    id: loginEmail
                    Layout.fillWidth: true
                    Layout.preferredHeight: 46
                    leftPadding: 14
                    rightPadding: 14
                    placeholderText: "Enter your email"
                    selectByMouse: true
                    font.pixelSize: 13
                    background: Rectangle {
                        border.color: loginEmail.activeFocus ? window.primaryRed : window.borderColor
                        border.width: loginEmail.activeFocus ? 2 : 1
                        radius: 6
                    }
                }
            }

            // Password Input Field
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Text {
                    text: "Password"
                    color: window.textMuted
                    font.pixelSize: 12
                    font.bold: true
                }

                TextField {
                    id: loginPassword
                    Layout.fillWidth: true
                    Layout.preferredHeight: 46
                    leftPadding: 14
                    rightPadding: 14
                    echoMode: TextField.Password
                    placeholderText: "••••••••"
                    selectByMouse: true
                    font.pixelSize: 13
                    background: Rectangle {
                        border.color: loginPassword.activeFocus ? window.primaryRed : window.borderColor
                        border.width: loginPassword.activeFocus ? 2 : 1
                        radius: 6
                    }
                    onAccepted: loginButton.clicked()
                }
            }

            // Submit Button
            Button {
                id: loginButton
                Layout.fillWidth: true
                Layout.preferredHeight: 48
                Layout.topMargin: 8

                contentItem: Text {
                    text: "Login"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.down ? Qt.darker(window.primaryRed, 1.2) : window.primaryRed
                    radius: 6
                }

                onClicked: {
                    var trimmedEmail = loginEmail.text.trim();
                    var trimmedPassword = loginPassword.text.trim();

                    if (trimmedEmail === "" || trimmedPassword === "") {
                        statusText.text = "Please enter both Email and Password!";
                        return;
                    }

                    if (!loginPage.isEmailValid(trimmedEmail)) {
                        statusText.text = "Please enter a valid email address!";
                        return;
                    }

                    statusText.text = "";

                    var loginStatus = controller.login(trimmedEmail, trimmedPassword);

                    if (loginStatus === EnumsAdapter.LoginStatus.USER_FOUND) {
                        statusText.text = "";
                        mainStack.replace("Dashboard.qml");
                    } else if (loginStatus === EnumsAdapter.LoginStatus.INVALID_PASSWORD) {
                        statusText.text = "Error: Invalid Password! Please try again.";
                    } else if (loginStatus === EnumsAdapter.LoginStatus.USER_NOT_FOUND) {
                        statusText.text = "Error: User email not found!";
                    } else if (loginStatus === EnumsAdapter.LoginStatus.USER_ALREADY_LOGGED_IN) {
                        statusText.text = "Error: User is already logged in elsewhere!";
                    } else {
                        statusText.text = "Error: Login failed with code " + loginStatus;
                    }
                }
            }

            // Back Button
            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                flat: true
                text: "Back"
                contentItem: Text {
                    text: "Back"
                    color: window.textMuted
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: mainStack.pop()
            }
        }
    }
}