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
                        onClicked: mainStack.replace("MainMenu.qml")
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
                                return "MovieCatalog.qml"
                            }
                            else if (dashboardPage.activeView === "Theatre Management") {
                                return "TheatreManagement.qml"
                            }
                            else if (dashboardPage.activeView === "Show Management") {
                                return "ShowManagement.qml"
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
}