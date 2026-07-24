/*
 * File: NotificationssView.qml
 * Description: Displays unread notifications for the authenticated user in batches.
 *              Integrates with QuickBookBridge backend to fetch notification maps
 *              and updates list dynamically upon clicking "View More".
 * Author: Trenser
 * Created: 22 July 2026
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: notificationsPage

    // PROPERTIES & BACKEND STATE
    property int batchSize: 5
    property bool hasMoreNotifications: true
    property var notificationsList: []

    Component.onCompleted: {
        loadNotificationsBatch()
    }

    // Fetch batch using QuickBookBridge::getUnreadNotifications(batchSize, remainingUnreadCount)
    function loadNotificationsBatch() {
        // Pass only batchSize integer (e.g., notificationsPage.batchSize)
        var fetchedNotifications = controller.getUnreadNotifications(notificationsPage.batchSize)

        if (fetchedNotifications && fetchedNotifications.length > 0) {
            var tempArray = notificationsPage.notificationsList.slice()
            for (var i = 0; i < fetchedNotifications.length; i++) {
                tempArray.push(fetchedNotifications[i])
            }
            notificationsPage.notificationsList = tempArray

            if (fetchedNotifications.length < notificationsPage.batchSize) {
                notificationsPage.hasMoreNotifications = false
            }
        } else {
            notificationsPage.hasMoreNotifications = false
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // 1. TOP HEADER BAR
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                RowLayout {
                    spacing: 10
                    Text {
                        text: "Notifications"
                        font.pixelSize: 22
                        font.bold: true
                        color: window.textDark
                    }

                    // Unread Counter Badge
                    Rectangle {
                        width: 28; height: 20; radius: 10
                        color: window.primaryRed
                        visible: notificationsPage.notificationsList.length > 0

                        Text {
                            anchors.centerIn: parent
                            text: notificationsPage.notificationsList.length
                            font.pixelSize: 11
                            font.bold: true
                            color: "#FFFFFF"
                        }
                    }
                }

                Text {
                    text: "Stay updated with bookings, theatre requests, and system alerts"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true } // Spacer

            CustomButton {
                text: "Clear Displayed"
                implicitWidth: 130; implicitHeight: 36
                btnColor: "#E5E5E5"
                textColor: window.textDark
                visible: notificationsPage.notificationsList.length > 0
                onClicked: {
                    notificationsPage.notificationsList = []
                }
            }
        }

        // 2. NOTIFICATIONS LIST
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: window.cardBg
            radius: 8
            border.color: window.borderColor

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 12

                ListView {
                    id: notificationListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 10
                    model: notificationsPage.notificationsList

                    delegate: Rectangle {
                        width: notificationListView.width
                        height: 64
                        color: "#FFF8F8"
                        radius: 8
                        border.color: "#FFCDD2"

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 15

                            // Unread Indicator Dot
                            Rectangle {
                                width: 8; height: 8; radius: 4
                                color: window.primaryRed
                            }

                            // Notification Content
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                Text {
                                    text: "Notification #" + (index + 1)
                                    font.bold: true
                                    font.pixelSize: 12
                                    color: window.textMuted
                                }

                                Text {
                                    // Accesses "message" key set in C++ QVariantMap
                                    text: modelData.message ? modelData.message : ""
                                    font.pixelSize: 13
                                    color: window.textDark
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                            }

                            // Dismiss Item Button
                            Button {
                                text: "✕"
                                flat: true
                                onClicked: {
                                    var tempArr = notificationsPage.notificationsList.slice()
                                    tempArr.splice(index, 1)
                                    notificationsPage.notificationsList = tempArr
                                }
                            }
                        }
                    }

                    // Empty State Graphic
                    Item {
                        anchors.centerIn: parent
                        visible: notificationsPage.notificationsList.length === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8

                            Text {
                                text: "🔔"
                                font.pixelSize: 36
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Text {
                                text: "NO UNREAD NOTIFICATIONS"
                                font.pixelSize: 14
                                font.bold: true
                                color: window.textMuted
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }

                // 3. PAGINATION / "VIEW MORE" FOOTER BAR
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 45
                    color: "#FAFAFA"
                    radius: 6
                    border.color: window.borderColor
                    visible: notificationsPage.hasMoreNotifications

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15

                        Text {
                            text: "More unread notifications available"
                            font.pixelSize: 12
                            color: window.textMuted
                        }

                        Item { Layout.fillWidth: true } // Spacer

                        CustomButton {
                            text: "View More"
                            implicitWidth: 100; implicitHeight: 30
                            btnColor: window.primaryRed
                            onClicked: {
                                notificationsPage.loadNotificationsBatch()
                            }
                        }
                    }
                }
            }
        }
    }
}