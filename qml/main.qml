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

    Component.onCompleted: {
        // Connect to the event hooks
        eventHook.mouseEvent.connect(function(event) {
            logArea.append("Mouse Event: " + event.type + " at (" + event.x + ", " + event.y + ")");
        });

        eventHook.keyboardEvent.connect(function(event) {
            logArea.append("Keyboard Event: " + event.type + " key: " + event.key);
        });
    }

}