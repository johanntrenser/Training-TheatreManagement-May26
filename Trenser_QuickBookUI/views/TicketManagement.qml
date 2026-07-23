/*
 * File: TicketManagement.qml
 * Description: Implements the admin ticket management interface for the
 *              Theatre Management System. Displays all customer tickets in a
 *              searchable, filterable table (by Ticket ID, customer name,
 *              payment ID, or booking ID; and by status). Loads data from
 *              ControllerAdapter's tickets property (populated by calling
 *              ControllerAdapter::loadTickets()), which provides ticketId,
 *              customerName, paymentId, amount, bookingId, and status for
 *              every ticket in the system. The tickets property emits
 *              ticketsChanged() so the table updates automatically whenever
 *              loadTickets() is called elsewhere (e.g. after a cancellation).
 * Author: Trenser
 * Created: 23 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: ticketManagementPage
    Component.onCompleted: controller.loadTickets()

    property string currentFilter: "All"
    property string searchQuery: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // 1. TOP HEADER BAR
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "Admin Ticket Management"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: "Overview of all customer tickets, payment IDs, booking references, and transaction statuses"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true }

        }

        // 2. SEARCH & STATUS FILTER TABS
        RowLayout {
            Layout.fillWidth: true
            spacing: 15

            Rectangle {
                Layout.preferredWidth: 340
                Layout.preferredHeight: 38
                color: "#FFFFFF"
                border.color: window.borderColor
                radius: 6

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    Text { text: "🔍"; color: window.textMuted }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "Search by Ticket ID, Name, Booking ID..."
                        background: Item {}
                        onTextChanged: ticketManagementPage.searchQuery = text.toLowerCase()
                    }
                }
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                spacing: 8
                // Matches Enums::TicketStatus (ACTIVE, CANCELLED, NOT_FOUND,
                // COMPLETED) — "Confirmed"/"Refunded" don't correspond to any
                // value getTicketStatusString() can return.
                Repeater {
                    model: ["All", "Active", "Cancelled", "Completed"]
                    CustomButton {
                        text: modelData
                        implicitWidth: 90
                        implicitHeight: 34
                        btnColor: ticketManagementPage.currentFilter === modelData ? window.primaryRed : "#E5E5E5"
                        textColor: ticketManagementPage.currentFilter === modelData ? "#FFFFFF" : window.textDark
                        onClicked: ticketManagementPage.currentFilter = modelData
                    }
                }
            }
        }

        // 3. TICKET TABLE DATA
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: window.cardBg
            radius: 8
            border.color: window.borderColor

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    color: "#FAFAFA"
                    border.color: window.borderColor

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 20
                        anchors.rightMargin: 20
                        spacing: 10

                        Text { text: "TICKET ID"; font.bold: true; font.pixelSize: 12; color: window.textMuted; Layout.preferredWidth: 110 }
                        Text { text: "CUSTOMER NAME"; font.bold: true; font.pixelSize: 12; color: window.textMuted; Layout.preferredWidth: 180 }
                        Text { text: "PAYMENT ID"; font.bold: true; font.pixelSize: 12; color: window.textMuted; Layout.preferredWidth: 120 }
                        Text { text: "AMOUNT"; font.bold: true; font.pixelSize: 12; color: window.textMuted; Layout.preferredWidth: 100 }
                        Text { text: "BOOKING ID"; font.bold: true; font.pixelSize: 12; color: window.textMuted; Layout.fillWidth: true }
                        Text { text: "STATUS"; font.bold: true; font.pixelSize: 12; color: window.textMuted; Layout.preferredWidth: 110 }
                    }
                }

                ListView {
                    id: ticketListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: controller.tickets

                    delegate: Rectangle {
                        width: ticketListView.width
                        height: visible ? 52 : 0
                        color: index % 2 === 0 ? "#FFFFFF" : "#F9FAFB"

                        visible: {
                            var matchesFilter = (ticketManagementPage.currentFilter === "All") ||
                                                (ticketManagementPage.currentFilter.toUpperCase() === model.status)
                            var query = ticketManagementPage.searchQuery
                            var matchesSearch = query === "" ||
                                                model.ticketId.toLowerCase().indexOf(query) !== -1 ||
                                                model.customerName.toLowerCase().indexOf(query) !== -1 ||
                                                model.paymentId.toLowerCase().indexOf(query) !== -1 ||
                                                model.bookingId.toLowerCase().indexOf(query) !== -1
                            return matchesFilter && matchesSearch
                        }

                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: 1
                            color: window.borderColor
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 10

                            Text {
                                text: model.ticketId
                                font.bold: true
                                font.pixelSize: 13
                                color: window.textDark
                                Layout.preferredWidth: 110
                            }

                            Text {
                                text: model.customerName
                                font.pixelSize: 13
                                font.bold: true
                                color: window.primaryRed
                                Layout.preferredWidth: 180
                                elide: Text.ElideRight
                            }

                            Text {
                                text: model.paymentId
                                font.pixelSize: 13
                                color: window.textMuted
                                Layout.preferredWidth: 120
                            }

                            Text {
                                text: "Rs. " + model.amount
                                font.bold: true
                                font.pixelSize: 13
                                color: "#2E7D32"
                                Layout.preferredWidth: 100
                            }

                            Text {
                                text: model.bookingId
                                font.pixelSize: 13
                                color: window.textDark
                                Layout.fillWidth: true
                            }

                            Item {
                                Layout.preferredWidth: 110
                                Rectangle {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 90; height: 22; radius: 11
                                    color: model.status === "ACTIVE" ? "#E6F4EA"
                                         : (model.status === "COMPLETED" ? "#E8F0FE" : "#FDE8E8")
                                    Text {
                                        anchors.centerIn: parent
                                        text: model.status
                                        font.pixelSize: 10
                                        font.bold: true
                                        color: model.status === "ACTIVE" ? "#137333"
                                             : (model.status === "COMPLETED" ? "#1A73E8" : "#C5221F")
                                    }
                                }
                            }
                        }
                    }

                    // Empty List Indicator
                    Item {
                        anchors.centerIn: parent
                        visible: ticketListView.count === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8
                            Text {
                                text: "🎟️"
                                font.pixelSize: 36
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: "NO TICKETS AVAILABLE"
                                font.pixelSize: 14
                                font.bold: true
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