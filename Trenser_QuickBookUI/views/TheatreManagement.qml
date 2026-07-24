/*
 * File: TheatreManagement.qml
 * Description: Implements the theatre management interface in QML.
 *              Provides functionality for admins and theatre owners to
 *              view, add, edit, approve/reject, deactivate, and reactivate
 *              theatres. Integrates with the ControllerAdapter backend to
 *              fetch theatre data, manage assigned movies, and configure
 *              screens. Includes validation for mandatory fields, phone
 *              number format, and email format/uniqueness. Displays feedback
 *              banners for success or error states. Supports role-based
 *              actions:
 *              - Admin: Approve/Reject pending theatres.
 *              - Theatre Owner: Add new theatres, edit details, manage
 *                movies and screens, deactivate/reactivate theatres.
 *              Designed with responsive layouts, structured table views,
 *              and interactive dialogs for CRUD operations.
 * Author: Trenser
 * Created: 22 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: theatrePage

    property string searchQuery: ""
    property string statusFilter: "ALL"
    property string feedbackMessage: ""

    Component.onCompleted: {
        loadTheatresFromBackend();
    }

    function loadTheatresFromBackend() {
        theatreModel.clear();
        var theatres;
        if (controller.userType === EnumsAdapter.UserType.ADMIN) {
            theatres = controller.getAllTheatres();
        } else {
            theatres = controller.getMyTheatres();
        }
        for (var index = 0; index < theatres.length; index++) {
            theatreModel.append(theatres[index]);
        }
    }

    ListModel {
        id: theatreModel
    }

    ListModel {
        id: assignedMoviesModel
    }

    ListModel {
        id: availableMoviesModel
    }

    ListModel {
        id: screensModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // --- TOP HEADER BAR ---
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text { text: "Theatre Management"; font.pixelSize: 22; font.bold: true; color: window.textDark }
                Text { text: "Manage your theatres, screens, seat layouts, and movies"; font.pixelSize: 13; color: window.textMuted }
            }

            Item { Layout.fillWidth: true }

            CustomButton {
                text: "➕ Add Theatre"
                highlighted: true
                btnColor: window.primaryRed
                textColor: "#FFFFFF"
                visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER
                onClicked:
                {
                    addTheatreDialog.clearFields()
                    addTheatreDialog.open()
                }
            }
        }

        // --- FEEDBACK BANNER ---
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 36
            radius: 6
            color: "#E6F4EA"; border.color: "#A3E0B5"
            visible: theatrePage.feedbackMessage !== ""
            Text {
                anchors.centerIn: parent
                text: theatrePage.feedbackMessage
                font.pixelSize: 12; font.bold: true; color: "#137333"
            }
        }

        // --- FILTER BAR ---
        RowLayout {
            Layout.fillWidth: true; spacing: 15
            RowLayout {
                spacing: 5
                CustomButton {
                    text: "All"
                    implicitWidth: 110; implicitHeight: 36
                    btnColor: theatrePage.statusFilter === "ALL" ? window.primaryRed : "#E5E5E5"
                    textColor: theatrePage.statusFilter === "ALL" ? "#FFFFFF" : window.textDark
                    onClicked: {
                        theatrePage.statusFilter = "ALL"
                    }
                }

                CustomButton {
                    text: "Active"
                    implicitWidth: 110; implicitHeight: 36
                    btnColor: theatrePage.statusFilter === "ACTIVE" ? window.primaryRed : "#E5E5E5"
                    textColor: theatrePage.statusFilter === "ACTIVE" ? "#FFFFFF" : window.textDark
                    onClicked: {
                        theatrePage.statusFilter = "ACTIVE"
                    }
                }

                CustomButton {
                    text: "Inactive"
                    implicitWidth: 110; implicitHeight: 36
                    btnColor: theatrePage.statusFilter === "INACTIVE" ? window.primaryRed : "#E5E5E5"
                    textColor: theatrePage.statusFilter === "INACTIVE" ? "#FFFFFF" : window.textDark
                    onClicked: {
                        theatrePage.statusFilter = "INACTIVE"
                    }
                }
            }
            Item { Layout.fillWidth: true }
        }

        // --- MAIN THEATRE TABLE VIEW ---
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: window.cardBg; radius: 8; border.color: window.borderColor

            ColumnLayout {
                anchors.fill: parent; spacing: 0

                // Table Header
                // Table Header
                Rectangle {
                    Layout.fillWidth: true; implicitHeight: 40; color: "#F8F9FA"; border.color: window.borderColor

                    RowLayout {
                        anchors.fill: parent; anchors.leftMargin: 15; anchors.rightMargin: 15
                        spacing: 8

                        Text { text: "ID"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 60; Layout.minimumWidth: 60; Layout.maximumWidth: 60 }
                        Text { text: "Name"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 90; Layout.minimumWidth: 90; Layout.maximumWidth: 90 }
                        Text { text: "City"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 100; Layout.minimumWidth: 100; Layout.maximumWidth: 100 }
                        Text { text: "Address"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 100; Layout.minimumWidth: 100; Layout.maximumWidth: 100}
                        Text { text: "Contact"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 80; Layout.minimumWidth: 80; Layout.maximumWidth: 80 }
                        Text { text: "Status"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 80; Layout.minimumWidth: 80; Layout.maximumWidth: 80; horizontalAlignment: Text.AlignHCenter }
                        Text { text: "Actions"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 260; Layout.minimumWidth: 260; Layout.maximumWidth: 260; horizontalAlignment: Text.AlignHCenter }
                    }
                }

                // Table Body
                ListView {
                    id: theatreListView
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true
                    model: theatreModel

                    delegate: Rectangle {
                        width: theatreListView.width; implicitHeight: 52
                        color: index % 2 === 0 ? "#FFFFFF" : "#FAFBFD"
                        border.color: "#F0F0F0"

                        visible: {
                            var matchesSearch = model.name.toLowerCase().indexOf(theatrePage.searchQuery) !== -1;
                            var matchesFilter = (theatrePage.statusFilter === "ALL") || (model.status === theatrePage.statusFilter);
                            return matchesSearch && matchesFilter;
                        }
                        height: visible ? 52 : 0

                        RowLayout {
                            anchors.fill: parent; anchors.leftMargin: 15; anchors.rightMargin: 15
                            spacing: 8
                            Text { text: model.id; font.bold: true; color: window.textDark; Layout.preferredWidth: 60; Layout.minimumWidth: 60; Layout.maximumWidth: 60 }
                            Text { text: model.name; color: window.textDark; elide: Text.ElideRight; Layout.preferredWidth: 90; Layout.minimumWidth: 90; Layout.maximumWidth: 90 }
                            Text { text: model.city; color: window.textDark; elide: Text.ElideRight; Layout.preferredWidth: 100; Layout.minimumWidth: 100; Layout.maximumWidth: 100 }
                            Text { text: model.address; color: window.textDark; elide: Text.ElideRight; Layout.preferredWidth: 100; Layout.minimumWidth: 100; Layout.maximumWidth: 100 }
                            Text { text: model.phone; color: window.textDark; Layout.preferredWidth: 80; Layout.minimumWidth: 80; Layout.maximumWidth: 80 }
                            Item {
                                Layout.preferredWidth: 65; Layout.minimumWidth: 65; Layout.maximumWidth: 65; Layout.fillHeight: true
                                Rectangle {
                                    width: 65; height: 22; radius: 11
                                    anchors.centerIn: parent
                                    color: model.status === "ACTIVE" ? "#E6F4EA" : (model.status === "PENDING" ? "#FEF7E0" : "#FCE8E6")
                                    Text {
                                        anchors.centerIn: parent; text: model.status; font.pixelSize: 10; font.bold: true
                                        color: model.status === "ACTIVE" ? "#137333" : (model.status === "PENDING" ? "#B06000" : "#C5221F")
                                    }
                                }
                            }
                            RowLayout {
                                Layout.preferredWidth: 260; Layout.minimumWidth: 260; Layout.maximumWidth: 260; Layout.alignment: Qt.AlignLeft; spacing: 8

                                Item { Layout.fillWidth: true }
                                CustomButton {
                                    text: "✅ Approve"
                                    implicitWidth: 80; implicitHeight: 30
                                    btnColor: "#E3F2FD"; textColor: "#1565C0"
                                    visible: controller.userType === EnumsAdapter.UserType.ADMIN && model.status === "PENDING"
                                    onClicked: {
                                        if (controller.approveTheatre(model.id) === EnumsAdapter.ProcessStatus.SUCCESS) {
                                            theatreModel.setProperty(index, "status", "ACTIVE");
                                            theatrePage.feedbackMessage = model.name + " has been Approved!";
                                        }
                                        else
                                        {
                                            theatrePage.feedbackMessage = "Failed to approve Theatre " + model.name + "!";
                                        }
                                    }
                                }
                                CustomButton {
                                    text: "❌ Reject"
                                    implicitWidth: 80; implicitHeight: 30
                                    btnColor: "#FFEBEE"; textColor: "#C62828"
                                    visible: controller.userType === EnumsAdapter.UserType.ADMIN && model.status === "PENDING"
                                    onClicked: {
                                        if (controller.rejectTheatre(model.id) === EnumsAdapter.ProcessStatus.SUCCESS) {
                                            theatreModel.setProperty(index, "status", "INACTIVE");
                                            theatrePage.feedbackMessage = model.name + " has been Rejected.";
                                        }
                                        else
                                        {
                                            theatrePage.feedbackMessage = "Failed to reject Theatre " + model.name + "!";
                                        }
                                    }
                                }
                                CustomButton {
                                    text: "✏️ Edit"
                                    implicitWidth: 65; implicitHeight: 30
                                    btnColor: "#E3F2FD"; textColor: "#1565C0"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER
                                    onClicked: editTheatreDialog.openForTheatre(index)
                                    icon.width: 10
                                }
                                CustomButton {
                                    text: "🎬 Movies"
                                    implicitWidth: 75; implicitHeight: 30
                                    btnColor: window.primaryRed
                                    textColor: "#FFFFFF"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER && model.status === "ACTIVE"
                                    onClicked: moviesDialog.openForTheatre(model.id, model.name)
                                }
                                CustomButton {
                                    text: "🖥️ Screens"
                                    implicitWidth: 80; implicitHeight: 30
                                    btnColor: window.primaryRed
                                    textColor: "#FFFFFF"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER && model.status === "ACTIVE"
                                    onClicked: screensDialog.openForTheatre(model.id, model.name)
                                }
                                CustomButton {
                                    text: "Deactivate"
                                    implicitWidth: 80; implicitHeight: 30
                                    btnColor: "#FFEBEE"; textColor: "#C62828"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER && model.status === "ACTIVE"
                                    onClicked: {
                                        if (controller.deactivateTheatre(model.id) === EnumsAdapter.ProcessStatus.SUCCESS) {
                                            theatreModel.setProperty(index, "status", "INACTIVE");
                                            theatrePage.feedbackMessage = model.name + " deactivated successfully."
                                        } else {
                                            theatrePage.feedbackMessage = "Failed to deactivate " + model.name + "."
                                        }
                                    }
                                }

                                CustomButton {
                                    text: "Reactivate"
                                    implicitWidth: 80; implicitHeight: 30
                                    btnColor: "#E3F2FD"
                                    textColor: "#1565C0"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER && model.status === "INACTIVE"
                                    onClicked: {
                                        if (controller.reactivateTheatre(model.id) === EnumsAdapter.ProcessStatus.SUCCESS) {
                                            theatreModel.setProperty(index, "status", "PENDING");
                                            theatrePage.feedbackMessage = "Reactivation request sent to Admin for " + model.name + "."
                                        } else {
                                            theatrePage.feedbackMessage = "Failed to request reactivation."
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: addTheatreDialog
        title: "Add New Theatre"
        modal: true;
        anchors.centerIn: parent; width: 450
        function clearFields() {
            nameIn.text = "";
            cityIn.text = "";
            addrIn.text = "";
            phoneIn.text = "";
            emailIn.text = "";
            errorTxt.text = ""
        }
        ColumnLayout {
            width: parent.width; spacing: 12
            Text { id: errorTxt; color: "red"; font.pixelSize: 11; visible: text !== "" }
            TextField { id: nameIn; Layout.fillWidth: true; placeholderText: "Theatre Name"; selectByMouse: true }
            TextField { id: cityIn; Layout.fillWidth: true; placeholderText: "City"; selectByMouse: true }
            TextField { id: addrIn; Layout.fillWidth: true; placeholderText: "Address"; selectByMouse: true }
            TextField { id: phoneIn; Layout.fillWidth: true; placeholderText: "10-Digit Phone Number"; inputMethodHints: Qt.ImhDigitsOnly; selectByMouse: true }
            TextField { id: emailIn; Layout.fillWidth: true; placeholderText: "Email (e.g., example@domain.com)"; selectByMouse: true }
            RowLayout {
                Layout.fillWidth: true; Layout.topMargin: 10
                Item { Layout.fillWidth: true }
                Button { text: "Cancel"; onClicked: addTheatreDialog.close() }
                Button {
                    text: "Submit for Approval"; highlighted: true
                    onClicked: {
                        var phoneRegex = /^[0-9]{10}$/
                        var emailRegex = /^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/
                        var tName = nameIn.text.trim();
                        var tCity = cityIn.text.trim();
                        var tAddr = addrIn.text.trim();
                        var tPhone = phoneIn.text.trim();
                        var tEmail = emailIn.text.trim();
                        if (tName === "" || tCity === "" || tAddr === "") {
                            errorTxt.text = "All fields are required!";
                            return
                        }
                        if (!phoneRegex.test(tPhone)) {
                            errorTxt.text = "Phone number must be exactly 10 digits!";
                            return
                        }
                        if (!emailRegex.test(tEmail)) {
                            errorTxt.text = "Invalid email format!";
                            return
                        }
                        var resultStatus = controller.addTheatre(tName, tCity, tAddr, tPhone, tEmail);

                        if (resultStatus === EnumsAdapter.ProcessStatus.SUCCESS) {
                            theatrePage.feedbackMessage = "Theatre added! Approval request sent to admin."
                            addTheatreDialog.close()

                        } else if (resultStatus === EnumsAdapter.ProcessStatus.EMAIL_ALREADY_EXISTS) {
                            errorTxt.text = "Error: This Theatre Email is already in use.";
                        } else if (resultStatus === EnumsAdapter.ProcessStatus.PHONE_NUMBER_ALREADY_EXISTS) {
                            errorTxt.text = "Error: This Theatre Phone Number is already in use.";
                        } else if (resultStatus === EnumsAdapter.ProcessStatus.ALREADY_EXISTS) {
                            errorTxt.text = "Error: A theatre with this exact name, city, and address already exists.";
                        } else {
                            errorTxt.text = "Error: Failed to add theatre due to a backend issue.";
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: editTheatreDialog
        title: "Edit Theatre Details"
        modal: true
        anchors.centerIn: parent; width: 450
        property int targetIndex: -1
        property string targetTheatreId: ""
        property string originalPhone: ""
        property string originalEmail: ""
        function openForTheatre(index) {
            targetIndex = index
            var item = theatreModel.get(index)
            targetTheatreId = item.id
            nameInput.text = item.name || "";
            cityInput.text = item.city || "";
            addressInput.text = item.address || "";
            phoneInput.text = item.phone || "";
            emailInput.text = item.email || "";
            // Save original values for comparison
            originalPhone = phoneInput.text;
            originalEmail = emailInput.text;
            editErrorText.text = ""
            open()
        }
        ColumnLayout {
            width: parent.width; spacing: 12
            Text { id: editErrorText; color: "red"; font.pixelSize: 11; visible: text !== "" }

            TextField { id: nameInput; Layout.fillWidth: true; placeholderText: "Theatre Name"; selectByMouse: true }
            TextField { id: cityInput; Layout.fillWidth: true; placeholderText: "City"; selectByMouse: true }
            TextField { id: addressInput; Layout.fillWidth: true; placeholderText: "Address"; selectByMouse: true }
            TextField { id: phoneInput; Layout.fillWidth: true; placeholderText: "Phone Number"; inputMethodHints: Qt.ImhDigitsOnly; selectByMouse: true }
            TextField { id: emailInput; Layout.fillWidth: true; placeholderText: "Email (Optional)"; selectByMouse: true }
            RowLayout {
                Layout.fillWidth: true; Layout.topMargin: 10
                Item { Layout.fillWidth: true }

                Button { text: "Cancel"; onClicked: editTheatreDialog.close() }

                Button {
                    text: "Save Changes"; highlighted: true
                    onClicked: {
                        var phoneRegex = /^[0-9]{10}$/
                        var emailRegex = /^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/
                        var inputName = nameInput.text.trim();
                        var inputCity = cityInput.text.trim();
                        var inputAddr = addressInput.text.trim();
                        var inputPhone = phoneInput.text.trim();
                        var inputEmail = emailInput.text.trim();
                        if (inputName === "" || inputCity === "" || inputAddr === "") {
                            editErrorText.text = "Name, City, and Address are required!"; return
                        }
                        if (inputPhone !== "" && !phoneRegex.test(inputPhone)) {
                            editErrorText.text = "Phone number must be exactly 10 digits!"; return
                        }
                        if (inputEmail !== "" && !emailRegex.test(inputEmail)) {
                            editErrorText.text = "Invalid email format!"; return
                        }
                        var finalPhone = (inputPhone === editTheatreDialog.originalPhone) ? "" : inputPhone;
                        var finalEmail = (inputEmail === editTheatreDialog.originalEmail) ? "" : inputEmail;
                        var resultStatus = controller.updateTheatre(editTheatreDialog.targetTheatreId, inputName, inputCity, inputAddr, finalPhone, finalEmail)
                        if (resultStatus === EnumsAdapter.ProcessStatus.SUCCESS) {
                            theatreModel.setProperty(editTheatreDialog.targetIndex, "name", inputName)
                            theatreModel.setProperty(editTheatreDialog.targetIndex, "city", inputCity)
                            theatreModel.setProperty(editTheatreDialog.targetIndex, "address", inputAddr)
                            if (finalPhone !== "") theatreModel.setProperty(editTheatreDialog.targetIndex, "phone", finalPhone)
                            if (finalEmail !== "") theatreModel.setProperty(editTheatreDialog.targetIndex, "email", finalEmail)
                            theatrePage.feedbackMessage = "Theatre details updated successfully!"
                            editTheatreDialog.close()
                        } else if (resultStatus === EnumsAdapter.ProcessStatus.EMAIL_ALREADY_EXISTS) {
                            editErrorText.text = "Error: That Email is already taken by another theatre.";
                        } else if (resultStatus === EnumsAdapter.ProcessStatus.PHONE_NUMBER_ALREADY_EXISTS) {
                            editErrorText.text = "Error: That Phone Number is already in use.";
                        } else {
                            editErrorText.text = "Error: Failed to update theatre in database."
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: moviesDialog
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 450
        property string currentTheatreId: ""
        function openForTheatre(theatreId, theatreName) {
            title = "Manage Movies: " + theatreName
            currentTheatreId = theatreId
            moviesErrorText.text = ""
            refreshData()
            open()
        }

        // Refresh Movie List
        function refreshData() {
            assignedMoviesModel.clear()
            availableMoviesModel.clear()
            var assigned = controller.getMoviesInTheatre(currentTheatreId)
            for (var indexOne = 0; indexOne < assigned.length; indexOne++) {
                assignedMoviesModel.append(assigned[indexOne])
            }
            var available = controller.getActiveMovies()
            for (var indexTwo = 0; indexTwo < available.length; indexTwo++) {
                availableMoviesModel.append(available[indexTwo])
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            Text { id: moviesErrorText; color: "red"; font.pixelSize: 11; visible: text !== "" }

            Text { text: "Currently Assigned Movies"; font.bold: true; font.pixelSize: 14; color: window.textDark }

            //LIST OF ASSIGNED MOVIES
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#F8F9FA"
                border.color: "#E0E0E0"
                radius: 4

                ListView {
                    anchors.fill: parent
                    anchors.margins: 5
                    clip: true
                    model: assignedMoviesModel
                    spacing: 5

                    delegate: Rectangle {
                        width: ListView.view.width
                        implicitHeight: 60
                        color: "#FFFFFF"
                        border.color: "#F0F0F0"
                        radius: 4

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10

                            Text { text: model.title; font.bold: true; Layout.fillWidth: true; color: window.textDark }
                            Text { text: model.genre || ""; color: window.textMuted; Layout.preferredWidth: 80 }

                            CustomButton {
                                text: "Remove"
                                implicitWidth: 80; implicitHeight: 30
                                btnColor: "#FFEBEE"
                                textColor: "#C62828"
                                onClicked: {
                                    if (controller.removeMovieFromTheatre(moviesDialog.currentTheatreId, model.id) === EnumsAdapter.ProcessStatus.SUCCESS) {
                                        moviesErrorText.text = ""
                                        moviesDialog.refreshData()
                                    } else {
                                        moviesErrorText.text = "Failed to remove movie."
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Rectangle { Layout.fillWidth: true; height: 1; color: "#E0E0E0"; Layout.topMargin: 5; Layout.bottomMargin: 5 }
            Text { text: "Assign New Movie"; font.bold: true; font.pixelSize: 14; color: window.textDark }
            //DROPDOWN TO ADD MOVIES
            RowLayout {
                Layout.fillWidth: true

                ComboBox {
                    id: movieCombo
                    Layout.fillWidth: true
                    model: availableMoviesModel
                    textRole: "title"
                    valueRole: "id"
                }

                Button {
                    text: "Assign Movie"
                    highlighted: true
                    enabled: movieCombo.count > 0
                    onClicked: {
                        var selectedMovieId = movieCombo.currentValue
                        if (!selectedMovieId)
                        {
                            return;
                        }
                        for (var index = 0; index < assignedMoviesModel.count; index++) {
                            if (assignedMoviesModel.get(index).id === selectedMovieId) {
                                moviesErrorText.text = "Movie is already assigned to this theatre!"
                                return;
                            }
                        }

                        if (controller.addMovieToTheatre(moviesDialog.currentTheatreId, selectedMovieId) === EnumsAdapter.ProcessStatus.SUCCESS) {
                            moviesErrorText.text = ""
                            moviesDialog.refreshData()
                        } else {
                            moviesErrorText.text = "Failed to assign movie."
                        }
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Item { Layout.fillWidth: true }
                Button { text: "Close"; onClicked: moviesDialog.close() }
            }
        }
    }
    Dialog {
        id: screensDialog
        modal: true
        anchors.centerIn: parent
        width: 620
        height: 520
        property string currentTheatreId: ""
        property string editingScreenId: ""

        function openForTheatre(theatreId, theatreName) {
            title = "Manage Screens: " + theatreName
            currentTheatreId = theatreId
            screensErrorText.text = ""
            editingScreenId = ""
            clearInputFields()
            refreshData()
            open()
        }

        function clearInputFields() {
            newScreenName.text = ""
            newScreenRows.text = ""
            newScreenColumns.text = ""
            newScreenPrice.text = ""
        }

        function refreshData() {
            screensModel.clear()
            var screens = controller.getScreensInTheatre(currentTheatreId)
            for (var index = 0; index < screens.length; index++) {
                screensModel.append(screens[index])
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            Text { id: screensErrorText; color: "red"; font.pixelSize: 11; visible: text !== "" }
            Text { text: "Existing Screens"; font.bold: true; font.pixelSize: 14; color: window.textDark }
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#F8F9FA"
                border.color: "#E0E0E0"
                radius: 4

                ListView {
                    anchors.fill: parent
                    anchors.margins: 5
                    clip: true
                    model: screensModel
                    spacing: 6

                    delegate: Rectangle {
                        width: ListView.view.width
                        implicitHeight: 50
                        color: "#FFFFFF"
                        border.color: "#F0F0F0"
                        radius: 4

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 8
                            Loader {
                                Layout.fillWidth: true
                                sourceComponent: screensDialog.editingScreenId === model.id ? editNameComponent : displayNameComponent
                            }

                            Component {
                                id: displayNameComponent
                                RowLayout {
                                    Text { text: model.name; font.bold: true; color: window.textDark }
                                    CustomButton {
                                        text: "Edit"
                                        implicitWidth: 45; implicitHeight: 30
                                        btnColor: "#E3F2FD"; textColor: "#1565C0"
                                        flat: true
                                        onClicked: screensDialog.editingScreenId = model.id
                                    }
                                }
                            }

                            Component {
                                id: editNameComponent
                                RowLayout {
                                    TextField {
                                        id: renameInput
                                        text: model.name
                                        selectByMouse: true
                                        Layout.preferredWidth: 120
                                    }
                                    CustomButton {
                                        text: "Save"
                                        implicitWidth: 80; implicitHeight: 30
                                        btnColor: "#E3F2FD"; textColor: "#1565C0"
                                        highlighted: true
                                        onClicked: {
                                            var newName = renameInput.text.trim();
                                            if (newName === "") {
                                                screensErrorText.text = "Screen name cannot be empty."
                                                return
                                            }
                                            if (newName === model.name) {
                                                screensErrorText.text = ""
                                                screensDialog.editingScreenId = ""
                                                return
                                            }
                                            for (var index = 0; index < screensModel.count; index++) {
                                                var item = screensModel.get(index)
                                                if (item.id !== model.id && item.name.toLowerCase() === newName.toLowerCase()) {
                                                    screensErrorText.text = "Error: Screen with name '" + newName + "' already exists."
                                                    return
                                                }
                                            }
                                            var result = controller.updateScreenName(screensDialog.currentTheatreId, model.id, newName);
                                            if (result === EnumsAdapter.ProcessStatus.SUCCESS) {
                                                screensErrorText.text = ""
                                                screensDialog.editingScreenId = ""
                                                screensDialog.refreshData()
                                            } else if (result === EnumsAdapter.ProcessStatus.ALREADY_EXISTS) {
                                                screensErrorText.text = "Error: Screen with name '" + newName + "' already exists."
                                            } else {
                                                screensErrorText.text = "Failed to update screen name."
                                            }
                                        }
                                    }
                                    CustomButton {
                                        text: "Cancel"
                                        implicitWidth: 80; implicitHeight: 30
                                        btnColor: "#FFEBEE"; textColor: "#C62828"
                                        onClicked: {
                                            screensErrorText.text = ""
                                            screensDialog.editingScreenId = ""
                                        }
                                    }
                                }
                            }
                            Text {
                                text: model.rows + "x" + model.cols + " (" + model.capacity + " seats)";
                                color: window.textMuted
                                Layout.preferredWidth: 120
                                visible: screensDialog.editingScreenId !== model.id
                            }
                            Text {
                                text: "₹" + (model.price || 0).toFixed(2);
                                font.bold: true
                                color: "#137333"
                                Layout.preferredWidth: 80
                                visible: screensDialog.editingScreenId !== model.id
                            }
                            CustomButton {
                                text: "💺 Seats"
                                implicitWidth: 80; implicitHeight: 30
                                btnColor: window.primaryRed
                                textColor: "#FFFFFF"
                                visible: screensDialog.editingScreenId !== model.id
                                onClicked: {
                                    seatLayoutDialog.openForScreen(model.id, model.name)
                                }
                            }
                            CustomButton {
                                text: model.status === 0 ? "Deactivate" : "Reactivate"
                                implicitWidth: 80; implicitHeight: 30
                                btnColor: model.status === 0 ? "#FFEBEE" : "#E3F2FD"
                                textColor: model.status === 0 ? "#C62828" : "#1565C0"
                                onClicked: {
                                    var resultStatus = 0;
                                    if (model.status === 0) {
                                        resultStatus = controller.deactivateScreen(screensDialog.currentTheatreId, model.id);
                                    } else {
                                        resultStatus = controller.reactivateScreen(screensDialog.currentTheatreId, model.id);
                                    }

                                    if (resultStatus === EnumsAdapter.ProcessStatus.SUCCESS) {
                                        screensErrorText.text = ""
                                        screensDialog.refreshData()
                                    } else {
                                        screensErrorText.text = "Failed to update screen status."
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Rectangle { Layout.fillWidth: true; height: 1; color: "#E0E0E0"; Layout.topMargin: 2; Layout.bottomMargin: 2 }
            Text { text: "Add New Screen"; font.bold: true; font.pixelSize: 14; color: window.textDark }
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    TextField {
                        id: newScreenName
                        Layout.fillWidth: true
                        placeholderText: "Screen Name (e.g. Screen 1)"
                        selectByMouse: true
                    }
                    TextField {
                        id: newScreenPrice
                        Layout.preferredWidth: 130
                        placeholderText: "Price/Seat (₹)"
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        selectByMouse: true
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    TextField {
                        id: newScreenRows
                        Layout.fillWidth: true
                        placeholderText: "Rows (e.g. 10)"
                        inputMethodHints: Qt.ImhDigitsOnly
                        selectByMouse: true
                        maximumLength: 2
                        validator: IntValidator { bottom: 1; top: 20 }
                    }
                    TextField {
                        id: newScreenColumns
                        Layout.fillWidth: true
                        placeholderText: "Columns (e.g. 12)"
                        inputMethodHints: Qt.ImhDigitsOnly
                        selectByMouse: true
                        maximumLength: 2
                        validator: IntValidator { bottom: 1; top: 20 }
                    }
                    Button {
                        text: "Add Screen"
                        highlighted: true
                        onClicked: {
                            var screenName = newScreenName.text.trim();
                            var screenRows = parseInt(newScreenRows.text.trim());
                            var screenColumns = parseInt(newScreenColumns.text.trim());
                            var screenPrice = parseFloat(newScreenPrice.text.trim());
                            if (screenName === "" || isNaN(screenRows) || isNaN(screenColumns) || isNaN(screenPrice)) {
                                screensErrorText.text = "Please enter valid values for all fields."
                                return
                            }
                            if (screenRows <= 0 || screenColumns <= 0 || screenPrice <= 0) {
                                screensErrorText.text = "Rows, Columns, and Price must be greater than 0."
                                return
                            }
                            var result = controller.addScreenToTheatre(
                                screensDialog.currentTheatreId,
                                screenName,
                                screenRows,
                                screenColumns,
                                screenPrice
                            );
                            if (result === EnumsAdapter.ProcessStatus.SUCCESS) {
                                screensErrorText.text = ""
                                screensDialog.clearInputFields()
                                screensDialog.refreshData()
                            } else if (result === EnumsAdapter.ProcessStatus.ALREADY_EXISTS) {
                                screensErrorText.text = "Error: Screen with name '" + screenName + "' already exists."
                            } else if (result === EnumsAdapter.ProcessStatus.NOT_FOUND) {
                                screensErrorText.text = "Error: Theatre not found in database."
                            } else {
                                screensErrorText.text = "Error: Failed to add screen."
                            }
                        }
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Item { Layout.fillWidth: true }
                Button { text: "Close"; onClicked: screensDialog.close() }
            }
        }
    }
    Dialog {
        id: seatLayoutDialog
        modal: true
        anchors.centerIn: parent
        width: 720
        height: 580

        property string currentScreenId: ""
        property string currentScreenName: ""
        property var seatGridData: []

        function openForScreen(selectedScreenId, screenName) {
            currentScreenId = selectedScreenId
            currentScreenName = screenName
            title = "Seat Layout Management: " + screenName
            seatLayoutErrorText.text = ""
            refreshSeatLayout()
            open()
        }

        function refreshSeatLayout() {
            var flatSeatList = controller.getSeatLayout(currentScreenId)
            var groupedSeatMap = {}
            var rowOrderList = []

            for (var seatIndex = 0; seatIndex < flatSeatList.length; seatIndex++) {
                var seatObject = flatSeatList[seatIndex]
                if (!groupedSeatMap[seatObject.row]) {
                    groupedSeatMap[seatObject.row] = []
                    rowOrderList.push(seatObject.row)
                }
                groupedSeatMap[seatObject.row].push(seatObject)
            }

            var seatRowList = []
            for (var rowIndex = 0; rowIndex < rowOrderList.length; rowIndex++) {
                var rowKey = rowOrderList[rowIndex]
                seatRowList.push(groupedSeatMap[rowKey])
            }

            seatGridData = seatRowList
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 12

            Text {
                id: seatLayoutErrorText
                color: "red"
                font.pixelSize: 11
                visible: text !== ""
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                RowLayout {
                    spacing: 6
                    Rectangle { width: 18; height: 18; radius: 3; color: "#E6F4EA"; border.color: "#A3E0B5" }
                    Text { text: "Active (Available)"; font.pixelSize: 12; color: window.textDark }
                }

                RowLayout {
                    spacing: 6
                    Rectangle { width: 18; height: 18; radius: 3; color: "#FCE8E6"; border.color: "#F5C2C7" }
                    Text { text: "Inactive (Blocked)"; font.pixelSize: 12; color: window.textDark }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 24
                color: "#E8EAED"
                radius: 4

                Text {
                    anchors.centerIn: parent
                    text: "SCREEN THIS WAY"
                    font.pixelSize: 10
                    font.bold: true
                    color: window.textMuted
                }
            }

            Flickable {
                id: seatGridScrollView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                contentWidth: seatGridColumn.implicitWidth
                contentHeight: seatGridColumn.implicitHeight
                boundsBehavior: Flickable.StopAtBounds

                ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                Column {
                    id: seatGridColumn
                    spacing: 8

                    Repeater {
                        model: seatLayoutDialog.seatGridData

                        Row {
                            spacing: 8
                            property var seatRowData: modelData

                            Text {
                                text: (seatRowData && seatRowData.length > 0) ? seatRowData[0].row : ""
                                font.bold: true
                                font.pixelSize: 13
                                color: window.textDark
                                width: 24
                                height: 36
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Repeater {
                                model: seatRowData

                                Rectangle {
                                    property var seatObject: modelData
                                    property bool isBlocked: seatObject ? (seatObject.status === 3) : false

                                    width: 36
                                    height: 36
                                    radius: 6
                                    color: isBlocked ? "#FCE8E6" : "#E6F4EA"
                                    border.color: isBlocked ? "#F5C2C7" : "#A3E0B5"

                                    Text {
                                        anchors.centerIn: parent
                                        text: seatObject ? (seatObject.column + 1) : ""
                                        font.pixelSize: 11
                                        font.bold: true
                                        color: isBlocked ? "#C5221F" : "#137333"
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            var selectedSeatId = seatObject.id
                                            var processStatus = 0

                                            if (isBlocked) {
                                                processStatus = controller.reactivateSeat(seatLayoutDialog.currentScreenId, selectedSeatId)
                                            } else {
                                                processStatus = controller.deactivateSeat(seatLayoutDialog.currentScreenId, selectedSeatId)
                                            }

                                            if (processStatus === EnumsAdapter.ProcessStatus.SUCCESS) {
                                                seatLayoutErrorText.text = ""
                                                seatLayoutDialog.refreshSeatLayout()
                                            } else {
                                                if (isBlocked) {
                                                    seatLayoutErrorText.text = "Failed to reactivate seat " + selectedSeatId + "."
                                                } else {
                                                    seatLayoutErrorText.text = "Cannot deactivate seat " + selectedSeatId + ". It has active bookings in scheduled shows!"
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Item { Layout.fillWidth: true }
                Button {
                    text: "Close"
                    onClicked: {
                        seatLayoutDialog.close()
                    }
                }
            }
        }
    }
}