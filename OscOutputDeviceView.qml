import QtQuick
import QtQuick.Controls
import MotionGloveInterface

Item {
    id: root
//    required
    property string uniqueID: "oscout-1"
    implicitWidth:  120


    implicitHeight: contentrect.height

    property string ipAddress: "127.0.0.1"
    property int port: 55211

    OscOutputViewController {

        id: oscviewcontrol
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
                    text: "127.0.0.1"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight

                    validator: RegularExpressionValidator {
                        regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/

                    }
                    Keys.onReturnPressed: focus=false
                    Keys.onEscapePressed: {
                        text = root.ipAddress
                        focus = false
                    }
                    onEditingFinished: root.ipAddress = text
                    onFocusChanged: {
                        if(!focus) { text = root.ipAddress}
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
                    text: "55211"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight
                    //                    anchors.leftMargin: 273
                    validator: IntValidator {bottom: 1025;top: 65535}
                    Keys.onReturnPressed: focus=false
                    Keys.onEscapePressed: {
                        text=root.port
                        focus=false
                    }
                    onEditingFinished: root.port=text
                    onFocusChanged: {
                        if(!focus) { text = root.port}
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
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:3}D{i:5}D{i:4}D{i:7}D{i:9}D{i:8}D{i:2}
D{i:1}
}
##^##*/
