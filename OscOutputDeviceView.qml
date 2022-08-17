import QtQuick
import QtQuick.Controls
import MotionGloveInterface

Item {
    id: root
//    required
    property string uniqueID: "oscout-1"
    implicitWidth:  140


    implicitHeight: contentrect.height

//    property string ipAddress: "127.0.0.1"
//    property int port: 55211

    OscOutputViewController {

        id: oscviewcontrol
        uniqueID: root.uniqueID
    }

    Rectangle {
        id: contentrect
        color: "dodgerblue"
        width: root.width
        height: childrenRect.height+childrenRect.y

        border {
            color: "burlywood"
            width: 1
        }

        Rectangle {
            width: 8
            height: 8
            anchors {
                right: parent.right
                top: parent.top
            }
            color:  "red"
            opacity: 0.5

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onClicked: contextMenu.popup()
            }
        }

        Column {
            id: settingcolumne
            width: parent.width
            height: childrenRect.height
            spacing: 2

            Item {
                anchors {
                    left: parent.left; right:parent.right
                }
                height: 4
            }

            Text {
                text: "devicename"
                anchors.left: settingcolumne.left
                anchors.leftMargin: 3
            }

            Text {
                anchors.right: settingcolumne.right
                anchors.rightMargin: 4
                text: "ip-Addr"
                color: "white"
                font.italic: true
            }

            Rectangle {
                color: "white"
                height: ipinputfield.implicitHeight + 4
                width: 95
                border {color: "black"; width: 1}
                anchors.right: settingcolumne.right
                anchors.rightMargin: 4

                TextInput {
                    id: ipinputfield
                    anchors {fill: parent; margins: 2 }
                    color: "black"
                    text: oscviewcontrol.destIp
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight

                    validator: RegularExpressionValidator {
                        regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/

                    }
                    Keys.onReturnPressed: focus=false
                    Keys.onEscapePressed: {
                        text: oscviewcontrol.destIp
                        focus = false
                    }
                    onEditingFinished: oscviewcontrol.sig_changeIp(text)
                    onFocusChanged: {
                        if(!focus) { text: oscviewcontrol.destIp}
                    }
                    selectByMouse: true


                }
            }

            Text {
                anchors.right: settingcolumne.right
                anchors.rightMargin: 4
                text: "port"
                color: "white"
                font.italic: true
            }

            Rectangle {
                color: "white"
                border {color: "black"; width: 1}
                height: portinputfield.implicitHeight + 4
                anchors.right: settingcolumne.right
                anchors.rightMargin: 3
                width: 45

                TextInput {
                    id: portinputfield
                    anchors.fill: parent
                    anchors.margins: 2
                    color: "black"
                    text: oscviewcontrol.destPort
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight
                    //                    anchors.leftMargin: 273
                    validator: IntValidator {bottom: 1025;top: 65535}
                    Keys.onReturnPressed: focus=false
                    Keys.onEscapePressed: {
                        text: oscviewcontrol.destPort
                        focus=false
                    }
                    onEditingFinished: oscviewcontrol.sig_changePort(text)
                    onFocusChanged: {
                        if(!focus) { text: oscviewcontrol.destPort}
                    }
                    selectByMouse: true

                }
            }


        }
        Column {
            id: outputPathsColumn
            anchors {
                left: parent.left
                right: parent.right
                top: settingcolumne.bottom
                topMargin: 10
            }

            Repeater {
                model: oscviewcontrol.nPaths
                delegate:
                    OscOutputView {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    controller: oscviewcontrol
                    viewIdx: model.index
                    uniqueID: root.uniqueID
                }
            }
        }

        Button {
            text: "add path"
            anchors.top: outputPathsColumn.bottom
            anchors.right: parent.right
            onClicked: oscviewcontrol.sig_addOscPath();
        }
    }

    Menu {
        width: 110
        id: contextMenu
        MenuItem {text: "delete device"
            onClicked: _mbackend.deleteOscOutputDeviceWithId(root.uniqueID)
        }
    }

}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:3}D{i:5}D{i:4}D{i:7}D{i:9}D{i:8}D{i:2}
D{i:1}
}
##^##*/
