/*
 * File: Register.qml
 * Description: Implements the user registration interface for the Theatre Management System.
 *              Provides a structured form that enables new users to register as either a Customer
 *              or a Theatre Owner. Features client-side validation for mandatory fields, email format
 *              verification using standard regular expressions, strict 10-digit phone number formatting,
 *              and password complexity verification requiring uppercase, lowercase, numeric, and special
 *              characters. Interacts with the ControllerAdapter backend via registerUser() and maps
 *              response codes from EnumsAdapter.ProcessStatus to provide clear error messages for duplicate
 *              emails or phone numbers. Includes an interactive success state upon successful account
 *              creation with seamless navigation to the login view.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Trenser_QuickBookUI

Item {
    id: registerPage

    property bool isRegistered: false
    property string errorMessage: ""

    // Email Regex Validation Helper
    function isValidEmail(email) {
        var emailPattern = /^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/;
        return emailPattern.test(email);
    }

    // 10-Digit Phone Number Validation Helper
    function isValidPhoneNumber(phone) {
        var phonePattern = /^[0-9]{10}$/;
        return phonePattern.test(phone);
    }

    // Password Complexity Validation Helper
    function isValidPassword(password) {
        if (password.length < 8) return false;

        var hasUpper = /[A-Z]/.test(password);
        var hasLower = /[a-z]/.test(password);
        var hasDigit = /[0-9]/.test(password);
        var hasSpecial = /[^A-Za-z0-9]/.test(password);

        return hasUpper && hasLower && hasDigit && hasSpecial;
    }

    Rectangle {
        anchors.centerIn: parent
        width: 440
        height: isRegistered ? 320 : 640
        color: window.cardBg
        radius: 8
        border.color: window.borderColor

        // ------------------ 1. REGISTRATION FORM VIEW ------------------
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 12
            visible: !registerPage.isRegistered

            Text {
                text: "Create Account"
                font.pixelSize: 24
                font.bold: true
                color: window.textDark
                Layout.alignment: Qt.AlignHCenter
            }

            // User Type Selector
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Register As"; color: window.textMuted; font.pixelSize: 12 }
                ComboBox {
                    id: userTypeCombo
                    Layout.fillWidth: true
                    model: ["Customer", "Theatre Owner"]
                }
            }

            // Full Name Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Full Name"; color: window.textMuted; font.pixelSize: 12 }
                TextField {
                    id: userNameInput
                    Layout.fillWidth: true
                    placeholderText: "John Doe"
                    selectByMouse: true
                    background: Rectangle { border.color: window.borderColor; radius: 4 }
                }
            }

            // Email Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Email Address"; color: window.textMuted; font.pixelSize: 12 }
                TextField {
                    id: emailInput
                    Layout.fillWidth: true
                    placeholderText: "name@example.com"
                    selectByMouse: true
                    background: Rectangle { border.color: window.borderColor; radius: 4 }
                }
            }

            // Phone Number Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Phone Number"; color: window.textMuted; font.pixelSize: 12 }
                TextField {
                    id: phoneInput
                    Layout.fillWidth: true
                    placeholderText: "Enter 10-digit phone number"
                    inputMethodHints: Qt.ImhDigitsOnly
                    selectByMouse: true
                    background: Rectangle { border.color: window.borderColor; radius: 4 }
                }
            }

            // Password Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Password"; color: window.textMuted; font.pixelSize: 12 }
                TextField {
                    id: passwordInput
                    Layout.fillWidth: true
                    echoMode: TextField.Password
                    placeholderText: "Min 8 chars (Upper, Lower, Digit, Special)"
                    selectByMouse: true
                    background: Rectangle { border.color: window.borderColor; radius: 4 }
                    onAccepted: registerButton.clicked()
                }
            }

            // Error Banner
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                color: "#FDE8E8"
                border.color: "#F8B4B4"
                radius: 4
                visible: registerPage.errorMessage !== ""

                Text {
                    anchors.centerIn: parent
                    anchors.margins: 5
                    width: parent.width - 10
                    text: registerPage.errorMessage
                    color: "#9B1C1C"
                    font.pixelSize: 11
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Item { Layout.fillHeight: true }

            // Submit Button
            Button {
                id: registerButton
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                contentItem: Text {
                    text: "Register"
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
                    var trimmedName = userNameInput.text.trim();
                    var trimmedEmail = emailInput.text.trim();
                    var trimmedPhone = phoneInput.text.trim();
                    var password = passwordInput.text;

                    // 1. Basic empty field validation
                    if (trimmedName === "" || trimmedEmail === "" || password === "" || trimmedPhone === "") {
                        registerPage.errorMessage = "Please fill in all fields.";
                        return;
                    }

                    // 2. Email format validation
                    if (!registerPage.isValidEmail(trimmedEmail)) {
                        registerPage.errorMessage = "Invalid email format (e.g., example@domain.com).";
                        return;
                    }

                    // 3. Phone format validation
                    if (!registerPage.isValidPhoneNumber(trimmedPhone)) {
                        registerPage.errorMessage = "Phone number must be exactly 10 digits.";
                        return;
                    }

                    // 4. Password complexity validation
                    if (!registerPage.isValidPassword(password)) {
                        registerPage.errorMessage = "Password must be at least 8 chars, contain upper, lower, digit & special char.";
                        return;
                    }

                    registerPage.errorMessage = "";
                    var resultStatus = 0;
                    if (userTypeCombo.currentText === "Customer") {
                        resultStatus = controller.registerUser(trimmedName, trimmedEmail, password, trimmedPhone, EnumsAdapter.UserType.CUSTOMER);
                    } else {
                        resultStatus = controller.registerUser(trimmedName, trimmedEmail, password, trimmedPhone, EnumsAdapter.UserType.THEATRE_OWNER);
                    }
                    if (resultStatus === EnumsAdapter.ProcessStatus.SUCCESS) {
                        registerPage.errorMessage = "";
                        registerPage.isRegistered = true;
                    } else if (resultStatus === EnumsAdapter.ProcessStatus.EMAIL_ALREADY_EXISTS) {
                        registerPage.errorMessage = "Email address is already in use.";
                    } else if (resultStatus === EnumsAdapter.ProcessStatus.PHONE_NUMBER_ALREADY_EXISTS) {
                        registerPage.errorMessage = "Phone number is already in use.";
                    } else {
                        registerPage.errorMessage = "Registration failed with status code: " + resultStatus;
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                flat: true
                text: "Back"
                onClicked: mainStack.pop()
            }
        }

        // ------------------ 2. SUCCESS CONFIRMATION VIEW ------------------
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 15
            visible: registerPage.isRegistered

            Item { Layout.fillHeight: true }

            Rectangle {
                width: 60; height: 60
                radius: 30
                color: "#E6F4EA"
                Layout.alignment: Qt.AlignHCenter
                Text {
                    text: "✓"
                    color: "#137333"
                    font.pixelSize: 32
                    anchors.centerIn: parent
                }
            }

            Text {
                text: "Registration Successful!"
                font.pixelSize: 20
                font.bold: true
                color: "#137333"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Your account has been created, please login to continue."
                font.pixelSize: 14
                color: window.textMuted
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            Item { Layout.preferredHeight: 10 }

            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                contentItem: Text {
                    text: "Go to Login"
                    color: "white"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle { color: window.primaryRed; radius: 4 }

                onClicked: mainStack.replace("Login.qml")
            }

            Item { Layout.fillHeight: true }
        }
    }
}