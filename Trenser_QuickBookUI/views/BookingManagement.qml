/*
 * File: BookingManagement.qml
 * Description: Implements the booking and financial management interface for
 *              theatre owners. Provides functionality to view and filter
 *              bookings, payments, and refunds. Integrates with the
 *              ControllerAdapter backend to load booking, payment, and refund
 *              data dynamically. Supports filtering bookings by status
 *              (ALL, CONFIRMED, CANCELLED) and displays structured tables for
 *              each section. Includes a booking details dialog to show
 *              comprehensive information about individual bookings. Designed
 *              with responsive layouts, consistent styling, and interactive
 *              controls aligned with the application’s UI palette.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: bookingManagementPage

    property string activeTabName: "BOOKINGS"
    property string selectedStatusFilter: "ALL"

    function isBookingVisible(status) {
        if (bookingManagementPage.selectedStatusFilter === "ALL") {
            return true;
        }
        return bookingManagementPage.selectedStatusFilter === status;
    }

    Component.onCompleted: {
        controller.loadBookings()
        controller.loadPayments()
        controller.loadRefunds()
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "Owner Booking & Financial Management"
            font.pixelSize: 20
            font.bold: true
            color: "#333333"
        }

        Row {
            spacing: 10

            Button {
                text: "1. View All Bookings"
                onClicked: {
                    bookingManagementPage.activeTabName = "BOOKINGS"
                    controller.loadBookings()
                }
            }

            Button {
                text: "2. View All Payments"
                onClicked: {
                    bookingManagementPage.activeTabName = "PAYMENTS"
                    controller.loadPayments()
                }
            }

            Button {
                text: "3. View All Refunds"
                onClicked: {
                    bookingManagementPage.activeTabName = "REFUNDS"
                    controller.loadRefunds()
                }
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#CCCCCC"
        }

        // Bookings Management Section
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: bookingManagementPage.activeTabName === "BOOKINGS"

            Row {
                spacing: 10
                Text {
                    text: "Filter Status:"
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
                Button {
                    text: "All"
                    onClicked: bookingManagementPage.selectedStatusFilter = "ALL"
                }
                Button {
                    text: "Confirmed"
                    onClicked: bookingManagementPage.selectedStatusFilter = "CONFIRMED"
                }
                Button {
                    text: "Cancelled"
                    onClicked: bookingManagementPage.selectedStatusFilter = "CANCELLED"
                }
            }

            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 10

                    Text { text: "Booking ID"; width: 90; font.bold: true }
                    Text { text: "Movie Name"; width: 130; font.bold: true }
                    Text { text: "Theatre"; width: 110; font.bold: true }
                    Text { text: "Date & Time"; width: 140; font.bold: true }
                    Text { text: "Seats"; width: 60; font.bold: true }
                    Text { text: "Status"; width: 90; font.bold: true }
                    Text { text: "Action"; width: 100; font.bold: true }
                }
            }

            ListView {
                id: bookingListView
                width: parent.width
                height: parent.height - 80
                clip: true
                model: controller.bookings

                delegate: Rectangle {
                    property bool isRowVisible: modelData ? bookingManagementPage.isBookingVisible(modelData.status) : false

                    width: bookingListView.width
                    visible: isRowVisible
                    height: isRowVisible ? 40 : 0
                    color: index % 2 === 0 ? "#FFFFFF" : "#F5F5F5"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 10

                        Text { text: modelData.bookingId ? modelData.bookingId : ""; width: 90; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.movieName ? modelData.movieName : ""; width: 130; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.theaterName ? modelData.theaterName : ""; width: 110; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.dateTime ? modelData.dateTime : ""; width: 140; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.seatsCount !== undefined ? String(modelData.seatsCount) : "0"; width: 60; anchors.verticalCenter: parent.verticalCenter }

                        Text {
                            text: modelData.status ? modelData.status : ""
                            width: 90
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                            color: modelData.status === "CONFIRMED" ? "green" : "red"
                        }

                        Button {
                            text: "Details"
                            anchors.verticalCenter: parent.verticalCenter
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
                    }
                }
            }
        }

        // Payments Management Section
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: bookingManagementPage.activeTabName === "PAYMENTS"

            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 15

                    Text { text: "Payment ID"; width: 100; font.bold: true }
                    Text { text: "Booking ID"; width: 100; font.bold: true }
                    Text { text: "Amount"; width: 80; font.bold: true }
                    Text { text: "Method"; width: 120; font.bold: true }
                    Text { text: "Status"; width: 100; font.bold: true }
                    Text { text: "Timestamp"; width: 150; font.bold: true }
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
                    color: index % 2 === 0 ? "#FFFFFF" : "#F5F5F5"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 15

                        Text { text: modelData.paymentId ? modelData.paymentId : ""; width: 100; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.bookingId ? modelData.bookingId : ""; width: 100; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: "Rs. " + (modelData.amount !== undefined ? modelData.amount : "0"); width: 80; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.paymentMethod ? modelData.paymentMethod : ""; width: 120; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.paymentStatus ? modelData.paymentStatus : ""; width: 100; anchors.verticalCenter: parent.verticalCenter; font.bold: true }
                        Text { text: modelData.timeStamp ? modelData.timeStamp : ""; width: 150; anchors.verticalCenter: parent.verticalCenter }
                    }
                }
            }
        }

        // Refunds Management Section
        Column {
            width: parent.width
            height: parent.height - 120
            spacing: 10
            visible: bookingManagementPage.activeTabName === "REFUNDS"

            Rectangle {
                width: parent.width
                height: 35
                color: "#E0E0E0"
                border.color: "#CCCCCC"

                Row {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 20

                    Text { text: "Refund ID"; width: 100; font.bold: true }
                    Text { text: "Ticket ID"; width: 100; font.bold: true }
                    Text { text: "Amount"; width: 100; font.bold: true }
                    Text { text: "Status"; width: 100; font.bold: true }
                    Text { text: "Refund Time"; width: 150; font.bold: true }
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
                    color: index % 2 === 0 ? "#FFFFFF" : "#F5F5F5"
                    border.color: "#E0E0E0"

                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        spacing: 20

                        Text { text: modelData.refundId ? modelData.refundId : ""; width: 100; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.ticketId ? modelData.ticketId : ""; width: 100; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: "Rs. " + (modelData.refundAmount !== undefined ? modelData.refundAmount : "0"); width: 100; anchors.verticalCenter: parent.verticalCenter }
                        Text { text: modelData.refundStatus ? modelData.refundStatus : ""; width: 100; anchors.verticalCenter: parent.verticalCenter; font.bold: true }
                        Text { text: modelData.refundTime ? modelData.refundTime : ""; width: 150; anchors.verticalCenter: parent.verticalCenter }
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
            detailBookingIdText.text = bookingIdentifier
            detailMovieTitleText.text = movieTitleName
            detailTheatreNameText.text = theatreTitleName
            detailShowTimeText.text = showDateTimeString
            detailSeatsText.text = seatsCountValue
            detailStatusText.text = bookingStatusString
            detailStatusText.color = bookingStatusString === "CONFIRMED" ? "green" : "red"

            bookingDetailsDialog.open()
        }

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            Row {
                spacing: 10
                Text { text: "Booking ID:"; font.bold: true; width: 120 }
                Text { id: detailBookingIdText; text: "" }
            }

            Row {
                spacing: 10
                Text { text: "Movie Title:"; font.bold: true; width: 120 }
                Text { id: detailMovieTitleText; text: "" }
            }

            Row {
                spacing: 10
                Text { text: "Theatre Name:"; font.bold: true; width: 120 }
                Text { id: detailTheatreNameText; text: "" }
            }

            Row {
                spacing: 10
                Text { text: "Show Date & Time:"; font.bold: true; width: 120 }
                Text { id: detailShowTimeText; text: "" }
            }

            Row {
                spacing: 10
                Text { text: "Seats Booked:"; font.bold: true; width: 120 }
                Text { id: detailSeatsText; text: ""; color: "blue" }
            }

            Row {
                spacing: 10
                Text { text: "Status:"; font.bold: true; width: 120 }
                Text { id: detailStatusText; text: ""; font.bold: true }
            }

            Item { width: 1; height: 10 }

            Button {
                text: "Close"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: bookingDetailsDialog.close()
            }
        }
    }
}