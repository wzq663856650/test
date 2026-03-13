import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#f0f4f8"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        Label {
            text: "Settings"
            font.pixelSize: 28
            font.bold: true
            color: "#1a202c"
        }

        Label {
            text: "This module was loaded on-demand"
            font.pixelSize: 14
            color: "#718096"
        }

        Rectangle {
            Layout.fillWidth: true
            height: 160
            radius: 12
            color: "white"
            border.color: "#e2e8f0"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 16

                RowLayout {
                    spacing: 16
                    Label { text: "Dark Mode"; font.pixelSize: 14; color: "#2d3748"; Layout.fillWidth: true }
                    Switch { checked: false }
                }

                RowLayout {
                    spacing: 16
                    Label { text: "Notifications"; font.pixelSize: 14; color: "#2d3748"; Layout.fillWidth: true }
                    Switch { checked: true }
                }

                RowLayout {
                    spacing: 16
                    Label { text: "Language"; font.pixelSize: 14; color: "#2d3748"; Layout.fillWidth: true }
                    ComboBox {
                        model: ["English", "Chinese", "Japanese"]
                        currentIndex: 0
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
