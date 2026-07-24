import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customerTicketsPage

    // 1. REUSABLE UI COMPONENTS
    // Component A: Table Header Cell
    component TableHeaderCell : Text {
        property string headerTitle: ""
        property int cellWidth: 100

        text: headerTitle
        width: cellWidth
        font.bold: true
        color: "#2C3E50"
        font.pixelSize: 13
    }

    // Component B: Table Data Text Cell
    component TableDataCell : Text {
        property string cellText: ""
        property int cellWidth: 100
        property color textCustomColor: "#000000"

        text: cellText
        width: cellWidth
        color: textCustomColor
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideRight
        font.pixelSize: 12
    }

    // Component C: Key-Value Label Line for Dialog Details
    component DetailRow : Row {
        property string labelName: ""
        property string valueText: ""
        property color valueColor: "#000000"
        property bool isValueBold: false

        spacing: 10
        width: parent.width

        Text {
            text: labelName
            font.bold: true
            width: 140
            color: "#555555"
        }
        Text {
            text: valueText
            color: valueColor
            font.bold: isValueBold
            width: parent.width - 150
            wrapMode: Text.Wrap
        }
    }

    // PAGE STATE PROPERTIES
    property string activeTabName: "ACTIVE_TICKETS" // "ACTIVE_TICKETS" or "TICKET_HISTORY"

    Component.onCompleted: {
        controller.loadActiveTickets()
        controller.loadTicketHistory()
    }

    // MAIN LAYOUT
    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "My Tickets Management"
            font.pixelSize: 20
            font.bold: true
            color: "#2C3E50"
        }

        // --- NAVIGATION TABS (Option 1 & Option 2) ---
        Row {
            spacing: 10

            Button {
                text: "1. View Active Tickets"
                onClicked: {
                    customerTicketsPage.activeTabName = "ACTIVE_TICKETS"
                    controller.loadActiveTickets()
                }
            }

            Button {
                text: "2. View Ticket History"
                onClicked: {
                    customerTicketsPage.activeTabName = "TICKET_HISTORY"
                    controller.loadTicketHistory()
                }
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#CCCCCC"
        }

        // VIEW 1: ACTIVE TICKETS (Option 1)
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: customerTicketsPage.activeTabName === "ACTIVE_TICKETS"

            Text {
                text: "Active Tickets"
                font.bold: true
                font.pixelSize: 15
            }

            // Table Header
            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 15

                    TableHeaderCell { headerTitle: "Ticket ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Payment ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Amount"; cellWidth: 90 }
                    TableHeaderCell { headerTitle: "Booking ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Status"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Action"; cellWidth: 100 }
                }
            }

            // Bound to controller.activeTickets
            ListView {
                id: activeTicketsListView
                width: parent.width
                height: parent.height - 80
                clip: true
                model: controller.activeTickets

                delegate: Rectangle {
                    width: activeTicketsListView.width
                    height: 40
                    color: index % 2 === 0 ? "#FFFFFF" : "#F8F9FA"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 15

                        TableDataCell { cellText: modelData.ticketId || ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData.paymentId || ""; cellWidth: 100 }
                        TableDataCell { cellText: "Rs. " + (modelData.amount || 0); cellWidth: 90 }
                        TableDataCell { cellText: modelData.bookingId || ""; cellWidth: 100 }
                        TableDataCell {
                            cellText: modelData.status || ""
                            cellWidth: 100
                            textCustomColor: "green"
                        }

                        Button {
                            text: "View Pass"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                ticketDetailsDialog.openTicketPass(
                                    modelData.ticketId,
                                    modelData.paymentId,
                                    modelData.bookingId,
                                    modelData.amount,
                                    modelData.status,
                                    modelData.movieTitle,
                                    modelData.showDateAndTime
                                )
                            }
                        }
                    }
                }
            }
        }

        // VIEW 2: TICKET HISTORY (Option 2)
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: customerTicketsPage.activeTabName === "TICKET_HISTORY"

            Text {
                text: "Ticket History (Past / Cancelled)"
                font.bold: true
                font.pixelSize: 15
            }

            // Table Header
            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 15

                    TableHeaderCell { headerTitle: "Ticket ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Payment ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Amount"; cellWidth: 90 }
                    TableHeaderCell { headerTitle: "Booking ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Status"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Action"; cellWidth: 100 }
                }
            }

            // Bound to controller.ticketHistory
            ListView {
                id: historyTicketsListView
                width: parent.width
                height: parent.height - 80
                clip: true
                model: controller.ticketHistory

                delegate: Rectangle {
                    width: historyTicketsListView.width
                    height: 40
                    color: index % 2 === 0 ? "#FFFFFF" : "#F8F9FA"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 15

                        TableDataCell { cellText: modelData.ticketId || ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData.paymentId || ""; cellWidth: 100 }
                        TableDataCell { cellText: "Rs. " + (modelData.amount || 0); cellWidth: 90 }
                        TableDataCell { cellText: modelData.bookingId || ""; cellWidth: 100 }
                        TableDataCell {
                            cellText: modelData.status || ""
                            cellWidth: 100
                            textCustomColor: modelData.status === "COMPLETED" ? "gray" : "red"
                        }

                        Button {
                            text: "Details"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                ticketDetailsDialog.openTicketPass(
                                    modelData.ticketId,
                                    modelData.paymentId,
                                    modelData.bookingId,
                                    modelData.amount,
                                    modelData.status,
                                    modelData.movieTitle,
                                    modelData.showDateAndTime
                                )
                            }
                        }
                    }
                }
            }
        }
    }

    // POPUP DIALOG: TICKET DETAILS PASS
    Dialog {
        id: ticketDetailsDialog
        title: "Ticket Pass Details"
        modal: true
        anchors.centerIn: parent
        width: 380
        height: 320

        function openTicketPass(
            selectedTicketId,
            selectedPaymentId,
            selectedBookingId,
            totalTicketAmount,
            currentTicketStatus,
            selectedMovieTitle,
            showDateTime
        ) {
            rowTicketId.valueText = selectedTicketId || "";
            rowMovieTitle.valueText = selectedMovieTitle || "";
            rowShowTime.valueText = showDateTime || "";
            rowPaymentId.valueText = selectedPaymentId || "";
            rowBookingId.valueText = selectedBookingId || "";
            rowAmount.valueText = "Rs. " + (totalTicketAmount || 0);
            rowStatus.valueText = currentTicketStatus || "";
            rowStatus.valueColor = currentTicketStatus === "ACTIVE" ? "green" : "gray";

            ticketDetailsDialog.open();
        }

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            DetailRow { id: rowTicketId; labelName: "Ticket ID:" }
            DetailRow { id: rowMovieTitle; labelName: "Movie Title:" }
            DetailRow { id: rowShowTime; labelName: "Show Time:" }
            DetailRow { id: rowBookingId; labelName: "Booking ID:" }
            DetailRow { id: rowPaymentId; labelName: "Payment ID:" }
            DetailRow { id: rowAmount; labelName: "Total Amount:" }
            DetailRow { id: rowStatus; labelName: "Ticket Status:"; isValueBold: true }

            Item { width: 1; height: 10 }

            Button {
                text: "Close"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: ticketDetailsDialog.close()
            }
        }
    }
}