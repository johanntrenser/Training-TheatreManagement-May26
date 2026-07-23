/*
 * File: ShowManagementPage.qml
 * Description: Implements the show management interface for theatre owners.
 *              Provides functionality to schedule, update, cancel, and monitor
 *              theatre show listings. Integrates with the ControllerAdapter
 *              backend to fetch theatres, movies, screens, and active shows.
 *              Supports filtering by active or all shows, searching by ID,
 *              movie, or screen, and displays results in a structured table
 *              with badges for screen and status. Includes dialogs for adding
 *              new shows, updating show times, and confirming cancellations,
 *              with validation for date/time formats and conflict detection.
 *              Designed with responsive layouts, consistent styling, and
 *              interactive controls aligned with the application’s UI palette.
 * Author: Trenser
 * Created: 23 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: showManagementPage

    property string currentTab: "ACTIVE" // "ACTIVE", "ALL"
    property string selectedTheatreId: ""
    property string selectedMovieId: ""
    property string searchQuery: ""
    property var theatresList: []
    property var showsList: []
    property var ownTheatreMovies: []
    property var ownTheatreScreens: []

    function refreshTheatres() {
        theatresList = controller.getMyTheatres()
        if (theatresList.length > 0 && selectedTheatreId === "") {
            selectedTheatreId = theatresList[0].id
        }
    }

    function refreshShows() {
        if (selectedTheatreId === "") { showsList = []; return }

        ownTheatreMovies = controller.getMoviesInTheatre(selectedTheatreId)
        ownTheatreScreens = controller.getScreensInTheatre(selectedTheatreId)

        var allTheatreShows = controller.getActiveShows()

        if (currentTab === "ACTIVE") {
            var active = []
            for (var indexOne = 0; indexOne < allTheatreShows.length; indexOne++) {
                if (allTheatreShows[indexOne].status !== "CANCELLED") active.push(allTheatreShows[indexOne])
            }
            showsList = active
        }
        else if (currentTab === "ALL") {
            showsList = allTheatreShows
        }
    }

    Component.onCompleted: {
        refreshTheatres()
        refreshShows()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // 1. TOP HEADER BAR & PRIMARY ACTIONS
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "Show Management"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: "Schedule, update, cancel, and monitor theatre show listings"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                spacing: 10

                CustomButton {
                    text: "➕ Add Show"
                    implicitWidth: 120
                    btnColor: window.primaryRed
                    textColor: "#FFFFFF"
                    enabled: showManagementPage.selectedTheatreId !== ""
                    onClicked: addShowDialog.open()
                }

                CustomButton {
                    text: "⟳ Refresh"
                    implicitWidth: 100
                    btnColor: "#E5E5E5"
                    textColor: window.textDark
                    onClicked: {
                        showManagementPage.refreshTheatres()
                        showManagementPage.refreshShows()
                    }
                }
            }
        }

        // 2. THEATRE SELECTOR, TABS, MOVIE SELECTOR, SEARCH
        RowLayout {
            Layout.fillWidth: true
            spacing: 15

            ComboBox {
                id: theatrePicker
                Layout.preferredWidth: 200
                Layout.preferredHeight: 36
                model: showManagementPage.theatresList
                textRole: "name"
                onCurrentIndexChanged: {
                    if (currentIndex >= 0 && showManagementPage.theatresList[currentIndex]) {
                        showManagementPage.selectedTheatreId = showManagementPage.theatresList[currentIndex].id
                        showManagementPage.refreshShows()
                    }
                }
            }

            RowLayout {
                spacing: 8

                CustomButton {
                    text: "Active Shows"
                    implicitWidth: 110; implicitHeight: 36
                    btnColor: showManagementPage.currentTab === "ACTIVE" ? window.primaryRed : "#E5E5E5"
                    textColor: showManagementPage.currentTab === "ACTIVE" ? "#FFFFFF" : window.textDark
                    onClicked: { showManagementPage.currentTab = "ACTIVE"; showManagementPage.refreshShows() }
                }

                CustomButton {
                    text: "All Shows"
                    implicitWidth: 100; implicitHeight: 36
                    btnColor: showManagementPage.currentTab === "ALL" ? window.primaryRed : "#E5E5E5"
                    textColor: showManagementPage.currentTab === "ALL" ? "#FFFFFF" : window.textDark
                    onClicked: { showManagementPage.currentTab = "ALL"; showManagementPage.refreshShows() }
                }
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                Layout.preferredWidth: 260
                Layout.preferredHeight: 36
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
                        placeholderText: "Search Show / Movie / Screen..."
                        background: Item {}
                        onTextChanged: showManagementPage.searchQuery = text.toLowerCase()
                    }
                }
            }
        }

        // 3. SHOWS CONSOLE TABLE DISPLAY
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

                        Text { text: "SHOW ID"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.preferredWidth: 80; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "MOVIE TITLE"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.fillWidth: true; Layout.preferredWidth: 150; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "THEATRE NAME"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.fillWidth: true; Layout.preferredWidth: 150; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "SCREEN"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.preferredWidth: 90; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "START"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.preferredWidth: 130; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "END"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.preferredWidth: 130; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "STATUS"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.preferredWidth: 100; Layout.alignment: Qt.AlignVCenter }
                        Text { text: "ACTIONS"; font.bold: true; font.pixelSize: 11; color: window.textMuted; Layout.preferredWidth: 160; Layout.alignment: Qt.AlignVCenter }
                    }
                }

                ListView {
                    id: showListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: showManagementPage.showsList

                    delegate: Rectangle {
                        width: showListView.width
                        height: visible ? 52 : 0
                        color: index % 2 === 0 ? "#FFFFFF" : "#F9FAFB"

                        visible: {
                            var query = showManagementPage.searchQuery
                            if (query === "") return true
                            var idMatch = modelData.id && modelData.id.toLowerCase().indexOf(query) !== -1
                            var movieMatch = modelData.movie && modelData.movie.toLowerCase().indexOf(query) !== -1
                            var screenMatch = modelData.screen && modelData.screen.toLowerCase().indexOf(query) !== -1
                            return idMatch || movieMatch || screenMatch
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

                            // 1. Show ID
                            Text {
                                text: modelData.id || ""
                                font.bold: true
                                font.pixelSize: 12
                                color: window.textDark
                                Layout.preferredWidth: 80
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 2. Movie Title
                            Text {
                                text: modelData.movie || ""
                                font.pixelSize: 12
                                font.bold: true
                                color: window.textDark
                                Layout.fillWidth: true
                                Layout.preferredWidth: 150
                                elide: Text.ElideRight
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 3. Theatre Name
                            Text {
                                text: modelData.theatre || ""
                                font.pixelSize: 12
                                font.bold: true
                                color: window.textDark
                                Layout.fillWidth: true
                                Layout.preferredWidth: 150
                                elide: Text.ElideRight
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 4. Screen Badge
                            Item {
                                Layout.preferredWidth: 90
                                Layout.preferredHeight: 22
                                Layout.alignment: Qt.AlignVCenter
                                Rectangle {
                                    anchors.fill: parent
                                    radius: 4
                                    color: "#E8F0FE"
                                    Text {
                                        anchors.centerIn: parent
                                        text: modelData.screen || ""
                                        font.pixelSize: 11
                                        font.bold: true
                                        color: "#1A73E8"
                                    }
                                }
                            }

                            // 5. Start Time
                            Text {
                                text: modelData.startingTime || ""
                                font.pixelSize: 11
                                color: window.textDark
                                Layout.preferredWidth: 130
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 6. End Time
                            Text {
                                text: modelData.endingTime || ""
                                font.pixelSize: 11
                                color: window.textDark
                                Layout.preferredWidth: 130
                                Layout.alignment: Qt.AlignVCenter
                            }

                            // 7. Status Badge
                            Item {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 22
                                Layout.alignment: Qt.AlignVCenter
                                Rectangle {
                                    anchors.fill: parent
                                    radius: 4
                                    color: modelData.status === "CANCELLED" ? "#FFEBEE" : "#E8F5E9"
                                    Text {
                                        anchors.centerIn: parent
                                        text: modelData.status || "UNKNOWN"
                                        font.pixelSize: 10
                                        font.bold: true
                                        color: modelData.status === "CANCELLED" ? "#C62828" : "#2E7D32"
                                    }
                                }
                            }

                            // 8. Actions
                            RowLayout {
                                Layout.preferredWidth: 160
                                Layout.alignment: Qt.AlignVCenter
                                spacing: 6

                                CustomButton {
                                    text: "✏️ Edit"
                                    implicitWidth: 65; implicitHeight: 28
                                    btnColor: "#E3F2FD"; textColor: "#1565C0"
                                    enabled: modelData.status !== "CANCELLED"
                                    onClicked: {
                                        updateShowDialog.targetShowId = modelData.id
                                        updateShowDialog.open()
                                    }
                                }

                                CustomButton {
                                    text: "✕ Cancel"
                                    implicitWidth: 75; implicitHeight: 28
                                    btnColor: "#FFEBEE"; textColor: "#C62828"
                                    enabled: modelData.status !== "CANCELLED"
                                    onClicked: {
                                        cancelConfirmDialog.targetShowId = modelData.id
                                        cancelConfirmDialog.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // DIALOGS

    // 1. ADD SHOW DIALOG
    Dialog {
        id: addShowDialog
        title: "Add New Show"
        anchors.centerIn: parent
        width: 420
        modal: true
        standardButtons: Dialog.NoButton

        function clearFields() {
            addDateField.text = ""
            addTimeField.text = ""
            errorText.visible = false
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            Text { text: "Select Movie:"; font.bold: true }
            ComboBox {
                id: addMovieCombo
                Layout.fillWidth: true
                model: showManagementPage.ownTheatreMovies
                textRole: "title"
            }

            Text { text: "Select Screen:"; font.bold: true }
            ComboBox {
                id: addScreenCombo
                Layout.fillWidth: true
                model: showManagementPage.ownTheatreScreens
                textRole: "name"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    Text { text: "Show Date (YYYY-MM-DD)"; font.bold: true; font.pixelSize: 12 }
                    TextField {
                        id: addDateField
                        Layout.fillWidth: true
                        placeholderText: "2026-08-01"
                        onTextChanged: errorText.visible = false
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    Text { text: "Show Time (HH:MM)"; font.bold: true; font.pixelSize: 12 }
                    TextField {
                        id: addTimeField
                        Layout.fillWidth: true
                        placeholderText: "18:30"
                        onTextChanged: errorText.visible = false
                    }
                }
            }

            Text {
                id: errorText
                visible: false
                color: "#C62828"
                font.pixelSize: 12
                font.bold: true
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Item { Layout.preferredHeight: 6 }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Item { Layout.fillWidth: true }

                CustomButton {
                    text: "Cancel"
                    implicitWidth: 90
                    implicitHeight: 34
                    btnColor: "#E5E5E5"
                    textColor: window.textDark
                    onClicked: {
                        addShowDialog.clearFields()
                        addShowDialog.close()
                    }
                }

                CustomButton {
                    text: "Add Show"
                    implicitWidth: 100
                    implicitHeight: 34
                    btnColor: window.primaryRed
                    textColor: "#FFFFFF"
                    onClicked: {
                        if (addMovieCombo.currentIndex < 0 || addScreenCombo.currentIndex < 0) {
                            errorText.text = "⚠️ Please select a movie and a screen."
                            errorText.visible = true
                            return
                        }

                        var selectedMovie = showManagementPage.ownTheatreMovies[addMovieCombo.currentIndex]
                        var selectedScreen = showManagementPage.ownTheatreScreens[addScreenCombo.currentIndex]
                        var screenId = selectedScreen.id

                        var dateParts = addDateField.text.trim().split("-")
                        var hourMinuteParts = addTimeField.text.trim().split(":")
                        if (dateParts.length !== 3 || hourMinuteParts.length !== 2) {
                            errorText.text = "⚠️ Enter date as YYYY-MM-DD and time as HH:MM"
                            errorText.visible = true
                            return
                        }

                        var year = parseInt(dateParts[0])
                        var month = parseInt(dateParts[1])
                        var day = parseInt(dateParts[2])
                        var hour = parseInt(hourMinuteParts[0])
                        var minute = parseInt(hourMinuteParts[1])
                        if (isNaN(year) || isNaN(month) || isNaN(day) || isNaN(hour) || isNaN(minute)) {
                            errorText.text = "⚠️ Date and time must contain valid numbers.";
                            errorText.visible = true;
                            return;
                        }
                        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
                            errorText.text = "⚠️ Time must be between 00:00 and 23:59.";
                            errorText.visible = true;
                            return;
                        }

                        if (controller.isShowTimeConflicting(selectedMovie.id, screenId, year, month, day, hour, minute)) {
                            errorText.text = "⚠️ Cannot add show as it conflicts with the time of another show!"
                            errorText.visible = true
                            return
                        }

                        if (controller.addShow(selectedMovie.id, screenId, year, month, day, hour, minute)) {
                            showManagementPage.refreshShows()
                            addShowDialog.clearFields()
                            addShowDialog.close()
                        } else {
                            errorText.text = "⚠️ Could not add show. Check movie/screen/date and time selection."
                            errorText.visible = true
                        }
                    }
                }
            }
        }
    }

    // 2. UPDATE SHOW TIME DIALOG
    Dialog {
        id: updateShowDialog
        title: "Update Show Time"
        anchors.centerIn: parent
        width: 380
        modal: true
        property string targetShowId: ""
        standardButtons: Dialog.NoButton

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            Text { text: "Show ID: " + updateShowDialog.targetShowId; font.bold: true }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    Text { text: "New Date (YYYY-MM-DD)"; font.bold: true; font.pixelSize: 12 }
                    TextField {
                        id: updateDateField
                        Layout.fillWidth: true
                        placeholderText: "2026-08-01"
                        onTextChanged: updateErrorText.visible = false
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4
                    Text { text: "New Time (HH:MM)"; font.bold: true; font.pixelSize: 12 }
                    TextField {
                        id: updateTimeField
                        Layout.fillWidth: true
                        placeholderText: "18:30"
                        onTextChanged: updateErrorText.visible = false
                    }
                }
            }

            Text {
                id: updateErrorText
                visible: false
                color: "#C62828"
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                Item { Layout.fillWidth: true }

                CustomButton {
                    text: "Cancel"
                    implicitWidth: 90; implicitHeight: 34
                    btnColor: "#E5E5E5"; textColor: window.textDark
                    onClicked: {
                        updateDateField.text = ""
                        updateTimeField.text = ""
                        updateErrorText.visible = false
                        updateShowDialog.close()
                    }
                }

                CustomButton {
                    text: "Update"
                    implicitWidth: 90; implicitHeight: 34
                    btnColor: window.primaryRed; textColor: "#FFFFFF"
                    onClicked: {
                        var dateStr = updateDateField.text.trim()
                        var timeStr = updateTimeField.text.trim()

                        var dateParts = dateStr.split("-")
                        var hourMinuteParts = timeStr.split(":")

                        // Validate string format splits
                        if (dateParts.length !== 3 || hourMinuteParts.length !== 2) {
                            updateErrorText.text = "⚠️ Enter date as YYYY-MM-DD and time as HH:MM"
                            updateErrorText.visible = true
                            return
                        }

                        var year = parseInt(dateParts[0], 10)
                        var month = parseInt(dateParts[1], 10)
                        var day = parseInt(dateParts[2], 10)
                        var hour = parseInt(hourMinuteParts[0], 10)
                        var minute = parseInt(hourMinuteParts[1], 10)

                        // Validate numeric inputs
                        if (isNaN(year) || isNaN(month) || isNaN(day) || isNaN(hour) || isNaN(minute)) {
                            updateErrorText.text = "⚠️ Date and time must contain valid numbers."
                            updateErrorText.visible = true
                            return
                        }

                        // Validate calendar date & hour bounds
                        if (typeof controller.isValidDate === "function" && !controller.isValidDate(year, month, day)) {
                            updateErrorText.text = "⚠️ Please enter a valid calendar date!"
                            updateErrorText.visible = true
                            return
                        }

                        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
                            updateErrorText.text = "⚠️ Time must be between 00:00 and 23:59."
                            updateErrorText.visible = true
                            return
                        }

                        // Call C++ updateShow(year, month, day, hour, minute, showId)
                        if (controller.updateShow(year, month, day, hour, minute, updateShowDialog.targetShowId)) {
                            showManagementPage.refreshShows()
                            updateDateField.text = ""
                            updateTimeField.text = ""
                            updateErrorText.visible = false
                            updateShowDialog.close()
                        } else {
                            updateErrorText.text = "⚠️ Could not update — show may have bookings or time conflicts."
                            updateErrorText.visible = true
                        }
                    }
                }
            }
        }
    }

    // 3. CANCEL SHOW DIALOG
    Dialog {
        id: cancelConfirmDialog
        title: "Confirm Cancellation"
        anchors.centerIn: parent
        width: 350
        modal: true
        property string targetShowId: ""
        standardButtons: Dialog.Yes | Dialog.No

        ColumnLayout {
            spacing: 10
            Text {
                text: "Are you sure you want to cancel Show ID: " + cancelConfirmDialog.targetShowId + "?"
                font.pixelSize: 13
                wrapMode: Text.WordWrap
            }
        }

        onAccepted: {
            controller.cancelShow(cancelConfirmDialog.targetShowId)
            showManagementPage.refreshShows()
        }
    }
}