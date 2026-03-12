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
            text: "Dashboard"
            font.pixelSize: 28
            font.bold: true
            color: "#1a202c"
        }

        Label {
            text: "Welcome to QtPrism Admin Application"
            font.pixelSize: 16
            color: "#718096"
        }

        // Stats cards
        Row {
            spacing: 16
            Layout.fillWidth: true

            Repeater {
                model: [
                    { title: "Total Orders", value: "1,284", color: "#4299e1" },
                    { title: "Revenue", value: "$48,500", color: "#48bb78" },
                    { title: "Users", value: "573", color: "#ed8936" }
                ]

                Rectangle {
                    width: 180
                    height: 100
                    radius: 8
                    color: "white"
                    border.color: "#e2e8f0"

                    Column {
                        anchors.centerIn: parent
                        spacing: 8

                        Label {
                            text: modelData.title
                            font.pixelSize: 12
                            color: "#a0aec0"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Label {
                            text: modelData.value
                            font.pixelSize: 24
                            font.bold: true
                            color: modelData.color
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
