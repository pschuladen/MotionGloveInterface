import QtQuick
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Layouts
import MotionGloveInterface


Window {

    id: mainWindow
    width: 1024
    height: 768
    x:300
    y:200
    visible: true
    title: qsTr("Motion Glove Interface")
    color: "black"

    Component.onCompleted: console.log(TypeHelper.Accel)

    Item {
        id: mainItem
        objectName: "mainRootItem"
        anchors.fill: parent
        property real topBarHeight: 90
        property real leftBarWidth: 160
        property real rightBarWidth: 160

        Rectangle {
            id: processingGraph
            objectName: "processingGraphView"
            property color bgColor: "#FFA8A8A8"
            color: dropa.containsDrag ? bgColor.lighter(1.1) :  bgColor

            anchors {
                top: processingNodes.bottom
                left: inputNodes.right
                right: outputNodes.left
                bottom: mainItem.bottom
            }
            property int tN: 4
            onTNChanged: console.log("tn changed", tN)
            DropArea {
                id: dropa
                anchors.fill: parent
                keys: ["text/procType"]
                onEntered: (drag) => {console.log("drag entered", drag.keys)}
//                onContainsDragChanged: console.log("drag entered", drag.formats)

                onDropped: (drop) => {
                               drop.acceptProposedAction()
                               console.log("bla", drop.keys)
//                               console.log("Dropped Dataaa", drop.getDataAsString("text/proceName"))
//                               console.log("Dropped source", drop.source)
//                               console.log("Dropped action", drop.action)
//                               console.log("Dropped formats", drop.formats)
                               console.log("Position", Qt.point(drop.x, drop.y))
//                               console.log("funktioniert dieses enum", ProcessNodeController.ViewOnly)
                               _mbackend.createNewProcessingView(drop.getDataAsString("text/procType"), Qt.point(drop.x, drop.y))
                               parent.tN = drop.getDataAsString("text/procType")
                }
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
           Button {
               text: "click"
anchors.verticalCenter: outputControl.verticalCenter
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
            ColumnLayout {
                id: outputNodesRow
                anchors{
                    top: parent.top
                    left: parent.left
                    right:parent.right
                    margins: 5
                }
                height: childrenRect.height


                OscOutputDeviceView {
                    Layout.fillWidth: true
                }
            }
            Button {
                text: "create Osc-Out"
                anchors {
                    horizontalCenter: outputNodesRow.horizontalCenter
                    top: outputNodesRow.bottom
                }
                onClicked: _mbackend.createOscOutputDevice()

            }

        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:3}D{i:4}D{i:7}D{i:9}D{i:10}D{i:1}
}
##^##*/
