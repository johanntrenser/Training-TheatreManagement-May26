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

    // 2. Empty UI Model (Filled by the function above)
    ListModel {
        id: theatreModel
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

            Button {
                text: "➕ Add Theatre"
                highlighted: true
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
                Button { text: "All"; flat: theatrePage.statusFilter !== "ALL"; onClicked: theatrePage.statusFilter = "ALL" }
                Button { text: "Active"; flat: theatrePage.statusFilter !== "ACTIVE"; onClicked: theatrePage.statusFilter = "ACTIVE" }
                Button { text: "Inactive"; flat: theatrePage.statusFilter !== "INACTIVE"; onClicked: theatrePage.statusFilter = "INACTIVE" }
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
                        Text { text: "Actions"; font.bold: true; color: window.textMuted; Layout.preferredWidth: 260; Layout.minimumWidth: 260; Layout.maximumWidth: 260; horizontalAlignment: Text.AlignRight }
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
                                Layout.preferredWidth: 260; Layout.minimumWidth: 260; Layout.maximumWidth: 260; Layout.alignment: Qt.AlignLeft; spacing: 4

                                Item { Layout.fillWidth: true }
                                Button {
                                    text: "✅ Approve"
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
                                Button {
                                    text: "❌ Reject"
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
                                Button {
                                    text: "✏️"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER
                                    onClicked: editTheatreDialog.openForTheatre(index)
                                    icon.width: 10
                                }
                                Button {
                                    text: "🎬 Movies"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER && model.status === "ACTIVE"
                                    onClicked: console.log("Movies Dialog coming!")
                                }
                                Button {
                                    text: "🖥️ Screens"
                                    visible: controller.userType === EnumsAdapter.UserType.THEATRE_OWNER && model.status === "ACTIVE"
                                    onClicked: console.log("Screens Dialog coming!")
                                }
                                Button {
                                    text: "Deactivate"
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

                                Button {
                                    text: "Reactivate"
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
}