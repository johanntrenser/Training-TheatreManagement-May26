/*
 * File: MovieManagement.qml
 * Description: Implements the movie management interface in QML.
 *              Provides functionality to add, update, search, activate,
 *              and deactivate movies. Integrates with the ControllerAdapter
 *              backend to fetch all movies, validate uniqueness, and persist
 *              changes. Supports filtering by status (All, Active, Inactive)
 *              and searching by movie title. Displays movies in a grid view
 *              with structured cards showing ID, title, language, genre,
 *              duration, and status badges. Includes a form dialog for
 *              adding or editing movies with validation for mandatory fields
 *              and duration constraints (60–300 minutes). Designed with
 *              responsive layouts, consistent styling, and interactive
 *              controls aligned with the application’s UI palette.
 * Author: Trenser
 * Created: 23 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: movieManagementPage

    Component.onCompleted: {
        controller.initialize()
        refreshMovies()
    }

    ListModel {
        id: movieModel
    }

    function refreshMovies()
    {
        movieModel.clear()
        var movies = controller.getAllMovies()
        for (var index = 0; index < movies.length; index++)
            movieModel.append(movies[index])
    }

    property string currentFilter: "All"
    property string searchQuery: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "Movie Management"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: "Add, update, search, activate, or deactivate movies"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true }

            CustomButton {
                text: "+ Add Movie"
                btnColor: window.primaryRed
                implicitWidth: 130
                onClicked: {
                    movieFormDialog.isEditMode = false

                    mTitleField.text = ""
                    mLangField.text = ""
                    mGenreField.text = ""
                    mDurationField.text = ""

                    movieFormDialog.open()
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 15

            Rectangle {
                Layout.preferredWidth: 320
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
                        id: searchInput
                        Layout.fillWidth: true
                        placeholderText: "Search movie by title..."
                        background: Item {}
                        onTextChanged: movieManagementPage.searchQuery = text.toLowerCase()
                    }
                }
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                spacing: 8
                Repeater {
                    model: ["All", "Active", "Inactive"]
                    CustomButton {
                        text: modelData
                        implicitWidth: 85
                        implicitHeight: 32
                        btnColor: movieManagementPage.currentFilter === modelData ? window.primaryRed : "#E5E5E5"
                        textColor: movieManagementPage.currentFilter === modelData ? "#FFFFFF" : window.textDark
                        onClicked: movieManagementPage.currentFilter = modelData
                    }
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            GridView {
                id: movieGrid
                anchors.fill: parent
                cellWidth: 260
                cellHeight: 280
                model: movieModel

                delegate: Item {
                    width: 240
                    height: 260

                    visible: {
                        var matchesFilter = (movieManagementPage.currentFilter === "All") ||
                                            (movieManagementPage.currentFilter === "Active" && model.active) ||
                                            (movieManagementPage.currentFilter === "Inactive" && !model.active)
                        var matchesSearch = model.title.toLowerCase().indexOf(movieManagementPage.searchQuery) !== -1
                        return matchesFilter && matchesSearch
                    }

                    implicitWidth: visible ? 240 : 0
                    implicitHeight: visible ? 260 : 0

                    Rectangle {
                        anchors.fill: parent
                        color: window.cardBg
                        radius: 8
                        border.color: window.borderColor

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 110
                                color: model.active ? "#F0F0F0" : "#FCE8E6"
                                radius: 6

                                ColumnLayout {
                                    anchors.centerIn: parent
                                    spacing: 4
                                    Text {
                                        text: "🎬 " + model.movieId
                                        font.pixelSize: 14
                                        font.bold: true
                                        color: window.textMuted
                                        Layout.alignment: Qt.AlignHCenter
                                    }
                                    Rectangle {
                                        Layout.alignment: Qt.AlignHCenter
                                        width: 70; height: 20; radius: 10
                                        color: model.active ? "#E6F4EA" : "#FDE8E8"
                                        Text {
                                            anchors.centerIn: parent
                                            text: model.active ? "Active" : "Inactive"
                                            font.pixelSize: 10
                                            font.bold: true
                                            color: model.active ? "#137333" : "#C5221F"
                                        }
                                    }
                                }
                            }

                            Text {
                                text: model.title
                                font.pixelSize: 15
                                font.bold: true
                                color: window.textDark
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Text {
                                text: model.language + " • " + model.genre
                                font.pixelSize: 12
                                color: window.textMuted
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Text {
                                text: "⏱ " + model.duration + " mins"
                                font.pixelSize: 12
                                color: window.textDark
                            }

                            Item { Layout.fillHeight: true }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                CustomButton {
                                    text: "Edit"
                                    Layout.fillWidth: true
                                    implicitHeight: 32
                                    btnColor: "#E0E0E0"
                                    textColor: window.textDark
                                    onClicked: {

                                        movieFormDialog.isEditMode = true

                                        movieFormDialog.movieId = model.movieId

                                        movieFormDialog.originalTitle = model.title
                                        movieFormDialog.originalLanguage = model.language
                                        movieFormDialog.originalGenre = model.genre
                                        movieFormDialog.originalDuration = model.duration

                                        movieFormDialog.titleInput = model.title
                                        movieFormDialog.langInput = model.language
                                        movieFormDialog.genreInput = model.genre
                                        movieFormDialog.durationInput = model.duration.toString()

                                        movieFormDialog.open()
                                    }
                                }

                                CustomButton {
                                    text: model.active ? "Deactivate" : "Activate"
                                    Layout.fillWidth: true
                                    implicitHeight: 32
                                    btnColor: model.active ? "#D32F2F" : "#2E7D32"
                                    textColor: "#FFFFFF"
                                    onClicked: {
                                        var status
                                        if (model.active)
                                        {
                                            status = controller.deactivateMovie(model.movieId)
                                        }
                                        else
                                        {
                                            status = controller.reactivateMovie(model.movieId)
                                        }
                                        switch (status)
                                        {
                                        case EnumsAdapter.ProcessStatus.SUCCESS:
                                            refreshMovies()
                                            break

                                        default:
                                            console.log("Unable to update movie status.")
                                            break
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
        id: movieFormDialog

        property bool isEditMode: false

        property string movieId: ""

        property string originalTitle: ""
        property string originalLanguage: ""
        property string originalGenre: ""
        property int originalDuration: 0

        property alias titleInput: mTitleField.text
        property alias langInput: mLangField.text
        property alias genreInput: mGenreField.text
        property alias durationInput: mDurationField.text

        title: isEditMode ? "Update Movie Details" : "Add New Movie"
        anchors.centerIn: parent
        width: 400
        modal: true
        focus: true

        background: Rectangle {
            color: window.cardBg
            radius: 8
            border.color: window.borderColor
        }

        contentItem: ColumnLayout {
            spacing: 12

            Text {
                text: movieFormDialog.isEditMode ? "Edit Movie Details" : "Add New Movie"
                font.pixelSize: 18
                font.bold: true
                color: window.textDark
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Movie Title"; font.pixelSize: 12; color: window.textMuted }
                TextField { id: mTitleField; Layout.fillWidth: true; placeholderText: "e.g. Inception" }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Language"; font.pixelSize: 12; color: window.textMuted }
                TextField { id: mLangField; Layout.fillWidth: true; placeholderText: "e.g. English, Hindi" }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Genre"; font.pixelSize: 12; color: window.textMuted }
                TextField { id: mGenreField; Layout.fillWidth: true; placeholderText: "e.g. Action / Sci-Fi" }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text { text: "Duration (in minutes)"; font.pixelSize: 12; color: window.textMuted }
                TextField {
                    id: mDurationField
                    Layout.fillWidth: true
                    placeholderText: "e.g. 150"
                    inputMethodHints: Qt.ImhDigitsOnly
                }
            }

            Item { Layout.preferredHeight: 10 }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                CustomButton {
                    text: "Cancel"
                    btnColor: "#E0E0E0"
                    textColor: window.textDark
                    Layout.fillWidth: true
                    onClicked: {
                        movieFormDialog.isEditMode = false
                        movieFormDialog.close()
                    }
                }

                CustomButton {
                    text: movieFormDialog.isEditMode ? "Update" : "Add Movie"
                    btnColor: window.primaryRed
                    textColor: "#FFFFFF"
                    Layout.fillWidth: true

                    onClicked: {

                        if (movieFormDialog.isEditMode)
                        {
                            if (mTitleField.text.trim() === "" ||
                                mLangField.text.trim() === "" ||
                                mGenreField.text.trim() === "" ||
                                mDurationField.text.trim() === "")
                            {
                                console.log("All fields are required.")
                                return
                            }

                            var updatedDuration = parseInt(mDurationField.text, 10)

                            if (isNaN(updatedDuration))
                            {
                                console.log("Invalid duration.")
                                return
                            }

                            if (updatedDuration < 60 || updatedDuration > 300)
                            {
                                console.log("Duration should be between 60 and 300 minutes.")
                                return
                            }

                            var updateStatus = controller.updateMovie(
                                        movieFormDialog.movieId,

                                        movieFormDialog.originalTitle,
                                        movieFormDialog.originalLanguage,
                                        movieFormDialog.originalGenre,
                                        movieFormDialog.originalDuration,

                                        mTitleField.text.trim(),
                                        mLangField.text.trim(),
                                        mGenreField.text.trim(),
                                        updatedDuration)

                            switch(updateStatus)
                            {
                            case EnumsAdapter.ProcessStatus.SUCCESS:

                                refreshMovies()

                                mTitleField.text = ""
                                mLangField.text = ""
                                mGenreField.text = ""
                                mDurationField.text = ""

                                movieFormDialog.isEditMode = false

                                movieFormDialog.close()

                                break

                            case EnumsAdapter.ProcessStatus.ALREADY_EXISTS:

                                console.log("Movie already exists.")

                                break

                            default:

                                console.log("Movie update failed.")

                                break
                            }

                            return
                        }

                        if (mTitleField.text.trim() === "" ||
                            mLangField.text.trim() === "" ||
                            mGenreField.text.trim() === "" ||
                            mDurationField.text.trim() === "") {

                            console.log("All fields are required.")
                            return
                        }

                        var duration = parseInt(mDurationField.text, 10)

                        if (isNaN(duration)) {
                            console.log("Invalid duration.")
                            return
                        }

                        if (duration < 60 || duration > 300) {
                            console.log("Duration should be between 60 and 300 minutes.")
                            return
                        }

                        var status = controller.isMovieUnique(
                                        mTitleField.text.trim(),
                                        mLangField.text.trim(),
                                        mGenreField.text.trim(),
                                        duration)

                        if (status !== EnumsAdapter.ProcessStatus.SUCCESS) {

                            switch (status)
                            {
                            case EnumsAdapter.ProcessStatus.ALREADY_EXISTS:
                                console.log("Movie already exists.")
                                break

                            default:
                                console.log("Unable to validate movie.")
                                break
                            }
                            return
                        }

                        status = controller.addMovie(
                                    mTitleField.text.trim(),
                                    mLangField.text.trim(),
                                    mGenreField.text.trim(),
                                    duration)

                        switch (status)
                        {
                        case EnumsAdapter.ProcessStatus.SUCCESS:

                            refreshMovies()

                            mTitleField.text = ""
                            mLangField.text = ""
                            mGenreField.text = ""
                            mDurationField.text = ""

                            movieFormDialog.close()
                            break

                        case EnumsAdapter.ProcessStatus.ALREADY_EXISTS:
                            console.log("Movie already exists.")
                            break

                        default:
                            console.log("Failed to add movie.")
                            break
                        }
                    }
                }
            }
        }
    }
}