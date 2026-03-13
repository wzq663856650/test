import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#f0f4f8"

    property int orderId: 0
    property string customerName: ""
    property double orderTotal: 0
    property string orderStatus: ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        RowLayout {
            spacing: 12

            Button {
                text: "< Back"
                flat: true
                onClicked: {
                    if (regionManager) {
                        regionManager.RequestNavigate("MainRegion", "OrderListView", {})
                    }
                }
            }

            Label {
                text: "Order Detail"
                font.pixelSize: 28
                font.bold: true
                color: "#1a202c"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 200
            radius: 12
            color: "white"
            border.color: "#e2e8f0"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 12

                Label {
                    text: "Order #" + root.orderId
                    font.pixelSize: 20
                    font.bold: true
                    color: "#2d3748"
                }

                GridLayout {
                    columns: 2
                    columnSpacing: 24
                    rowSpacing: 8

                    Label { text: "Customer:"; color: "#718096"; font.pixelSize: 14 }
                    Label { text: root.customerName; color: "#2d3748"; font.pixelSize: 14; font.bold: true }

                    Label { text: "Total:"; color: "#718096"; font.pixelSize: 14 }
                    Label { text: "$" + root.orderTotal.toFixed(2); color: "#48bb78"; font.pixelSize: 14; font.bold: true }

                    Label { text: "Status:"; color: "#718096"; font.pixelSize: 14 }
                    Label { text: root.orderStatus; color: "#4299e1"; font.pixelSize: 14; font.bold: true }
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
