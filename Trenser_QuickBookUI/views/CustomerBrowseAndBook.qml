/*
 * File: CustomerBroeserAndBook.qml
 * Description: Implements the customer-facing movie booking portal in QML.
 *              Provides a step-by-step workflow for browsing movies, selecting
 *              shows, choosing seats, confirming bookings, and completing
 *              payments. Integrates with the ControllerAdapter backend to
 *              fetch movies, shows, seat layouts, and process bookings and
 *              payments. Includes reusable UI components for table headers,
 *              data cells, and themed styling. Supports seat selection with
 *              validation (max 10 seats), booking confirmation summary, and
 *              multiple payment methods (Card, UPI) with input validation.
 *              Designed with responsive layouts, consistent theming, and
 *              interactive controls aligned with the application’s UI palette.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: customerPage

    // -------------------------------------------------------------------------
    // 1. REUSABLE UI COMPONENTS (Themed)
    // -------------------------------------------------------------------------
    component TableHeaderCell : Text {
        property string headerTitle: ""
        property int cellWidth: 100
        text: headerTitle
        width: cellWidth
        font.bold: true
        color: window.textMuted // Themed
        font.pixelSize: 13
    }

    component TableDataCell : Text {
        property string cellText: ""
        property int cellWidth: 100
        property color textCustomColor: window.textDark // Themed
        text: cellText
        width: cellWidth
        color: textCustomColor
        anchors.verticalCenter: parent.verticalCenter
        elide: Text.ElideRight
        font.pixelSize: 12
    }

    // -------------------------------------------------------------------------
    // 2. DATA MODELS
    // -------------------------------------------------------------------------
    ListModel { id: activeMoviesModel }
    ListModel { id: showDetailsModel }
    ListModel { id: seatGridModel }

    // -------------------------------------------------------------------------
    // 3. PAGE STATE PROPERTIES
    // -------------------------------------------------------------------------
    property string activeViewStep: "BROWSE_MOVIES"
    property string selectedMovieId: ""
    property string selectedMovieTitle: ""
    property string selectedShowId: ""
    property string selectedShowTime: ""
    property real singleTicketPrice: 0.0
    property var chosenSeatList: []
    property real totalBookingAmount: 0.0

    // Backend Response Properties
    property string activeBookingId: ""
    property string activeBookingStatus: ""

    // -------------------------------------------------------------------------
    // 4. HELPER FUNCTIONS
    // -------------------------------------------------------------------------
    Component.onCompleted: {
        loadMoviesWithShows();
    }

    function loadMoviesWithShows() {
        activeMoviesModel.clear();
        var allMovies = controller.getAllMovies();
        console.log("movies fetched:", allMovies.length);

        for (var j = 0; j < allMovies.length; j++) {
            var movieData = allMovies[j];
            if (!movieData.active) continue;

            // Check per-movie instead of pre-building a title lookup from
            // getActiveShows() — avoids relying on title matching, since
            // getShowsForMovie() is keyed by the actual movie ID.
            var showsForThisMovie = controller.getShowsForMovie(movieData.movieId);
            console.log(movieData.title, "shows:", showsForThisMovie.length);

            if (showsForThisMovie.length > 0) {
                activeMoviesModel.append({
                    movieId: movieData.movieId,
                    title: movieData.title,
                    language: movieData.language,
                    genre: movieData.genre,
                    duration: movieData.duration + " mins"
                });
            }
        }
    }

    function loadSeatLayoutForShow(showId) {
        seatGridModel.clear();
        var freshSeatLayout = controller.getShowSeatLayout(showId);
        console.log("seats fetched for show", showId, ":", freshSeatLayout.length);

        for (var i = 0; i < freshSeatLayout.length; i++) {
            var seatData = freshSeatLayout[i];
            // status arrives as "AVAILABLE"/"BOOKED"/"BLOCKED" directly from the adapter.
            seatGridModel.append({
                seatId: seatData.id,
                seatStatus: seatData.status,
                seatAmount: seatData.amount
            });
        }
        console.log("seatGridModel total count:", seatGridModel.count);
    }

    function filterShowsByMovieId(targetMovieId) {
        showDetailsModel.clear();
        var showsForMovie = controller.getShowsForMovie(targetMovieId);

        for (var i = 0; i < showsForMovie.length; i++) {
            var showData = showsForMovie[i];
            showDetailsModel.append({
                showId: showData.id,
                theatreName: showData.theatre,
                screenId: showData.screen,
                movieTitle: showData.movie,
                showDateAndTime: showData.startingTime,
                ticketPrice: showData.price
            });
        }
    }

    function isSeatAlreadyInChosenList(targetSeatId) {
        for (var i = 0; i < customerPage.chosenSeatList.length; i++) {
            if (customerPage.chosenSeatList[i] === targetSeatId) return true;
        }
        return false;
    }

    function toggleSeatSelection(targetSeatId, targetSeatAmount) {
        var tempSeatList = customerPage.chosenSeatList.slice();
        var existingIndex = tempSeatList.indexOf(targetSeatId);

        if (existingIndex !== -1) {
            tempSeatList.splice(existingIndex, 1);
            customerPage.totalBookingAmount -= targetSeatAmount;
        } else {
            if (tempSeatList.length >= 10) {
                seatSelectionErrorMessage.text = "You can only select up to 10 seats!";
                return;
            }
            tempSeatList.push(targetSeatId);
            customerPage.totalBookingAmount += targetSeatAmount;
        }

        customerPage.chosenSeatList = tempSeatList;
        seatSelectionErrorMessage.text = "";
    }

    // -------------------------------------------------------------------------
    // 5. MAIN LAYOUT
    // -------------------------------------------------------------------------
    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "Customer Movie Booking Portal"
            font.pixelSize: 22
            font.bold: true
            color: window.textDark // Themed
        }

        // =====================================================================
        // STEP 1: BROWSE MOVIES
        // =====================================================================
        Column {
            width: parent.width
            spacing: 10
            visible: customerPage.activeViewStep === "BROWSE_MOVIES"

            Text { text: "Select a Movie:"; font.pixelSize: 16; font.bold: true; color: window.textDark }

            Rectangle {
                width: parent.width; height: 35; color: window.bgLight; border.color: window.borderColor // Themed
                Row {
                    anchors.fill: parent; anchors.margins: 5; spacing: 10
                    TableHeaderCell { headerTitle: "Movie ID"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Title"; cellWidth: 150 }
                    TableHeaderCell { headerTitle: "Language"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Genre"; cellWidth: 120 }
                    TableHeaderCell { headerTitle: "Duration"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Action"; cellWidth: 100 }
                }
            }

            ListView {
                id: movieListView
                width: parent.width; height: 250; clip: true
                model: activeMoviesModel
                delegate: Rectangle {
                    width: movieListView.width; height: 40
                    color: index % 2 === 0 ? window.cardBg : window.bgLight // Themed
                    border.color: window.borderColor
                    Row {
                        anchors.fill: parent; anchors.margins: 5; spacing: 10
                        TableDataCell { cellText: model.movieId; cellWidth: 100 }
                        TableDataCell { cellText: model.title; cellWidth: 150 }
                        TableDataCell { cellText: model.language; cellWidth: 100 }
                        TableDataCell { cellText: model.genre; cellWidth: 120 }
                        TableDataCell { cellText: model.duration; cellWidth: 100 }
                        Button {
                            text: "View Shows"
                            anchors.verticalCenter: parent.verticalCenter
                            palette.button: window.primaryRed // BMS Theme
                            palette.buttonText: window.cardBg
                            onClicked: {
                                customerPage.selectedMovieId = model.movieId;
                                customerPage.selectedMovieTitle = model.title;
                                customerPage.filterShowsByMovieId(model.movieId);
                                customerPage.activeViewStep = "SELECT_SHOW";
                            }
                        }
                    }
                }
            }
        }

        // =====================================================================
        // STEP 2: SELECT SHOW
        // =====================================================================
        Column {
            width: parent.width
            spacing: 10
            visible: customerPage.activeViewStep === "SELECT_SHOW"

            Row {
                spacing: 20
                Button {
                    text: "< Back to Movies"
                    onClicked: { customerPage.loadMoviesWithShows(); customerPage.activeViewStep = "BROWSE_MOVIES" }
                }
                Text { text: "Available Shows for: " + customerPage.selectedMovieTitle; font.pixelSize: 16; font.bold: true; anchors.verticalCenter: parent.verticalCenter; color: window.textDark }
            }

            Rectangle {
                width: parent.width; height: 35; color: window.bgLight; border.color: window.borderColor // Themed
                Row {
                    anchors.fill: parent; anchors.margins: 5; spacing: 10
                    TableHeaderCell { headerTitle: "Show ID"; cellWidth: 90 }
                    TableHeaderCell { headerTitle: "Theatre Name"; cellWidth: 140 }
                    TableHeaderCell { headerTitle: "Screen"; cellWidth: 80 }
                    TableHeaderCell { headerTitle: "Show Date & Time"; cellWidth: 150 }
                    TableHeaderCell { headerTitle: "Ticket Price"; cellWidth: 100 }
                    TableHeaderCell { headerTitle: "Action"; cellWidth: 100 }
                }
            }

            ListView {
                id: showListView
                width: parent.width; height: 250; clip: true
                model: showDetailsModel
                delegate: Rectangle {
                    width: showListView.width; height: 40
                    color: index % 2 === 0 ? window.cardBg : window.bgLight // Themed
                    border.color: window.borderColor
                    Row {
                        anchors.fill: parent; anchors.margins: 5; spacing: 10
                        TableDataCell { cellText: model.showId; cellWidth: 90 }
                        TableDataCell { cellText: model.theatreName; cellWidth: 140 }
                        TableDataCell { cellText: model.screenId; cellWidth: 80 }
                        TableDataCell { cellText: model.showDateAndTime; cellWidth: 150 }
                        TableDataCell { cellText: model.ticketPrice; cellWidth: 100 } // Price loaded dynamically via Seat Layout
                        Button {
                            text: "Select Seats"
                            anchors.verticalCenter: parent.verticalCenter
                            palette.button: window.primaryRed // BMS Theme
                            palette.buttonText: window.cardBg
                            onClicked: {
                                customerPage.selectedShowId = model.showId;
                                customerPage.selectedShowTime = model.showDateAndTime;
                                customerPage.singleTicketPrice = model.ticketPrice;
                                customerPage.chosenSeatList = [];
                                customerPage.totalBookingAmount = 0.0;

                                customerPage.loadSeatLayoutForShow(model.showId);
                                customerPage.activeViewStep = "SELECT_SEATS";
                            }
                        }
                    }
                }
            }
        }

        // =====================================================================
        // STEP 3: SEAT SELECTION MAP
        // =====================================================================
        Column {
            width: parent.width
            spacing: 15
            visible: customerPage.activeViewStep === "SELECT_SEATS"

            Row {
                spacing: 20
                Button { text: "< Back to Shows"; onClicked: customerPage.activeViewStep = "SELECT_SHOW" }
                Text { text: "Select Seats for Show: " + customerPage.selectedShowId; font.pixelSize: 16; font.bold: true; anchors.verticalCenter: parent.verticalCenter; color: window.textDark }
            }

            Rectangle {
                width: parent.width * 0.7; height: 25; color: window.borderColor // Themed grey for screen
                anchors.horizontalCenter: parent.horizontalCenter
                Text { text: "SCREEN THIS WAY"; anchors.centerIn: parent; font.bold: true; color: window.textMuted }
            }

            GridView {
                id: seatGridView
                width: 260; height: 180; cellWidth: 60; cellHeight: 50
                anchors.horizontalCenter: parent.horizontalCenter
                model: seatGridModel

                delegate: Rectangle {
                    width: 50; height: 40;
                    property bool isChosen: customerPage.isSeatAlreadyInChosenList(model.seatId)

                    // Style matching BMS (Grey = Unavailable, Green = Selected, White/Border = Available)
                    color: (model.seatStatus === "BOOKED" || model.seatStatus === "BLOCKED") ? window.borderColor : (isChosen ? "#2ECC71" : window.cardBg)
                    border.color: (model.seatStatus === "BOOKED" || model.seatStatus === "BLOCKED") ? window.borderColor : (isChosen ? "#2ECC71" : window.primaryRed)

                    Text {
                        text: model.seatId
                        anchors.centerIn: parent
                        font.bold: true
                        color: isChosen ? window.cardBg : (model.seatStatus === "BOOKED" || model.seatStatus === "BLOCKED" ? window.cardBg : window.textDark)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (model.seatStatus === "AVAILABLE") {
                                customerPage.toggleSeatSelection(model.seatId, model.seatAmount);
                            }
                        }
                    }
                }
            }

            Text { id: seatSelectionErrorMessage; text: ""; color: window.primaryRed; anchors.horizontalCenter: parent.horizontalCenter }

            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    text: "Selected Seats: " + (customerPage.chosenSeatList.length > 0 ? customerPage.chosenSeatList.join(", ") : "None")
                    font.pixelSize: 14; font.bold: true; color: window.textDark
                }

                Text {
                    text: "Total Amount: Rs. " + customerPage.totalBookingAmount
                    font.pixelSize: 14; font.bold: true; color: window.primaryRed // Themed Highlight
                }

                Button {
                    text: "Book Seats"
                    enabled: customerPage.chosenSeatList.length > 0
                    palette.button: enabled ? window.primaryRed : window.borderColor // Disabled state styling
                    palette.buttonText: enabled ? window.cardBg : window.textMuted
                    onClicked: {
                        // FIXED: was controllerAdapter (not registered) — controller is.
                        var result = controller.bookSeats(customerPage.selectedShowId, customerPage.chosenSeatList);
                        if (result.success) {
                            customerPage.activeBookingId = result.bookingId;
                            customerPage.activeBookingStatus = result.status;
                            if (result.amount > 0) {
                                customerPage.totalBookingAmount = result.amount;
                            }
                            customerPage.activeViewStep = "CONFIRM_BOOKING";
                        } else {
                            seatSelectionErrorMessage.text = result.message || "Failed to book seats. Please try again.";
                        }
                    }
                }
            }
        }

        // =====================================================================
        // STEP 4: BOOKING CONFIRMATION SUMMARY & PROCEED TO PAYMENT
        // =====================================================================
        Column {
            width: parent.width
            spacing: 15
            visible: customerPage.activeViewStep === "CONFIRM_BOOKING"

            Row {
                spacing: 20
                Button {
                    text: "< Back to Seat Selection"
                    onClicked: {
                        customerPage.chosenSeatList = [];
                        customerPage.totalBookingAmount = 0.0;
                        customerPage.activeViewStep = "SELECT_SEATS";
                    }
                }
                Text { text: "Booking Details"; font.pixelSize: 18; font.bold: true; anchors.verticalCenter: parent.verticalCenter; color: window.textDark }
            }

            Rectangle {
                width: parent.width * 0.8
                height: 180
                color: window.cardBg // Themed
                border.color: window.borderColor
                radius: 5
                anchors.horizontalCenter: parent.horizontalCenter

                Column {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Text { text: "Booking ID: " + customerPage.activeBookingId; font.bold: true; font.pixelSize: 15; color: window.textDark }
                    Text { text: "Movie: " + customerPage.selectedMovieTitle; font.pixelSize: 14; color: window.textMuted }
                    Text { text: "Show Time: " + customerPage.selectedShowTime; font.pixelSize: 14; color: window.textMuted }
                    Text { text: "Selected Seats: " + customerPage.chosenSeatList.join(", "); font.pixelSize: 14; font.bold: true; color: window.textDark }
                    Text { text: "Total Amount to Pay: Rs. " + customerPage.totalBookingAmount; font.pixelSize: 16; font.bold: true; color: window.primaryRed }
                }
            }

            Button {
                text: "Proceed to Payment"
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                palette.button: window.primaryRed // Themed main CTA
                palette.buttonText: window.cardBg
                onClicked: paymentWindowDialog.openPaymentDialog()
            }
        }
    }

    // =========================================================================
    // PAYMENT METHOD POPUP DIALOG
    // =========================================================================
    Dialog {
        id: paymentWindowDialog
        title: "Complete Payment"
        modal: true
        anchors.centerIn: parent
        width: 380
        height: 380

        property string currentPaymentMethod: "CARD"

        function openPaymentDialog() {
            paymentStatusNotice.text = "";
            cardNumberInput.text = "";
            cardExpiryInput.text = "";
            cardCvvInput.text = "";
            upiIdInput.text = "";
            paymentWindowDialog.open();
        }

        // Applying global card theme to dialog background
        background: Rectangle {
            color: window.cardBg
            border.color: window.borderColor
            radius: 8
        }

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Text {
                text: "Total Amount: Rs. " + customerPage.totalBookingAmount
                font.bold: true; font.pixelSize: 15
                color: window.textDark
            }

            Row {
                spacing: 10
                Button {
                    text: "Card Payment"
                    palette.button: paymentWindowDialog.currentPaymentMethod === "CARD" ? window.textDark : window.bgLight
                    palette.buttonText: paymentWindowDialog.currentPaymentMethod === "CARD" ? window.cardBg : window.textDark
                    onClicked: paymentWindowDialog.currentPaymentMethod = "CARD"
                }
                Button {
                    text: "UPI Payment"
                    palette.button: paymentWindowDialog.currentPaymentMethod === "UPI" ? window.textDark : window.bgLight
                    palette.buttonText: paymentWindowDialog.currentPaymentMethod === "UPI" ? window.cardBg : window.textDark
                    onClicked: paymentWindowDialog.currentPaymentMethod = "UPI"
                }
            }

            // --- Card Form ---
            Column {
                spacing: 8
                width: parent.width
                visible: paymentWindowDialog.currentPaymentMethod === "CARD"

                TextField {
                    id: cardNumberInput
                    placeholderText: "Card Number (16 digits)"
                    width: parent.width
                    maximumLength: 16
                }
                Row {
                    spacing: 10
                    TextField {
                        id: cardExpiryInput
                        placeholderText: "MM/YY"
                        width: 120
                        maximumLength: 5
                    }
                    TextField {
                        id: cardCvvInput
                        placeholderText: "CVV (3 digits)"
                        width: 120
                        maximumLength: 3
                        echoMode: TextInput.Password
                    }
                }
            }

            // --- UPI Form ---
            Column {
                spacing: 8
                width: parent.width
                visible: paymentWindowDialog.currentPaymentMethod === "UPI"

                TextField {
                    id: upiIdInput
                    placeholderText: "Enter UPI ID (e.g., user@bank)"
                    width: parent.width
                }
            }

            Text { id: paymentStatusNotice; text: ""; color: window.primaryRed }

            Row {
                spacing: 15
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: "Pay Now"
                    palette.button: window.primaryRed
                    palette.buttonText: window.cardBg
                    onClicked: {
                        var isValidInput = false;

                        if (paymentWindowDialog.currentPaymentMethod === "CARD") {
                            if (cardNumberInput.text.length === 16 && cardExpiryInput.text.length === 5 && cardCvvInput.text.length === 3) {
                                isValidInput = true;
                            } else {
                                paymentStatusNotice.color = window.primaryRed;
                                paymentStatusNotice.text = "Invalid card details. Please check!";
                            }
                        } else {
                            if (upiIdInput.text.indexOf("@") !== -1) {
                                isValidInput = true;
                            } else {
                                paymentStatusNotice.color = window.primaryRed;
                                paymentStatusNotice.text = "Invalid UPI ID. Please check!";
                            }
                        }

                        if (isValidInput) {
                            // FIXED: was controllerAdapter (not registered) — controller is.
                            var payResult = controller.processPayment(
                                customerPage.activeBookingId,
                                paymentWindowDialog.currentPaymentMethod,
                                customerPage.totalBookingAmount
                            );

                            if (payResult.success) {
                                paymentStatusNotice.color = "#2ECC71"; // Success Green
                                paymentStatusNotice.text = "Payment successful! Ticket booked.";
                                confirmTimer.start();
                            } else {
                                paymentStatusNotice.color = window.primaryRed;
                                paymentStatusNotice.text = payResult.message || "Payment processing failed!";
                            }
                        }
                    }
                }

                Button {
                    text: "Cancel"
                    onClicked: paymentWindowDialog.close()
                }
            }
        }

        Timer {
            id: confirmTimer
            interval: 1500
            repeat: false
            onTriggered: {
                paymentWindowDialog.close();
                customerPage.loadMoviesWithShows();
                customerPage.activeViewStep = "BROWSE_MOVIES";
            }
        }
    }
}
