import QtQuick
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Layouts


Window {

    id: mainWindow
    width: 1024
    height: 768
    x:300
    y:200
    visible: true
    title: qsTr("Motion Glove Interface")
    color: "black"

    Item {
        id: mainItem
        objectName: "mainRootItem"
        anchors.fill: parent
        property real topBarHeight: 90
        property real leftBarWidth: 160
        property real rightBarWidth: 160

        Rectangle {
            id: processingGraph
            color: "#FFA8A8A8"

            anchors {
                top: processingNodes.bottom
                left: inputNodes.right
                right: outputNodes.left
                bottom: mainItem.bottom
            }
        }

        Rectangle {
            //record button? etc.., global input mute

            id: inputControl
            color: "#FFA29B85"
            height: mainItem.topBarHeight
            width: mainItem.leftBarWidth
            anchors {
                left: mainItem.left
                top: mainItem.top
            }
            border {
                width: 1
                color: "black"
            }
        }
        Rectangle {
            // inputdevices are created in here by the mainbackend.
            // option for custom inputs?
            id: inputNodes
            color: "#FF84A186"
            width: mainItem.leftBarWidth
            anchors {
                top: inputControl.bottom
                left: mainItem.left
                bottom: mainItem.bottom
            }
            border {
                width: 1
                color: "black"
            }
            ScrollView {
                id: inputNodesScollView
                anchors.fill: parent
                anchors {
                    topMargin: 1
                    bottomMargin: 1
                }
                Column {
                    id: testCo
                    objectName: "inputDevicesView"
                    anchors {
                        left: parent.left
                        leftMargin: 7
                        top: parent.top
                        topMargin: 7
                    }
                }
            }
        }
        Rectangle {
            // a list of all possible processing nodes goes here
            // drag and drop it to the processingGraph to create nodes.
            id: processingNodes
            color: "#FF797979"

            height: mainItem.topBarHeight
            anchors {
                left: inputControl.right
                right: outputControl.left
                top: mainItem.top
            }
            border {
                width: 1
                color: "black"
            }
            CreateProcessNodeWindow {
                anchors.fill: parent
            }
        }
        Rectangle {
            //mastervolume, audiosettings
            id: outputControl
            color: "#FFA1849A"
            height: mainItem.topBarHeight
            width: mainItem.rightBarWidth
            anchors {
                top: mainItem.top
                right: mainItem.right
            }
            border {
                width: 1
                color: "black"
            }
        }
        Rectangle {
            // create OSC-outputs and instruments here
            id: outputNodes
            color: "#FFA18784"
            width: mainItem.rightBarWidth
            anchors {
                right: mainItem.right
                top: outputControl.bottom
                bottom: mainItem.bottom
            }
            border {
                width: 1
                color: "black"
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:3}D{i:4}D{i:7}D{i:9}D{i:10}D{i:1}
}
##^##*/
