import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


ApplicationWindow {
    id: root
    visible: true
    width: 1200
    height: 700
    title: qsTr("Mouse/Keyboard Event Hook Demo")

    ColumnLayout {
        anchors.fill: parent
        spacing: 10


        Text {
            text: qsTr("This demo shows how to hook mouse and keyboard events globally.")
            font.pointSize: 14
        }

        TextArea {
            id: logArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            wrapMode: TextArea.WrapAnywhere
        }
    }

    // 2 Buttons to start/stop the event hook  and play events
    RowLayout {
        Layout.alignment: Qt.AlignHCenter
        spacing: 20
        Button {
            text: qsTr("Start Hook")
            onClicked: {
                EventAppEngine.startHooking();
                console.log("Event hook started.");
            }
        }
        Button {
            text: qsTr("Stop Hook")
            onClicked: {
                EventAppEngine.stopHooking();
                console.log("Event hook stopped.");
            }
        }

        Button {
            text: qsTr("Play Recorded Events")
            onClicked: {
                EventAppEngine.playRecordedEvents();
                console.log("Playing recorded events.");
            }
        }
    }
    Connections {
        target: EventAppEngine

    }

}