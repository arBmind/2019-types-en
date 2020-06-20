import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import SchemaGui 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Schema GUI")

    ListView {
        id: listView
        anchors.fill: parent

        model: ViewModel.model()
        delegate: Item {
            id: wrapper
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40

            property var entity: {
                var x = ViewModel.entity(display);
                console.log(x.toString(), x instanceof PersonDataView);
                return x;
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                Label { text: roleText(entity.role);
                    Layout.minimumWidth: 200; Layout.maximumWidth: 200
                    font.pixelSize: 22
                }
                Label { text: entity.name;
                    Layout.fillWidth: true;
                    font.pixelSize: 22
                }
            }
            MouseArea {
                anchors.fill: parent
                enabled: !wrapper.ListView.isCurrentItem
                onClicked: listView.currentIndex = index
            }
        }
        highlight: Rectangle { color: "lightsteelblue"; radius: 3 }
        highlightMoveDuration: 100
        focus: true
    }

    function roleText(role) {
        switch(role)      {
        case 0: return "Unknown";
        case 1: return "Teacher";
        case 2: return "Student";
        }
    }
}
