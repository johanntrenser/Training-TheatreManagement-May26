/*
 * File: UserProfile.qml
 * Description: Implements the user profile management interface in QML. Provides
 *              functionality to view, edit, and update user account details such
 *              as name, email, and phone number. Includes validation for email
 *              format and uniqueness, phone number uniqueness, and mandatory field
 *              checks. Integrates with the ControllerAdapter backend to persist
 *              changes. Displays feedback banners for success or error states,
 *              and supports toggling between view and edit modes. The UI includes
 *              an avatar with user initials, role badge, and a structured form
 *              layout with responsive design.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: userProfilePage

    function isEmailValid(emailString) {
        var emailRegex = /^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/
        return emailRegex.test(emailString)
    }

    function refreshProfile() {
        var profile = controller.getProfile()
        userProfilePage.userId = profile.id
        userProfilePage.userName = profile.name
        userProfilePage.userEmail = profile.email
        userProfilePage.userPhone = profile.phone
        if(profile.role==="Admin")
        {
            userProfilePage.userRole="Adminstrator"
        }
        else if(profile.role==="TheatreOwner")
        {
            userProfilePage.userRole="Theatre Owner"
        }
        else if(profile.role==="Customer")
        {
            userProfilePage.userRole = "Customer"
        }
    }

    Component.onCompleted: {
        refreshProfile()
    }

    // 1. DATA PROPERTIES
    property string userId: ""
    property string userName: ""
    property string userEmail: ""
    property string userPhone: ""
    property string userRole: ""

    // Edit Mode Toggle Flag
    property bool isEditing: false

    // Status message for feedback after saving
    property string statusMessage: ""
    property bool isError: false

    // Helper function to extract user initials for the avatar circle
    function getInitials(name) {
        var parts = name.split(" ")
        if (parts.length >= 2) {
            return (parts[0][0] + parts[1][0]).toUpperCase()
        } else if (parts.length === 1 && parts[0].length > 0) {
            return parts[0][0].toUpperCase()
        }
        return "U"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // TOP HEADER BAR
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "User Profile"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: userProfilePage.isEditing ? "Edit your personal account details below" : "View and manage your account details"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true } // Spacer

            // "Edit Profile" Button (Only visible in View Mode)
            CustomButton {
                text: "Edit Profile"
                implicitWidth: 120; implicitHeight: 38
                btnColor: window.primaryRed
                visible: !userProfilePage.isEditing
                onClicked: {
                    nameInput.text = userProfilePage.userName
                    emailInput.text = userProfilePage.userEmail
                    phoneInput.text = userProfilePage.userPhone
                    userProfilePage.statusMessage = ""
                    userProfilePage.isError = false
                    userProfilePage.isEditing = true
                }
            }
        }

        // MAIN PROFILE CARD
        Rectangle {
            Layout.preferredWidth: 650
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 520
            color: window.cardBg
            radius: 12
            border.color: window.borderColor

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 30
                spacing: 20

                // Status Message Banner (Shows error or success feedback)
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 36
                    radius: 6
                    color: userProfilePage.isError ? "#FDE8E8" : "#E6F4EA"
                    border.color: userProfilePage.isError ? "#F8B4B4" : "#A3E0B5"
                    visible: userProfilePage.statusMessage !== ""

                    Text {
                        anchors.centerIn: parent
                        text: userProfilePage.statusMessage
                        font.pixelSize: 12
                        font.bold: true
                        color: userProfilePage.isError ? "#C5221F" : "#137333"
                    }
                }

                // AVATAR AND USER TITLE ROW
                RowLayout {
                    spacing: 20
                    Layout.fillWidth: true

                    Rectangle {
                        width: 70; height: 70; radius: 35
                        color: window.primaryRed

                        Text {
                            anchors.centerIn: parent
                            text: userProfilePage.getInitials(userProfilePage.userName)
                            font.pixelSize: 28
                            font.bold: true
                            color: "#FFFFFF"
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        RowLayout {
                            spacing: 10
                            Text {
                                text: userProfilePage.userName
                                font.pixelSize: 20
                                font.bold: true
                                color: window.textDark
                            }

                            Rectangle {
                                width: 80; height: 22; radius: 11
                                color: "#E8F0FE"
                                Text {
                                    anchors.centerIn: parent
                                    text: userProfilePage.userRole
                                    font.pixelSize: 10
                                    font.bold: true
                                    color: "#1A73E8"
                                }
                            }
                        }

                        Text {
                            text: userProfilePage.userEmail
                            font.pixelSize: 13
                            color: window.textMuted
                        }
                    }
                }

                // Horizontal Divider
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: window.borderColor
                }

                // DETAILS FORM GRID
                GridLayout {
                    columns: 2
                    columnSpacing: 25
                    rowSpacing: 18
                    Layout.fillWidth: true

                    // 1. User ID (Always Read-Only)
                    ColumnLayout {
                        spacing: 6
                        Layout.fillWidth: true
                        Text { text: "User ID (Cannot be changed)"; font.bold: true; font.pixelSize: 12; color: window.textMuted }
                        Rectangle {
                            Layout.fillWidth: true; implicitHeight: 40
                            color: "#F2F4F7"; radius: 6; border.color: window.borderColor
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                text: userProfilePage.userId
                                font.pixelSize: 13
                                color: window.textMuted
                            }
                        }
                    }

                    // 2. Full Name
                    ColumnLayout {
                        spacing: 6
                        Layout.fillWidth: true
                        Text { text: "Full Name *"; font.bold: true; font.pixelSize: 12; color: window.textMuted }

                        Rectangle {
                            Layout.fillWidth: true; implicitHeight: 40
                            color: "#F9FAFB"; radius: 6; border.color: window.borderColor
                            visible: !userProfilePage.isEditing
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 12
                                text: userProfilePage.userName
                                font.pixelSize: 13
                                color: window.textDark
                            }
                        }

                        TextField {
                            id: nameInput
                            Layout.fillWidth: true
                            visible: userProfilePage.isEditing
                            placeholderText: "Enter full name"
                        }
                    }

                    // 3. Email Address
                    ColumnLayout {
                        spacing: 6
                        Layout.fillWidth: true
                        Text { text: "Email Address *"; font.bold: true; font.pixelSize: 12; color: window.textMuted }

                        Rectangle {
                            Layout.fillWidth: true; implicitHeight: 40
                            color: "#F9FAFB"; radius: 6; border.color: window.borderColor
                            visible: !userProfilePage.isEditing
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 12
                                text: userProfilePage.userEmail
                                font.pixelSize: 13
                                color: window.textDark
                            }
                        }

                        TextField {
                            id: emailInput
                            Layout.fillWidth: true
                            visible: userProfilePage.isEditing
                            placeholderText: "Enter email address"
                        }
                    }

                    // 4. Phone Number
                    ColumnLayout {
                        spacing: 6
                        Layout.fillWidth: true
                        Text { text: "Phone Number *"; font.bold: true; font.pixelSize: 12; color: window.textMuted }

                        Rectangle {
                            Layout.fillWidth: true; implicitHeight: 40
                            color: "#F9FAFB"; radius: 6; border.color: window.borderColor
                            visible: !userProfilePage.isEditing
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 12
                                text: userProfilePage.userPhone
                                font.pixelSize: 13
                                color: window.textDark
                            }
                        }

                        TextField {
                            id: phoneInput
                            Layout.fillWidth: true
                            visible: userProfilePage.isEditing
                            placeholderText: "e.g. 9876543210"
                            inputMethodHints: Qt.ImhDigitsOnly
                            maximumLength: 10
                            validator: RegularExpressionValidator { regularExpression: /^[0-9]{0,10}$/ }
                        }
                    }
                }

                Item { Layout.fillHeight: true } // Spacer

                // ACTION BUTTONS (Only visible when editing)
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    visible: userProfilePage.isEditing

                    Item { Layout.fillWidth: true }

                    CustomButton {
                        text: "Cancel"
                        implicitWidth: 100; implicitHeight: 38
                        btnColor: "#E5E5E5"
                        textColor: window.textDark
                        onClicked: {
                            userProfilePage.statusMessage = ""
                            userProfilePage.isError = false
                            userProfilePage.isEditing = false
                        }
                    }

                    CustomButton {
                        text: "Save Changes"
                        implicitWidth: 130; implicitHeight: 38
                        btnColor: window.primaryRed
                        onClicked: {
                            var trimmedName = nameInput.text.trim()
                            var trimmedEmail = emailInput.text.trim()
                            var trimmedPhone = phoneInput.text.trim()

                            // 1. Mandatory fields check
                            if (trimmedName === "" || trimmedEmail === "" || trimmedPhone === "") {
                                userProfilePage.statusMessage = "Please fill in all required fields!"
                                userProfilePage.isError = true
                                return
                            }

                            // 2. Email format check
                            if (!userProfilePage.isEmailValid(trimmedEmail)) {
                                userProfilePage.statusMessage = "Invalid email format!"
                                userProfilePage.isError = true
                                return
                            }

                            // 3. Email uniqueness check (if changed)
                            if (trimmedEmail !== userProfilePage.userEmail && !controller.isEmailUnique(trimmedEmail)) {
                                userProfilePage.statusMessage = "Email already exists!"
                                userProfilePage.isError = true
                                return
                            }

                            // 4. Phone uniqueness check (if changed)
                            if (trimmedPhone !== userProfilePage.userPhone && !controller.isPhoneNumberUnique(trimmedPhone)) {
                                userProfilePage.statusMessage = "Phone number already exists!"
                                userProfilePage.isError = true
                                return
                            }

                            // Save profile changes to backend C++
                            controller.updateProfile(trimmedName, trimmedEmail, trimmedPhone)

                            // Refresh profile data and update UI
                            userProfilePage.refreshProfile()
                            userProfilePage.statusMessage = "Profile updated successfully!"
                            userProfilePage.isError = false
                            userProfilePage.isEditing = false
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true } // Bottom Spacer
    }
}