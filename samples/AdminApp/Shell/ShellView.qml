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

        // Sidebar Region
        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "#1a202c"

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 8
                spacing: 4

                Repeater {
                    model: ListModel {
                        ListElement { name: "Dashboard"; viewId: "DashboardView"; icon: "\u2302" }
                        ListElement { name: "Orders"; viewId: "OrderListView"; icon: "\u2637" }
                        ListElement { name: "Settings"; viewId: "SettingsView"; icon: "\u2699" }
                    }

                    delegate: Rectangle {
                        Layout.fillWidth: true
                        Layout.leftMargin: 8
                        Layout.rightMargin: 8
                        height: 40
                        radius: 6
                        color: navMouse.containsMouse ? "#2d3748" :
                               (mainLoader.currentView === model.viewId ? "#4a5568" : "transparent")

                        MouseArea {
                            id: navMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                console.log("[Shell] Navigate to: " + model.viewId)
                                mainLoader.currentView = model.viewId

                                if (model.viewId === "SettingsView" && typeof moduleManager !== 'undefined') {
                                    moduleManager.LoadModule("SettingsModule")
                                }

                                if (regionManager) {
                                    regionManager.RequestNavigate("MainRegion", model.viewId, {})
                                }
                            }
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            spacing: 10

                            Label {
                                text: model.icon
                                color: "#a0aec0"
                                font.pixelSize: 16
                            }

                            Label {
                                text: model.name
                                color: mainLoader.currentView === model.viewId ? "white" : "#a0aec0"
                                font.pixelSize: 14
                                Layout.fillWidth: true
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

        // Main Region
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f0f4f8"

            Loader {
                id: mainLoader
                anchors.fill: parent

                property string currentView: "DashboardView"

                source: {
                    switch (currentView) {
                    case "DashboardView":
                        return "qrc:/qtprism/DashboardView.qml"
                    case "OrderListView":
                        return "qrc:/qtprism/OrderListView.qml"
                    case "OrderDetailView":
                        return "qrc:/qtprism/OrderDetailView.qml"
                    case "SettingsView":
                        return "qrc:/qtprism/SettingsView.qml"
                    default:
                        return "qrc:/qtprism/DashboardView.qml"
                    }
                }
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
                text: "Modules: 3"
                color: "#a0aec0"
                font.pixelSize: 11
                Layout.rightMargin: 12
            }
        }
    }
}
