/*
 * File: CustomerBookingView.qml
 * Description: Implements the customer bookings, payments, and refunds
 *              interface for the Theatre Management System. Displays the
 *              customer's bookings, payments, and refund history in separate
 *              tabbed table views, loaded from ControllerAdapter's bookings,
 *              payments, and refunds properties. Provides a booking details
 *              popup and a cancel-booking flow that confirms with the user,
 *              calls ControllerAdapter::cancelBooking, and surfaces the
 *              resulting success/failure message. Bookings, payments, and
 *              refunds refresh automatically after a successful cancellation.
 * Author: Trenser
 * Created: 23 July 2026
 */
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customerBookingsPage

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

    property string activeTabName: "BOOKINGS" // Options: "BOOKINGS", "PAYMENTS", "REFUNDS"

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
                    height: 40
                    color: index % 2 === 0 ? "#FFFFFF" : "#F8F9FA"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 10

                        TableDataCell { cellText: model.bookingId; cellWidth: 100 }
                        TableDataCell { cellText: model.movieName; cellWidth: 150 }
                        TableDataCell { cellText: model.dateTime; cellWidth: 140 }
                        TableDataCell { cellText: model.seatsCount; cellWidth: 80 }
                        TableDataCell {
                            cellText: model.status
                            cellWidth: 100
                            textCustomColor: model.status === "CONFIRMED" ? "green" : "red"
                        }

                        Row {
                            spacing: 5
                            anchors.verticalCenter: parent.verticalCenter

                            Button {
                                text: "Details"
                                onClicked: {
                                    bookingDetailsDialog.showBookingInformation(
                                        model.bookingId,
                                        model.movieName,
                                        model.theaterName,
                                        model.dateTime,
                                        model.seatsCount,
                                        model.status
                                    )
                                }
                            }

                            Button {
                                text: "Cancel"
                                enabled: model.status === "CONFIRMED"
                                onClicked: {
                                    cancelConfirmationDialog.targetBookingId = model.bookingId;
                                    cancelConfirmationDialog.open();
                                }
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

                        TableDataCell { cellText: model.paymentId; cellWidth: 100 }
                        TableDataCell { cellText: model.bookingId; cellWidth: 100 }
                        TableDataCell { cellText: "Rs. " + model.amount; cellWidth: 80 }
                        TableDataCell { cellText: model.paymentMethod; cellWidth: 120 }
                        TableDataCell { cellText: model.paymentStatus; cellWidth: 100 }
                        TableDataCell { cellText: model.timeStamp; cellWidth: 150 }
                    }
                }
            }
        }

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

                        TableDataCell { cellText: model.refundId; cellWidth: 100 }
                        TableDataCell { cellText: model.ticketId; cellWidth: 100 }
                        TableDataCell { cellText: "Rs. " + model.refundAmount; cellWidth: 100 }
                        TableDataCell { cellText: model.refundStatus; cellWidth: 100 }
                        TableDataCell { cellText: model.refundTime; cellWidth: 150 }
                    }
                }
            }
        }
    }

    Dialog {
        id: bookingDetailsDialog
        title: "Booking Details"
        modal: true
        anchors.centerIn: parent
        width: 380
        height: 260

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

            Item { width: 1; height: 10 }

            Button {
                text: "Close"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: bookingDetailsDialog.close()
            }
        }
    }

    Dialog {
        id: cancelConfirmationDialog
        title: "Confirm Cancellation"
        modal: true
        anchors.centerIn: parent
        width: 320
        height: 180

        property string targetBookingId: ""

        Column {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 15

            Text {
                text: "Are you sure you want to cancel booking " + cancelConfirmationDialog.targetBookingId + "?"
                wrapMode: Text.Wrap
                width: parent.width
            }

            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: "Yes, Cancel"
                    onClicked: {
                        customerBookingsPage.processCancelBooking(cancelConfirmationDialog.targetBookingId);
                        cancelConfirmationDialog.close();
                    }
                }

                Button {
                    text: "No, Keep"
                    onClicked: cancelConfirmationDialog.close()
                }
            }
        }
    }

    Dialog {
        id: cancelResultDialog
        title: cancelResultDialog.isSuccess ? "Cancellation Successful" : "Cancellation Failed"
        modal: true
        anchors.centerIn: parent
        width: 320
        height: 160

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

            Button {
                text: "OK"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: cancelResultDialog.close()
            }
        }
    }
}