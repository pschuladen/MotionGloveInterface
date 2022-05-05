import QtQuick
import QtQuick.Controls

Item {
    id: root

    implicitHeight: settingcolumne.implicitHeight + 5
    onHeightChanged: console.log("root rect height", height)

    Rectangle {
        id: contentrect
        color: "dodgerblue"
        width: root.width
        height: settingcolumne.implicitHeight + 5
        onHeightChanged: console.log("content rect height", height)

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
            //                Rectangle {
            //                    width: ipinputfield.implicitWidth
            //                    color: "white"
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
                    text: "254.254.254.254"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight
//                    anchors.leftMargin: 415
                    validator: RegularExpressionValidator {
                        regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }
                    //                    }
                }
            }


            //                width: parent.width
            //                height: portinputfield.implicitHeight
            //                id: portrow
            Text {
                anchors.right: settingcolumne.right
                anchors.rightMargin: 4
                text: "port"
                color: "white"
                font.italic: true
            }
            //                Rectangle {
            //                    width: ipinputfield.implicitWidth
            //                    color: "white"
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
                    text: "65432"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight
//                    anchors.leftMargin: 273
                    validator: IntValidator {bottom: 1025;top: 65536}
                    //                    }
                }
            }
            Button {
                text: "add path"
                anchors.right: parent.right
            }

            //                TextInput {
            //                    id: portinputfield
            //                    anchors.right: portrow.right
            //                    color: "white"
            //                    text: "65000"
            //                    //                    }
            //                }
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:3}D{i:5}D{i:4}D{i:7}D{i:9}D{i:8}D{i:2}
D{i:1}
}
##^##*/
