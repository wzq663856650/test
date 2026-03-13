import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: window
    visible: true
    width: 960
    height: 640
    title: "QtPrism Admin - Sample Application"
    color: "#f7fafc"

    header: ToolBar {
        height: 48
        background: Rectangle { color: "#2d3748" }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            Label {
                text: "QtPrism Admin"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "v1.0.0"
                color: "#a0aec0"
                font.pixelSize: 12
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar — driven entirely by shellController.navigationItems
        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "#1a202c"

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 8
                spacing: 4

                Repeater {
                    model: shellController ? shellController.navigationItems : []

                    delegate: Rectangle {
                        Layout.fillWidth: true
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        height: 40
                        radius: 6
                        opacity: modelData.available ? 1.0 : 0.4
                        color: navMouse.containsMouse ? "#2d3748" :
                               (shellController && shellController.currentViewName === modelData.viewId
                                   ? "#4a5568" : "transparent")

                        MouseArea {
                            id: navMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (shellController) {
                                    shellController.navigateTo(modelData.viewId)
                                }
                            }
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            spacing: 10

                            Label {
                                text: modelData.icon
                                color: "#a0aec0"
                                font.pixelSize: 16
                            }

                            Label {
                                text: modelData.name
                                color: shellController
                                       && shellController.currentViewName === modelData.viewId
                                           ? "white" : "#a0aec0"
                                font.pixelSize: 14
                                Layout.fillWidth: true
                            }

                            Label {
                                visible: !modelData.available && modelData.isOnDemand
                                text: "[load]"
                                color: "#718096"
                                font.pixelSize: 10
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8
                    Layout.bottomMargin: 8
                    height: 1
                    color: "#4a5568"
                }

                Label {
                    text: "Powered by QtPrism"
                    color: "#4a5568"
                    font.pixelSize: 10
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 12
                }
            }
        }

        // Main content — Loader source from ViewRegistry via ShellController
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f0f4f8"

            Loader {
                id: mainLoader
                anchors.fill: parent
                source: shellController ? shellController.currentViewUrl : ""
            }

            // Placeholder when no view is loaded
            Label {
                anchors.centerIn: parent
                visible: !mainLoader.source || mainLoader.source == ""
                text: "No modules loaded.\nPlease check your plugins directory."
                color: "#a0aec0"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    footer: ToolBar {
        height: 28
        background: Rectangle { color: "#edf2f7"; border.color: "#e2e8f0"; border.width: 1 }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12

            Label {
                text: "Ready"
                color: "#718096"
                font.pixelSize: 11
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Region: MainRegion"
                color: "#a0aec0"
                font.pixelSize: 11
            }

            Rectangle { width: 1; height: 14; color: "#cbd5e0" }

            Label {
                text: shellController
                      ? "Modules: " + shellController.loadedModuleCount
                      : "Modules: 0"
                color: "#a0aec0"
                font.pixelSize: 11
                Layout.rightMargin: 12
            }
        }
    }
}
