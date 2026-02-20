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

    property string currentInput: ""
    property string prevInput: ""

    function appendLog(msg) {
        logArea.text = logArea.text + msg + "\n"
    }

    RowLayout {
        anchors.fill: parent
        spacing: 16
        anchors.margins: 12

        ColumnLayout {
            Layout.preferredWidth: parent.width * 0.6
            spacing: 10

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#f7f7f7"
                radius: 8
                border.color: "#cccccc"
                border.width: 1

                TextArea {
                    id: logArea
                    anchors.fill: parent
                    anchors.margins: 8
                    readOnly: true
                    wrapMode: TextArea.WrapAnywhere
                    font.pointSize: 12
                }
            }
        }

        ColumnLayout {
            Layout.preferredWidth: parent.width * 0.35
            spacing: 12

            Rectangle {
                Layout.fillWidth: true
                height: 80
                color: "#222831"
                radius: 8
                border.color: "#393e46"
                border.width: 1

                Text {
                    id: display
                    text: root.currentInput.length > 0 ? root.currentInput : "0"
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.pixelSize: 36
                }
            }

            TextField {
                id: inputField
                Layout.fillWidth: true
                text: root.currentInput
                height: 40
                onAccepted: {
                    root.appendLog("Input accepted: " + text)
                }
            }

            GridLayout {
                columns: 3
                rowSpacing: 8
                columnSpacing: 8
                Layout.fillWidth: true

                Button {
                    text: "1"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: {
                        root.currentInput += "1";
                        root.appendLog("Pressed: 1")
                    }
                }
                Button {
                    text: "2"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: {
                        root.currentInput += "2";
                        root.appendLog("Pressed: 2")
                    }
                }
                Button {
                    text: "3"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: {
                        root.currentInput += "3";
                        root.appendLog("Pressed: 3")
                    }
                }

                Button {
                    text: "4"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: {
                        root.currentInput += "4";
                        root.appendLog("Pressed: 4")
                    }
                }
                Button {
                    text: "5"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: {
                        root.currentInput += "5";
                        root.appendLog("Pressed: 5")
                    }
                }
                Button {
                    text: "6"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: { root.currentInput += "6";
                        root.appendLog("Pressed: 6")
                    }
                }

                Button {
                    text: "7"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: { root.currentInput += "7";
                        root.appendLog("Pressed: 7")
                    }
                }
                Button {
                    text: "8"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: { root.currentInput += "8";
                        root.appendLog("PRessed: 8")
                    }
                }
                Button {
                    text: "9"
                    font.pixelSize: 20
                    width: parent ? parent.width / 3 - 8 : 80
                    height: 64
                    onClicked: { root.currentInput += "9";
                        root.appendLog("Pressed: 9")
                    }
                }

            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12

                Button {
                    text: qsTr("Start Hook")
                    onClicked: {
                        EventAppEngine.startHooking();
                        root.appendLog("Event hook started.")
                    }
                }
                Button {
                    text: qsTr("Stop Hook")
                    onClicked: {
                        EventAppEngine.stopHooking();
                        root.appendLog("Event hook stopped.")
                    }
                }
                Button {
                    text: qsTr("Play Recorded Events")
                    onClicked: {
                        inputField.forceActiveFocus()
                        EventAppEngine.playRecordedEvents();
                        root.appendLog("Playing recorded events.")
                    }
                }
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12
                Switch {
                    id: loopSwitch
                    text: qsTr("Loop Playback")
                    onCheckedChanged: {
                        EventAppEngine.setLoopPlayback(checked);
                        root.appendLog("Loop playback " + (checked ? "enabled" : "disabled") + ".");
                    }
                }
            }

            RowLayout
            {
                Layout.alignment: Qt.AlignHCenter
                spacing: 12
                Button {
                    text: qsTr("Stop Playback")
                    onClicked: {
                        EventAppEngine.stopPlayback();
                        root.appendLog("Playback stopped.")
                    }
                }
                Button {
                    id: clearAll
                    text: qsTr("Clear All")
                    onClicked: {
                        EventAppEngine.clearEvents();
                        logArea.text = "";
                        root.currentInput = "";
                        root.appendLog("Cleared all logs and input.")
                    }
                }
            }
        }
    }

    Connections {
        target: EventAppEngine

    }

}