/*
 * File: SystemLogs.qml
 * Description: Implements the system logs and audit trail interface in QML.
 *              Provides functionality to monitor system activities, application
 *              errors, and diagnostic events. Supports filtering by log type
 *              (ALL, SYSTEM_ACTIVITY, ERROR_LOG, UNKNOWN) and searching log
 *              messages or IDs. Integrates with the ControllerAdapter backend
 *              to fetch logs dynamically and refresh on demand. Displays logs
 *              in a console-style table with timestamp, log ID, type, and
 *              details. Includes empty-state handling when no logs are available.
 *              Designed with a dark theme console view, responsive layouts,
 *              and consistent styling aligned with the application’s UI palette.
 * Author: Trenser
 * Created: 23 July 2026
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: systemLogsPage

    property string currentTypeFilter: "ALL" // "ALL", "SYSTEM_ACTIVITY", "ERROR_LOG", "UNKNOWN"
    property string searchQuery: ""
    property var logsData: []

    function refreshLogs() {
        if (systemLogsPage.currentTypeFilter === "ALL") {
            var combined = []
            combined = combined.concat(controller.getLog("SYSTEM_ACTIVITY"))
            combined = combined.concat(controller.getLog("ERROR_LOG"))
            combined = combined.concat(controller.getLog("UNKNOWN"))
            systemLogsPage.logsData = combined
        } else {
            systemLogsPage.logsData = controller.getLog(systemLogsPage.currentTypeFilter)
        }
    }

    Component.onCompleted: refreshLogs()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 25
        spacing: 20

        // 1. TOP HEADER BAR
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                spacing: 2
                Text {
                    text: "System Logs & Audit Trail"
                    font.pixelSize: 22
                    font.bold: true
                    color: window.textDark
                }
                Text {
                    text: "Monitor system activities, application errors, and diagnostic events"
                    font.pixelSize: 13
                    color: window.textMuted
                }
            }

            Item { Layout.fillWidth: true } // Spacer

            CustomButton {
                text: "⟳ Refresh"
                implicitWidth: 100
                btnColor: "#E5E5E5"
                textColor: window.textDark
                onClicked: systemLogsPage.refreshLogs()
            }
        }

        // 2. SEARCH & LOG TYPE FILTER TABS
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
                        Layout.fillWidth: true
                        placeholderText: "Search log messages..."
                        background: Item {}
                        onTextChanged: systemLogsPage.searchQuery = text.toLowerCase()
                    }
                }
            }

            Item { Layout.fillWidth: true } // Spacer

            RowLayout {
                spacing: 8

                CustomButton {
                    text: "All Logs"
                    implicitWidth: 80; implicitHeight: 34
                    btnColor: systemLogsPage.currentTypeFilter === "ALL" ? window.primaryRed : "#E5E5E5"
                    textColor: systemLogsPage.currentTypeFilter === "ALL" ? "#FFFFFF" : window.textDark
                    onClicked: { systemLogsPage.currentTypeFilter = "ALL"; systemLogsPage.refreshLogs() }
                }

                CustomButton {
                    text: "System Activity"
                    implicitWidth: 125; implicitHeight: 34
                    btnColor: systemLogsPage.currentTypeFilter === "SYSTEM_ACTIVITY" ? window.primaryRed : "#E5E5E5"
                    textColor: systemLogsPage.currentTypeFilter === "SYSTEM_ACTIVITY" ? "#FFFFFF" : window.textDark
                    onClicked: { systemLogsPage.currentTypeFilter = "SYSTEM_ACTIVITY"; systemLogsPage.refreshLogs() }
                }

                CustomButton {
                    text: "Error Logs"
                    implicitWidth: 100; implicitHeight: 34
                    btnColor: systemLogsPage.currentTypeFilter === "ERROR_LOG" ? window.primaryRed : "#E5E5E5"
                    textColor: systemLogsPage.currentTypeFilter === "ERROR_LOG" ? "#FFFFFF" : window.textDark
                    onClicked: { systemLogsPage.currentTypeFilter = "ERROR_LOG"; systemLogsPage.refreshLogs() }
                }

                CustomButton {
                    text: "Unknown"
                    implicitWidth: 90; implicitHeight: 34
                    btnColor: systemLogsPage.currentTypeFilter === "UNKNOWN" ? window.primaryRed : "#E5E5E5"
                    textColor: systemLogsPage.currentTypeFilter === "UNKNOWN" ? "#FFFFFF" : window.textDark
                    onClicked: { systemLogsPage.currentTypeFilter = "UNKNOWN"; systemLogsPage.refreshLogs() }
                }
            }
        }

        // ---------------------------------------------------------------------
        // 3. LOG CONSOLE TABLE DISPLAY
        // ---------------------------------------------------------------------
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1E1E1E"
            radius: 8
            border.color: "#333333"

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "#2D2D2D"
                    radius: 8

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 15
                        anchors.rightMargin: 15
                        spacing: 10

                        Text { text: "TIMESTAMP"; font.bold: true; font.pixelSize: 11; color: "#AAAAAA"; Layout.preferredWidth: 160 }
                        Text { text: "LOG ID"; font.bold: true; font.pixelSize: 11; color: "#AAAAAA"; Layout.preferredWidth: 90 }
                        Text { text: "TYPE"; font.bold: true; font.pixelSize: 11; color: "#AAAAAA"; Layout.preferredWidth: 140 }
                        Text { text: "LOG MESSAGE / DETAILS"; font.bold: true; font.pixelSize: 11; color: "#AAAAAA"; Layout.fillWidth: true }
                    }
                }

                ListView {
                    id: logListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: systemLogsPage.logsData

                    delegate: Rectangle {
                        width: logListView.width
                        height: visible ? 45 : 0
                        color: index % 2 === 0 ? "#1E1E1E" : "#252526"

                        visible: {
                            var query = systemLogsPage.searchQuery
                            if (query === "") return true
                            return (modelData.details && modelData.details.toLowerCase().indexOf(query) !== -1) ||
                                   (modelData.logId && modelData.logId.toLowerCase().indexOf(query) !== -1)
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 15
                            anchors.rightMargin: 15
                            spacing: 10

                            Text {
                                text: modelData.timeStamp
                                font.family: "Monospace"
                                font.pixelSize: 12
                                color: "#858585"
                                Layout.preferredWidth: 160
                            }

                            Text {
                                text: modelData.logId
                                font.family: "Monospace"
                                font.bold: true
                                font.pixelSize: 12
                                color: "#CCCCCC"
                                Layout.preferredWidth: 90
                            }

                            Item {
                                Layout.preferredWidth: 140
                                Rectangle {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 125; height: 20; radius: 4
                                    color: modelData.type === "SYSTEM_ACTIVITY" ? "#1B3A2B" : (modelData.type === "ERROR_LOG" ? "#4A1D1D" : "#3D3219")
                                    Text {
                                        anchors.centerIn: parent
                                        text: modelData.type
                                        font.pixelSize: 10
                                        font.bold: true
                                        color: modelData.type === "SYSTEM_ACTIVITY" ? "#4CAF50" : (modelData.type === "ERROR_LOG" ? "#F44336" : "#FFC107")
                                    }
                                }
                            }

                            Text {
                                text: modelData.details
                                font.family: "Monospace"
                                font.pixelSize: 12
                                color: "#D4D4D4"
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }
                    }

                    Item {
                        anchors.centerIn: parent
                        visible: logListView.count === 0

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 8
                            Text {
                                text: "📋"
                                font.pixelSize: 32
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: "NO LOGS AVAILABLE"
                                font.pixelSize: 13
                                font.bold: true
                                color: "#858585"
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }
        }
    }
}