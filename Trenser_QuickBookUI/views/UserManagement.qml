    import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: userManagementPage

    function isEmailValid(emailString) {
            var emailRegex = /^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/
            return emailRegex.test(emailString)
        }

    // Filter property: "Active", or "Inactive"
    property string currentFilter: "Active"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // 1. TOP HEADER
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "User Management"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: "Manage customer, owner, and admin accounts"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true } // Flexible spacer

            // "Create User" Button - Replaces createUser() menu option
            CustomButton {
                text: "+ Create User"
                btnColor: window.primaryRed
                implicitWidth: 140
                onClicked: createUserDialog.open()
            }
        }

        // 2. FILTER TABS (Active / Inactive)
        RowLayout {
            spacing: 10

            Repeater {
                model: ["Active", "Inactive"]

                CustomButton {
                    text: modelData
                    implicitWidth: 90
                    implicitHeight: 32
                    btnColor: userManagementPage.currentFilter === modelData ? window.primaryRed : "#E5E5E5"
                    textColor: userManagementPage.currentFilter === modelData ? "#FFFFFF" : window.textDark
                    onClicked: userManagementPage.currentFilter = modelData
                }
            }
        }

        // 3. USER DATA TABLE HEADER
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#EEF0F2"
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                spacing: 10

                Text { text: "User ID"; font.bold: true; Layout.preferredWidth: 90; color: window.textDark }
                Text { text: "Name"; font.bold: true; Layout.preferredWidth: 150; color: window.textDark }
                Text { text: "Email"; font.bold: true; Layout.fillWidth: true; color: window.textDark }
                Text { text: "Role"; font.bold: true; Layout.preferredWidth: 120; color: window.textDark }
                Text { text: "Status"; font.bold: true; Layout.preferredWidth: 90; color: window.textDark }
                Text { text: "Action"; font.bold: true; Layout.preferredWidth: 100; color: window.textDark; horizontalAlignment: Text.AlignRight }
            }
        }

        // 4. USER DATA TABLE ROWS (ListView)
        ListView {
            id: usersList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: {
                if (userManagementPage.currentFilter === "Active") {
                    return controller.getActiveUsers()
                } else if (userManagementPage.currentFilter === "Inactive") {
                    return controller.getInactiveUsers()
                } else {
                    return controller.getActiveUsers()
                }
            }

            delegate: Rectangle {
                width: usersList.width
                height: 50
                color: "white"
                border.color: "#ccc"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15
                    spacing: 10

                    Text { text: modelData.id; Layout.preferredWidth: 90; color: window.textMuted }
                    Text { text: modelData.name; Layout.preferredWidth: 150; font.bold: true; color: window.textDark }
                    Text { text: modelData.email; Layout.fillWidth: true; color: window.textDark; elide: Text.ElideRight }
                    Text { text: modelData.type; Layout.preferredWidth: 120; color: window.textDark }
                    Text { text: modelData.status; Layout.preferredWidth: 90; color: window.textDark }


                    // Text { text: modelData.password;Layout.preferredWidth: 90; color: window.textDark }

                    Button {
                        text: modelData.status === "ACTIVE" ? "Deactivate" : "Reactivate"
                        Layout.preferredWidth: 95
                        onClicked: {
                            if (modelData.status === "ACTIVE") {
                                controller.deactivateUser(modelData.id)
                            } else {
                                controller.reactivateUser(modelData.id)
                            }
                            // Refresh list
                            if (userManagementPage.currentFilter === "Active") {
                                usersList.model = controller.getActiveUsers()
                            } else if (userManagementPage.currentFilter === "Inactive") {
                                usersList.model = controller.getInactiveUsers()
                            }
                        }
                    }
                }
            }
        }
    }

    // 5. CREATE USER DIALOG
    Dialog {
        id: createUserDialog
        title: "Create New User"
        anchors.centerIn: parent
        width: 400
        modal: true
        focus: true

        function resetForm() {
            newNameInput.text = ""
            newEmailInput.text = ""
            newPhoneInput.text = ""
            newPasswordInput.text = ""
            emailTextId.text = "Email Address"
            emailTextId.color = window.textMuted
            phoneNumberTextId.text = "Phone Number"
            phoneNumberTextId.color = window.textMuted
        }

        onOpened: resetForm()

        background: Rectangle {
            color: window.cardBg
            radius: 8
            border.color: window.borderColor
        }

        contentItem: ColumnLayout {
            spacing: 12

            Text {
                text: "Create User Account"
                font.pixelSize: 18
                font.bold: true
                color: window.textDark
            }

            // Role Selector
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "User Role"; font.pixelSize: 12; color: window.textMuted }
                ComboBox {
                    id: newRoleCombo
                    Layout.fillWidth: true
                    model: ["Customer", "Theatre Owner", "Admin"]
                }
            }

            // Name Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Full Name"; font.pixelSize: 12; color: window.textMuted }
                TextField {
                    id: newNameInput
                    Layout.fillWidth: true
                    placeholderText: "e.g. Jane Doe"
                }
            }

            // Email Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { id: emailTextId; text: "Email Address"; font.pixelSize: 12; color: window.textMuted }
                TextField {
                    id: newEmailInput
                    Layout.fillWidth: true
                    placeholderText: "e.g. jane@example.com"
                }
            }

            // Phone Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { id: phoneNumberTextId; text: "Phone Number"; font.pixelSize: 12; color: window.textMuted }
                TextField {
                    id: newPhoneInput
                    Layout.fillWidth: true
                    placeholderText: "e.g. 9876543210"
                    inputMethodHints: Qt.ImhDigitsOnly
                    maximumLength: 10
                    validator: RegularExpressionValidator { regularExpression: /^[0-9]{0,10}$/ }
                }
            }

            // Password Input
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Password"; font.pixelSize: 12; color: window.textMuted }
                TextField {
                    id: newPasswordInput
                    Layout.fillWidth: true
                    echoMode: TextField.Password
                    placeholderText: "••••••••"
                }
            }

            Item { Layout.preferredHeight: 10 }

            // Form Action Buttons
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                CustomButton {
                    text: "Cancel"
                    btnColor: "#E0E0E0"
                    textColor: window.textDark
                    Layout.fillWidth: true
                    onClicked: {
                        createUserDialog.resetForm()
                        createUserDialog.close()
                    }
                }

                CustomButton {
                    text: "Save User"
                    btnColor: window.primaryRed
                    textColor: "#FFFFFF"
                    Layout.fillWidth: true

                    onClicked: {
                        // Reset label styling first
                        emailTextId.text = "Email Address"
                        emailTextId.color = window.textMuted
                        phoneNumberTextId.text = "Phone Number"
                        phoneNumberTextId.color = window.textMuted

                        // 1. Mandatory fields check
                        if (newNameInput.text.trim() === "" || newEmailInput.text.trim() === "" || newPhoneInput.text.trim() === "" || newPasswordInput.text === "") {
                            emailTextId.text = "Please fill in all required fields!"
                            emailTextId.color = "#C5221F"
                            return
                        }

                        // 2. Email format validation
                        if (!userManagementPage.isEmailValid(newEmailInput.text.trim())) {
                            emailTextId.text = "Invalid email format!"
                            emailTextId.color = "#C5221F"
                            return
                        }

                        // 3. Email Uniqueness Check
                        if (!controller.isEmailUnique(newEmailInput.text.trim())) {
                            emailTextId.text = "Email already exists!"
                            emailTextId.color = "#C5221F"
                            return
                        }

                        // 4. Phone Uniqueness Check
                        if (!controller.isPhoneNumberUnique(newPhoneInput.text.trim())) {
                            phoneNumberTextId.text = "Phone number already exists!"
                            phoneNumberTextId.color = "#C5221F"
                            return
                        }

                        // 5. Successful Creation Pipeline
                        var roleMap = { "Customer": "customer", "Theatre Owner": "theatreOwner", "Admin": "admin" }
                        var roleKey = roleMap[newRoleCombo.currentText]

                        controller.createUser(
                            newNameInput.text.trim(),
                            newEmailInput.text.trim(),
                            newPasswordInput.text,
                            newPhoneInput.text.trim(),
                            roleKey
                        )

                        // Refresh list model
                        usersList.model = (userManagementPage.currentFilter === "Active")
                            ? controller.getActiveUsers()
                            : controller.getInactiveUsers()

                        // Cleanup and close
                        createUserDialog.resetForm()
                        createUserDialog.close()
                    }
                }
            }
        }
    }
}