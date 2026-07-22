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
        width: 400
        height: 480
        color: window.cardBg
        radius: 8
        border.color: window.borderColor

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 12

            Text {
                text: "Welcome Back"
                font.pixelSize: 24
                font.bold: true
                color: window.textDark
                Layout.alignment: Qt.AlignHCenter
            }

            // Error / Status Banner
            Text {
                id: statusText
                text: ""
                color: "#C5221F"
                font.pixelSize: 12
                visible: text !== ""
                wrapMode: Text.Wrap
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            // Email Input Field
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Text {
                    text: "Email Address"
                    color: window.textMuted
                    font.pixelSize: 12
                }

                TextField {
                    id: loginEmail
                    Layout.fillWidth: true
                    placeholderText: "enter your email"
                    selectByMouse: true
                    background: Rectangle {
                        border.color: window.borderColor
                        radius: 4
                    }
                }
            }

            // Password Input Field
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Text {
                    text: "Password"
                    color: window.textMuted
                    font.pixelSize: 12
                }

                TextField {
                    id: loginPassword
                    Layout.fillWidth: true
                    echoMode: TextField.Password
                    placeholderText: "••••••••"
                    selectByMouse: true
                    background: Rectangle {
                        border.color: window.borderColor
                        radius: 4
                    }
                    // Pressing Enter triggers the login button
                    onAccepted: loginButton.clicked()
                }
            }

            Item { Layout.fillHeight: true }

            // Submit Button
            Button {
                id: loginButton
                Layout.fillWidth: true
                Layout.preferredHeight: 45

                contentItem: Text {
                    text: "Login"
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.down ? Qt.darker(window.primaryRed, 1.2) : window.primaryRed
                    radius: 4
                }

                onClicked: {
                    var trimmedEmail = loginEmail.text.trim();
                    var trimmedPassword = loginPassword.text.trim();

                    // 1. Basic empty field check
                    if (trimmedEmail === "" || trimmedPassword === "") {
                        statusText.text = "Please enter both Email and Password!";
                        return;
                    }

                    // 2. Regex format check
                    if (!loginPage.isEmailValid(trimmedEmail)) {
                        statusText.text = "Please enter a valid email address!";
                        return;
                    }

                    statusText.text = "";

                    // 3. Direct call to global C++ "controller" context property
                    var loginStatus = controller.login(trimmedEmail, trimmedPassword);

                    if (loginStatus === EnumsAdapter.LoginStatus.USER_FOUND) {
                        statusText.text = "";
                        // Navigate to Dashboard
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
                flat: true
                text: "Back"
                onClicked: mainStack.pop()
            }
        }
    }
}