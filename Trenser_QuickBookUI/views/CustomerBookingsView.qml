/*
 * File: CustomerBookingView.qml
 * Description: Implements the customer bookings, payments, and refunds
 *              interface for the Theatre Management System. Displays the
 *              customer's bookings, payments, and refund history in separate
 *              tabbed table views.
 * Author: Trenser
 * Created: 23 July 2026
 */
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customerBookingsPage

    // Reusable Table Header Component
    component TableHeaderCell : Text {
        property string headerTitle: ""
        property int cellWidth: 100

        text: headerTitle
        width: cellWidth
        font.bold: true
        color: "#2C3E50"
        font.pixelSize: 13
    }

    // Reusable Table Cell Component
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

    // Reusable Dialog Detail Row
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

    property string activeTabName: "BOOKINGS"

    Component.onCompleted: {
        controller.loadBookings()
        controller.loadPayments()
        controller.loadRefunds()
    }

    function processCancelBooking(targetBookingId) {
        var response = controller.cancelBooking(targetBookingId)
        cancelResultDialog.showResult(response.success, response.message)
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "My Bookings & Account History"
            font.pixelSize: 20
            font.bold: true
            color: "#2C3E50"
        }

        // Tab Navigation Buttons
        Row {
            spacing: 10

            Button {
                text: "1. View All Bookings"
                onClicked: {
                    customerBookingsPage.activeTabName = "BOOKINGS"
                    controller.loadBookings()
                }
            }
            Button {
                text: "2. View All Payments"
                onClicked: {
                    customerBookingsPage.activeTabName = "PAYMENTS"
                    controller.loadPayments()
                }
            }
            Button {
                text: "3. View All Refunds"
                onClicked: {
                    customerBookingsPage.activeTabName = "REFUNDS"
                    controller.loadRefunds()
                }
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#CCCCCC"
        }

        // TAB 1: Bookings Section
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: customerBookingsPage.activeTabName === "BOOKINGS"

            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 10

                    TableHeaderCell { headerTitle: "Booking ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Movie Name"; cellWidth: 150 }
                    TableHeaderCell { headerTitle: "Date & Time"; cellWidth: 140 }
                    TableHeaderCell { headerTitle: "No. Seats"; cellWidth: 80 }
                    TableHeaderCell { headerTitle: "Status"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Actions"; cellWidth: 180 }
                }
            }

            ListView {
                id: bookingListView
                width: parent.width
                height: parent.height - 45
                clip: true
                model: controller.bookings

                delegate: Rectangle {
                    width: bookingListView.width
                    height: 50
                    color: index % 2 === 0 ? "#FFFFFF" : "#F8F9FA"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 10

                        TableDataCell { cellText: modelData && modelData.bookingId ? modelData.bookingId : ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData && modelData.movieName ? modelData.movieName : ""; cellWidth: 150 }
                        TableDataCell { cellText: modelData && modelData.dateTime ? modelData.dateTime : ""; cellWidth: 140 }
                        TableDataCell { cellText: modelData && modelData.seatsCount !== undefined ? String(modelData.seatsCount) : "0"; cellWidth: 80 }
                        TableDataCell {
                            cellText: modelData && modelData.status ? modelData.status : ""
                            cellWidth: 100
                            textCustomColor: (modelData && modelData.status === "CONFIRMED") ? "green" : "red"
                        }

                        Row {
                            spacing: 5
                            anchors.verticalCenter: parent.verticalCenter

                            Button {
                                text: "Details"
                                onClicked: {
                                    bookingDetailsDialog.showBookingInformation(
                                        modelData.bookingId ? modelData.bookingId : "",
                                        modelData.movieName ? modelData.movieName : "",
                                        modelData.theaterName ? modelData.theaterName : "",
                                        modelData.dateTime ? modelData.dateTime : "",
                                        modelData.seatsCount !== undefined ? String(modelData.seatsCount) : "0",
                                        modelData.status ? modelData.status : ""
                                    )
                                }
                            }

                            Button {
                                text: "Cancel"
                                enabled: modelData && modelData.status === "CONFIRMED"
                                onClicked: {
                                    cancelConfirmationDialog.targetBookingId = modelData.bookingId;
                                    cancelConfirmationDialog.open();
                                }
                            }
                        }
                    }
                }
            }
        }

        // TAB 2: Payments Section
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: customerBookingsPage.activeTabName === "PAYMENTS"

            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 15

                    TableHeaderCell { headerTitle: "Payment ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Booking ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Amount"; cellWidth: 80 }
                    TableHeaderCell { headerTitle: "Method"; cellWidth: 120 }
                    TableHeaderCell { headerTitle: "Status"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Timestamp"; cellWidth: 150 }
                }
            }

            ListView {
                id: paymentListView
                width: parent.width
                height: parent.height - 45
                clip: true
                model: controller.payments

                delegate: Rectangle {
                    width: paymentListView.width
                    height: 40
                    color: index % 2 === 0 ? "#FFFFFF" : "#F8F9FA"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 15

                        TableDataCell { cellText: modelData && modelData.paymentId ? modelData.paymentId : ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData && modelData.bookingId ? modelData.bookingId : ""; cellWidth: 100 }
                        TableDataCell { cellText: "Rs. " + (modelData && modelData.amount !== undefined ? modelData.amount : "0"); cellWidth: 80 }
                        TableDataCell { cellText: modelData && modelData.paymentMethod ? modelData.paymentMethod : ""; cellWidth: 120 }
                        TableDataCell { cellText: modelData && modelData.paymentStatus ? modelData.paymentStatus : ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData && modelData.timeStamp ? modelData.timeStamp : ""; cellWidth: 150 }
                    }
                }
            }
        }

        // TAB 3: Refunds Section
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: customerBookingsPage.activeTabName === "REFUNDS"

            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 20

                    TableHeaderCell { headerTitle: "Refund ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Ticket ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Amount"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Status"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Refund Time"; cellWidth: 150 }
                }
            }

            ListView {
                id: refundListView
                width: parent.width
                height: parent.height - 45
                clip: true
                model: controller.refunds

                delegate: Rectangle {
                    width: refundListView.width
                    height: 40
                    color: index % 2 === 0 ? "#FFFFFF" : "#F8F9FA"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 20

                        TableDataCell { cellText: modelData && modelData.refundId ? modelData.refundId : ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData && modelData.ticketId ? modelData.ticketId : ""; cellWidth: 100 }
                        TableDataCell { cellText: "Rs. " + (modelData && modelData.refundAmount !== undefined ? modelData.refundAmount : "0"); cellWidth: 100 }
                        TableDataCell { cellText: modelData && modelData.refundStatus ? modelData.refundStatus : ""; cellWidth: 100 }
                        TableDataCell { cellText: modelData && modelData.refundTime ? modelData.refundTime : ""; cellWidth: 150 }
                    }
                }
            }
        }
    }

    // Dialog: Booking Details View
    Dialog {
        id: bookingDetailsDialog
        title: "Booking Details"
        modal: true
        anchors.centerIn: parent
        width: 380
        height: 300
        standardButtons: Dialog.Ok

        function showBookingInformation(
            bookingIdentifier,
            movieTitleName,
            theatreTitleName,
            showDateTimeString,
            seatsCountValue,
            bookingStatusString
        ) {
            rowBookingId.valueText = bookingIdentifier;
            rowMovieTitle.valueText = movieTitleName;
            rowTheatreName.valueText = theatreTitleName;
            rowShowTime.valueText = showDateTimeString;
            rowSeats.valueText = seatsCountValue;
            rowStatus.valueText = bookingStatusString;
            rowStatus.valueColor = bookingStatusString === "CONFIRMED" ? "green" : "red";

            bookingDetailsDialog.open();
        }

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            DetailRow { id: rowBookingId; labelName: "Booking ID:" }
            DetailRow { id: rowMovieTitle; labelName: "Movie Title:" }
            DetailRow { id: rowTheatreName; labelName: "Theatre Name:" }
            DetailRow { id: rowShowTime; labelName: "Show Date & Time:" }
            DetailRow { id: rowSeats; labelName: "Seats Booked:"; valueColor: "blue" }
            DetailRow { id: rowStatus; labelName: "Status:"; isValueBold: true }
        }
    }

    // Dialog: Cancel Confirmation
    Dialog {
        id: cancelConfirmationDialog
        title: "Confirm Cancellation"
        modal: true
        anchors.centerIn: parent
        width: 320
        height: 180
        standardButtons: Dialog.Yes | Dialog.No

        property string targetBookingId: ""

        onAccepted: {
            customerBookingsPage.processCancelBooking(targetBookingId);
        }

        Column {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 15

            Text {
                text: "Are you sure you want to cancel booking " + cancelConfirmationDialog.targetBookingId + "?"
                wrapMode: Text.Wrap
                width: parent.width
            }
        }
    }

    // Dialog: Cancellation Status/Result
    Dialog {
        id: cancelResultDialog
        title: cancelResultDialog.isSuccess ? "Cancellation Successful" : "Cancellation Failed"
        modal: true
        anchors.centerIn: parent
        width: 320
        height: 160
        standardButtons: Dialog.Ok

        property bool isSuccess: false
        property string resultMessage: ""

        function showResult(success, message) {
            cancelResultDialog.isSuccess = success
            cancelResultDialog.resultMessage = message
            cancelResultDialog.open()
        }

        Column {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 15

            Text {
                text: cancelResultDialog.resultMessage
                wrapMode: Text.Wrap
                width: parent.width
                color: cancelResultDialog.isSuccess ? "green" : "red"
            }
        }
    }
}