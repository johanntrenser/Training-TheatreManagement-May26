/*
 * File: Dashboard.qml
 * Description: Implements the main dashboard interface for the Theatre Management System.
 *              Provides a role-based navigation experience where menu options are dynamically
 *              loaded depending on whether the user is an Admin, Theatre Owner, or Customer.
 *              Includes a top navigation bar with search functionality and a role badge, a
 *              sidebar menu for module selection, and a central workspace area that dynamically
 *              loads QML views based on the active menu option. Supports modules such as
 *              User Management, Movie Management, Theatre Management, Show Management,
 *              Ticket Management, Booking Management, Notifications, Profile, Change Password,
 *              and System Logs. Integrates with the ControllerAdapter backend to fetch user
 *              profile details and ensures responsive layouts with consistent styling.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: dashboardPage
    property string userRole: ""
    property string activeView: "User Management"

    // Load menu options based on role
    Component.onCompleted: {
        var profile = controller.getProfile()
        profilePopUp.text=profile.name[0]
        dashboardPage.userRole = profile.role

        if (userRole === "Admin") {
            sidebarModel.append({"name": "User Management"});
            sidebarModel.append({"name": "Movie Management"});
            sidebarModel.append({"name": "Theatre Management"});
            sidebarModel.append({"name": "Show Management"});
            sidebarModel.append({"name": "Ticket Management"});
            sidebarModel.append({"name": "View Logs"});
            sidebarModel.append({"name": "View Profile"});
            sidebarModel.append({"name": "Change Password"});
            sidebarModel.append({"name": "View Notifications"});
            activeView = "User Management";
        } else if (userRole === "TheatreOwner") {
            sidebarModel.append({"name": "Theatre Management"});
            sidebarModel.append({"name": "Show Management"});
            sidebarModel.append({"name": "Booking Management"});
            sidebarModel.append({"name": "View Notifications"});
            sidebarModel.append({"name": "View Profile"});
            sidebarModel.append({"name": "Update Details"});
            sidebarModel.append({"name": "Change Password"});
            activeView = "Theatre Management";
        } else if (userRole === "Customer"){
            sidebarModel.append({"name": "Browse & Book"});
            sidebarModel.append({"name": "My Bookings"});
            sidebarModel.append({"name": "My Tickets"});
            sidebarModel.append({"name": "View Notifications"});
            sidebarModel.append({"name": "View Profile"});
            sidebarModel.append({"name": "Update Details"});
            sidebarModel.append({"name": "Change Password"});
            activeView = "Browse & Book";
        }
    }

    ListModel { id: sidebarModel }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // ------------------ TOP NAVBAR ------------------
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 65
            color: "#FFFFFF"
            border.color: window.borderColor
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 20

                Text {
                    text: "QUICK BOOK"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.primaryRed
                }

                // Search Bar Container
                Rectangle {
                    Layout.fillWidth: true
                    Layout.maximumWidth: 400
                    Layout.preferredHeight: 38
                    color: window.bgLight
                    border.color: window.borderColor
                    radius: 6

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        Text { text: "🔍"; color: window.textMuted }
                        TextField {
                            Layout.fillWidth: true
                            placeholderText: "Search Movies, Theatres, Users..."
                            background: Item {}
                        }
                    }
                }

                Item { Layout.fillWidth: true } // Spacer

                // Role badge in navbar
                RowLayout {
                    spacing: 10
                    Rectangle {
                        width: 34; height: 34; radius: 17
                        color: window.primaryRed
                        Text {
                            id: profilePopUp;
                            color: "white"
                            font.bold: true
                            anchors.centerIn: parent
                        }
                    }
                    ColumnLayout {
                        spacing: 0
                        Text { text: "Role"; font.pixelSize: 10; color: window.textMuted }
                        Text { text: dashboardPage.userRole; font.pixelSize: 13; font.bold: true; color: window.textDark }
                    }
                }
            }
        }

        // ------------------ MAIN WORKSPACE (SIDEBAR + CONTENT) ------------------
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // Sidebar Menu
            Rectangle {
                Layout.preferredWidth: 230
                Layout.fillHeight: true
                color: "#FFFFFF"
                border.color: window.borderColor

                ColumnLayout {
                    anchors.fill: parent
                    anchors.topMargin: 15
                    anchors.bottomMargin: 15
                    spacing: 5

                    ListView {
                        id: sidebarMenu
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: sidebarModel
                        delegate: ItemDelegate {
                            width: sidebarMenu.width
                            height: 45

                            background: Rectangle {
                                color: dashboardPage.activeView === model.name ? "#FFF0F2" : (hovered ? "#FAFAFA" : "transparent")
                                Rectangle {
                                    anchors.left: parent.left
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    width: 4
                                    color: window.primaryRed
                                    visible: dashboardPage.activeView === model.name
                                }
                            }

                            contentItem: Text {
                                text: model.name
                                color: dashboardPage.activeView === model.name ? window.primaryRed : window.textDark
                                font.bold: dashboardPage.activeView === model.name
                                font.pixelSize: 14
                                verticalAlignment: Text.AlignVCenter
                                anchors.leftMargin: 15
                            }

                            onClicked: dashboardPage.activeView = model.name
                        }
                    }

                    // Logout Button
                    Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        flat: true
                        contentItem: Text {
                            text: "Logout"
                            color: "#D32F2F"
                            font.bold: true
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: {
                            controller.logout()
                            mainStack.replace("MainMenu.qml")
                        }
                    }
                }
            }

            // Central Dynamic View Area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: window.bgLight

                // Dynamic view switcher based on activeView selection
                Loader {
                    anchors.fill: parent

                    source: {
                            if (dashboardPage.activeView === "User Management") {
                                return "UserManagement.qml"
                            }
                            else if (dashboardPage.activeView === "Movie Management") {
                                return "MovieManagement.qml"
                            }
                            else if (dashboardPage.activeView === "Browse & Book") {
                                return "CustomerBrowseAndBook.qml"
                            }
                            else if (dashboardPage.activeView === "Theatre Management") {
                                return "TheatreManagement.qml"
                            }
                            else if (dashboardPage.activeView === "Show Management") {
                                return "ShowManagement.qml"
                            }
                            else if (dashboardPage.activeView === "Booking Management") {
                                return "BookingManagement.qml"
                            }
                            else if (dashboardPage.activeView === "My Bookings") {
                                return "CustomerBookingsView.qml"
                            }
                            else if (dashboardPage.activeView === "Ticket Management") {
                                return "TicketManagement.qml"
                            }
                            else if (dashboardPage.activeView === "View Logs") {
                                return "SystemLogs.qml"
                            }
                            else if (dashboardPage.activeView === "View Profile") {
                                return "UserProfile.qml"
                            }
                            else if (dashboardPage.activeView === "Change Password") {
                                return "ChangePassword.qml"
                            }
                            else if (dashboardPage.activeView === "View Notifications") {
                                return "NotificationsView.qml"
                            }
                            else if (dashboardPage.activeView === "Ticket Management") {
                                return "TicketManagement.qml"
                            }
                            else if (dashboardPage.activeView === "My Tickets") {
                                return "CustomerTicketsView.qml"
                            }
                            else {
                                return ""
                            }
                        }

                    // Placeholder content when a view QML file isn't created yet
                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        visible: parent.status !== Loader.Ready

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 10
                            Text {
                                text: "🔨 " + dashboardPage.activeView
                                font.pixelSize: 22
                                font.bold: true
                                color: window.textDark
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: "This module page is ready to be implemented."
                                font.pixelSize: 14
                                color: window.textMuted
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: toast

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 24
        anchors.bottomMargin: 24

        width: 360
        height: 72
        radius: 14
        color: "#FFFFFF"
        border.color: "#E5E7EB"
        border.width: 1
        z: 999

        property alias message: toastText.text
        property bool isVisible: false

        opacity: isVisible ? 1.0 : 0.0

        transform: Translate {
            id: toastTranslation
            x: toast.isVisible ? 0 : 120
            y: toast.isVisible ? 0 : 40
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: "#0F172A"
            opacity: 0.12
            z: -1
            transform: Translate { y: 6 }
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: window.primaryRed
            opacity: 0.08
            z: -2
            transform: Translate { y: 12 }
        }

        MouseArea {
            id: toastHoverArea
            anchors.fill: parent
            hoverEnabled: true
            onEntered: hideTimer.stop()
            onExited: {
                if (toast.isVisible) {
                    hideTimer.restart()
                }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 14
            anchors.rightMargin: 14
            spacing: 12

            Rectangle {
                Layout.preferredWidth: 42
                Layout.preferredHeight: 42
                radius: 12
                color: "#FFF0F2"
                border.color: "#FFD6DC"
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: "🔔"
                    font.pixelSize: 18
                    scale: toast.isVisible ? 1.0 : 0.5

                    Behavior on scale {
                        NumberAnimation { duration: 300; easing.type: Easing.OutBack }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                RowLayout {
                    spacing: 6
                    Rectangle {
                        width: 6; height: 6; radius: 3
                        color: window.primaryRed
                    }
                    Text {
                        text: "NEW NOTIFICATION"
                        font.pixelSize: 10
                        font.bold: true
                        font.letterSpacing: 0.5
                        color: window.primaryRed
                    }
                }

                Text {
                    id: toastText
                    Layout.fillWidth: true
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: window.textDark
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    elide: Text.ElideRight
                    lineHeight: 1.15
                }
            }

            Rectangle {
                Layout.preferredWidth: 26
                Layout.preferredHeight: 26
                radius: 13
                color: closeMouseArea.containsMouse ? "#F3F4F6" : "transparent"

                Text {
                    anchors.centerIn: parent
                    text: "✕"
                    font.pixelSize: 11
                    font.bold: true
                    color: closeMouseArea.containsMouse ? window.textDark : window.textMuted
                }

                MouseArea {
                    id: closeMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: toast.hide()
                }
            }
        }

        function show(msg) {
            toast.message = msg
            toast.isVisible = true
            hideTimer.restart()
        }

        function hide() {
            toast.isVisible = false
            hideTimer.stop()
        }

        Behavior on opacity {
            NumberAnimation { duration: 220; easing.type: Easing.OutCubic }
        }

        Behavior on transform {
            NumberAnimation {
                target: toastTranslation
                properties: "x,y"
                duration: 350
                easing.type: toast.isVisible ? Easing.OutBack : Easing.InCubic
                easing.overshoot: 1.1
            }
        }

        Timer {
            id: hideTimer
            interval: 4500
            onTriggered: toast.hide()
        }
    }

    Connections {
        target: controller
        function onNotificationReceived(message) {
            toast.show(message)
        }
    }
}