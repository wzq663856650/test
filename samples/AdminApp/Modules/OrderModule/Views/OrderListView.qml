import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#f0f4f8"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 12

        Label {
            text: "Order List"
            font.pixelSize: 28
            font.bold: true
            color: "#1a202c"
        }

        Label {
            text: "Click an order to view details"
            font.pixelSize: 14
            color: "#718096"
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 8

            model: ListModel {
                ListElement { orderId: 1; customerName: "Alice Johnson"; total: 299.99; status: "Completed" }
                ListElement { orderId: 2; customerName: "Bob Smith"; total: 149.50; status: "Processing" }
                ListElement { orderId: 3; customerName: "Carol White"; total: 89.00; status: "Shipped" }
                ListElement { orderId: 4; customerName: "David Brown"; total: 450.00; status: "Completed" }
                ListElement { orderId: 5; customerName: "Eve Davis"; total: 35.99; status: "Pending" }
            }

            delegate: Rectangle {
                width: ListView.view.width
                height: 60
                radius: 8
                color: mouseArea.containsMouse ? "#edf2f7" : "white"
                border.color: "#e2e8f0"

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        console.log("[QML] Order clicked: " + model.orderId)
                        if (regionManager) {
                            var params = { "orderId": model.orderId }
                            regionManager.RequestNavigate("MainRegion", "OrderDetailView", params)
                        }
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 16

                    Label {
                        text: "#" + model.orderId
                        font.pixelSize: 14
                        font.bold: true
                        color: "#4299e1"
                        Layout.preferredWidth: 40
                    }

                    Label {
                        text: model.customerName
                        font.pixelSize: 14
                        color: "#2d3748"
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "$" + model.total.toFixed(2)
                        font.pixelSize: 14
                        font.bold: true
                        color: "#2d3748"
                    }

                    Rectangle {
                        width: 80
                        height: 24
                        radius: 12
                        color: model.status === "Completed" ? "#c6f6d5" :
                               model.status === "Processing" ? "#bee3f8" :
                               model.status === "Shipped" ? "#fefcbf" : "#fed7d7"

                        Label {
                            anchors.centerIn: parent
                            text: model.status
                            font.pixelSize: 11
                            color: model.status === "Completed" ? "#276749" :
                                   model.status === "Processing" ? "#2b6cb0" :
                                   model.status === "Shipped" ? "#975a16" : "#c53030"
                        }
                    }
                }
            }
        }
    }
}
