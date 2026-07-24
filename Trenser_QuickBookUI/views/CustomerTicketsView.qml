/*
 * File: CustomerTicketsView.qml
 * Description: Implements the customer ticket management interface in QML.
 *              Provides functionality to view active tickets and ticket history
 *              (past or cancelled). Integrates with the ControllerAdapter backend
 *              to load ticket data dynamically. Displays tickets in structured
 *              tables with reusable UI components for headers, data cells, and
 *              detail rows. Supports navigation tabs for switching between active
 *              tickets and history. Includes a ticket details dialog to show
 *              comprehensive pass information such as ticket ID, movie title,
 *              show time, booking ID, payment ID, amount, and status. Designed
 *              with reusable components, responsive layouts, and consistent
 *              styling aligned with the application’s UI palette.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customerTicketsPage

    component TableHeaderCell : Text {
        property string headerTitle: ""
        property int cellWidth: 100

        text: headerTitle
        width: cellWidth
        font.bold: true
        color: "#2C3E50"
        font.pixelSize: 13
    }

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

    property string activeTabName: "ACTIVE_TICKETS"

    Component.onCompleted: {
        controller.loadActiveTickets()
        controller.loadTicketHistory()
    }

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

        Row {
            spacing: 10
            Button {
                text: "1. View Active Tickets"
                font.bold: true
                palette.button: customerTicketsPage.activeTabName === "ACTIVE_TICKETS" ? "#F84464" : window.bgLight
                palette.buttonText: customerTicketsPage.activeTabName === "ACTIVE_TICKETS" ? "#FFFFFF" : window.textDark
                onClicked: {
                    customerTicketsPage.activeTabName = "ACTIVE_TICKETS"
                    controller.loadActiveTickets()
                }
            }
            Button {
                text: "2. View Ticket History"
                font.bold: true
                palette.button: customerTicketsPage.activeTabName === "TICKET_HISTORY" ? "#F84464" : window.bgLight
                palette.buttonText: customerTicketsPage.activeTabName === "TICKET_HISTORY" ? "#FFFFFF" : window.textDark
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

                        CustomButton {
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
                            textCustomColor: modelData.status==="ACTIVE" || modelData.status==="COMPLETED" ? "GREEN" : "RED"
                        }

                        CustomButton {
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

            CustomButton {
                text: "Close"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: ticketDetailsDialog.close()
            }
        }
    }
}