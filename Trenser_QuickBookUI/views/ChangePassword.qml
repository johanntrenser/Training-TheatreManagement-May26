/*
 * File: ChangePassword.qml
 * Description: Implements the change password interface in QML. Provides a secure
 *              form for updating account credentials with validation checks for
 *              minimum length, uppercase, lowercase, digit, and special character
 *              requirements. Includes real-time indicators for password strength
 *              and confirmation matching. Integrates with the ControllerAdapter
 *              backend to persist password changes. Displays feedback banners for
 *              success or error states, and offers clear/reset functionality to
 *              improve user experience. Designed with responsive layouts and
 *              consistent styling to align with the application’s UI theme.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: changePasswordPage

    // Form inputs
    property string currentPassword: ""
    property string newPassword: ""
    property string confirmPassword: ""

    // Validation Flags
    readonly property bool hasMinLength: newPassword.length >= 8
    readonly property bool hasUpper: /[A-Z]/.test(newPassword)
    readonly property bool hasLower: /[a-z]/.test(newPassword)
    readonly property bool hasDigit: /[0-9]/.test(newPassword)
    readonly property bool hasSpecial: /[^A-Za-z0-9]/.test(newPassword)

    readonly property bool isPasswordValid: hasMinLength && hasUpper && hasLower && hasDigit && hasSpecial
    readonly property bool passwordsMatch: newPassword === confirmPassword && confirmPassword.length > 0

    // Status Banner Properties
    property string statusMessage: ""
    property bool isError: false

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 25

        // ---------------------------------------------------------------------
        // 1. TOP HEADER BAR
        // ---------------------------------------------------------------------
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "Change Password"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: "Update your account security credentials"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true } // Spacer
        }

        // ---------------------------------------------------------------------
        // 2. CHANGE PASSWORD FORM CARD
        // ---------------------------------------------------------------------
        Rectangle {
            Layout.preferredWidth: 580
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 520
            color: window.cardBg
            radius: 12
            border.color: window.borderColor

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 30
                spacing: 18

                // Status Message Banner
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 38
                    radius: 6
                    visible: changePasswordPage.statusMessage !== ""
                    color: changePasswordPage.isError ? "#FDE8E8" : "#E6F4EA"
                    border.color: changePasswordPage.isError ? "#F8B4B4" : "#A3E0B5"

                    Text {
                        anchors.centerIn: parent
                        text: changePasswordPage.statusMessage
                        font.pixelSize: 12
                        font.bold: true
                        color: changePasswordPage.isError ? "#C5221F" : "#137333"
                    }
                }

                // Current Password Field
                ColumnLayout {
                    spacing: 6
                    Layout.fillWidth: true
                    Text { text: "Current Password *"; font.bold: true; font.pixelSize: 12; color: window.textMuted }
                    TextField {
                        id: currentPassInput
                        Layout.fillWidth: true
                        echoMode: TextInput.Password
                        placeholderText: "Enter current password"
                        onTextChanged: changePasswordPage.currentPassword = text
                    }
                }

                // New Password Field
                ColumnLayout {
                    spacing: 6
                    Layout.fillWidth: true
                    Text { text: "New Password *"; font.bold: true; font.pixelSize: 12; color: window.textMuted }
                    TextField {
                        id: newPassInput
                        Layout.fillWidth: true
                        echoMode: TextInput.Password
                        placeholderText: "Enter new password"
                        onTextChanged: changePasswordPage.newPassword = newPassInput.text
                    }
                }

                // Dynamic Password Criteria Indicators
                GridLayout {
                    columns: 2
                    rowSpacing: 4
                    columnSpacing: 20
                    Layout.fillWidth: true

                    RowLayout {
                        spacing: 6
                        Text { text: changePasswordPage.hasMinLength ? "✓" : "○"; color: changePasswordPage.hasMinLength ? "#2E7D32" : window.textMuted; font.bold: true }
                        Text { text: "At least 8 characters"; font.pixelSize: 11; color: changePasswordPage.hasMinLength ? window.textDark : window.textMuted }
                    }

                    RowLayout {
                        spacing: 6
                        Text { text: changePasswordPage.hasUpper ? "✓" : "○"; color: changePasswordPage.hasUpper ? "#2E7D32" : window.textMuted; font.bold: true }
                        Text { text: "At least 1 uppercase letter"; font.pixelSize: 11; color: changePasswordPage.hasUpper ? window.textDark : window.textMuted }
                    }

                    RowLayout {
                        spacing: 6
                        Text { text: changePasswordPage.hasLower ? "✓" : "○"; color: changePasswordPage.hasLower ? "#2E7D32" : window.textMuted; font.bold: true }
                        Text { text: "At least 1 lowercase letter"; font.pixelSize: 11; color: changePasswordPage.hasLower ? window.textDark : window.textMuted }
                    }

                    RowLayout {
                        spacing: 6
                        Text { text: changePasswordPage.hasDigit ? "✓" : "○"; color: changePasswordPage.hasDigit ? "#2E7D32" : window.textMuted; font.bold: true }
                        Text { text: "At least 1 digit"; font.pixelSize: 11; color: changePasswordPage.hasDigit ? window.textDark : window.textMuted }
                    }

                    RowLayout {
                        spacing: 6
                        Text { text: changePasswordPage.hasSpecial ? "✓" : "○"; color: changePasswordPage.hasSpecial ? "#2E7D32" : window.textMuted; font.bold: true }
                        Text { text: "At least 1 special character"; font.pixelSize: 11; color: changePasswordPage.hasSpecial ? window.textDark : window.textMuted }
                    }
                }

                // Confirm Password Field
                ColumnLayout {
                    spacing: 6
                    Layout.fillWidth: true
                    Text { text: "Confirm New Password *"; font.bold: true; font.pixelSize: 12; color: window.textMuted }
                    TextField {
                        id: confirmPassInput
                        Layout.fillWidth: true
                        echoMode: TextInput.Password
                        placeholderText: "Re-enter new password"
                        onTextChanged: changePasswordPage.confirmPassword = text
                    }
                    Text {
                        text: "Passwords do not match!"
                        font.pixelSize: 11
                        color: "#C5221F"
                        visible: changePasswordPage.confirmPassword.length > 0 && !changePasswordPage.passwordsMatch
                    }
                }

                Item { Layout.fillHeight: true } // Spacer

                // Action Buttons
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    CustomButton {
                        text: "Clear"
                        implicitWidth: 100; implicitHeight: 40
                        btnColor: "#E5E5E5"
                        textColor: window.textDark
                        onClicked: {
                            currentPassInput.text = ""
                            newPassInput.text = ""
                            confirmPassInput.text = ""
                            changePasswordPage.statusMessage = ""
                        }
                    }

                    Item { Layout.fillWidth: true } // Spacer

                    CustomButton {
                        text: "Update Password"
                        implicitWidth: 160; implicitHeight: 40
                        btnColor: (changePasswordPage.currentPassword !== "" && changePasswordPage.isPasswordValid && changePasswordPage.passwordsMatch) ? window.primaryRed : "#CCCCCC"

                        onClicked: {
                            if (changePasswordPage.currentPassword === "") {
                                changePasswordPage.statusMessage = "Please enter your current password."
                                changePasswordPage.isError = true
                                return
                            }

                            if (!changePasswordPage.isPasswordValid) {
                                changePasswordPage.statusMessage = "New password does not meet complexity requirements."
                                changePasswordPage.isError = true
                                return
                            }

                            if (!changePasswordPage.passwordsMatch) {
                                changePasswordPage.statusMessage = "New passwords do not match."
                                changePasswordPage.isError = true
                                return
                            }

                            // Call C++ controller and get boolean response
                            var success = controller.changePassword(changePasswordPage.currentPassword, changePasswordPage.newPassword)

                            if (success) {
                                changePasswordPage.statusMessage = "Password changed successfully."
                                changePasswordPage.isError = false
                                currentPassInput.text = ""
                                newPassInput.text = ""
                                confirmPassInput.text = ""
                            } else {
                                changePasswordPage.statusMessage = "Password update failed. Please check your current password."
                                changePasswordPage.isError = true
                            }
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true } // Bottom Spacer
    }
}