import QtQuick
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Layouts
import MotionGloveInterface
import QtQuick.Dialogs


Window {

    id: mainWindow
    width: 1024
    height: 768
//    x:2700//300
//    y:85//200
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
            objectName: "processingGraphView"
            property color bgColor: "#FFA8A8A8"
            color: dropa.containsDrag ? bgColor.lighter(1.1) :  bgColor

//                layer.enabled: true
//                layer.smooth: true
//                layer.samples: 4
//            antialiasing: true


            anchors {
                top: processingNodes.bottom
                left: inputNodes.right
                right: outputNodes.left
                bottom: mainItem.bottom
            }
            DropArea {
                id: dropa
                anchors.fill: parent
                keys: ["nodeType"]
//                onEntered: (drag) => {console.log("drag entered", drag.keys)}

                onDropped: (drop) => {
//                               console.log("dropkeys:", drop.keys)
//                               _mbackend.newNodeDropped(drop.getDataAsArrayBuffer())


                               var nodetype = drop.getDataAsString("nodeType");
                               var shouldAccept = false


                               if( nodetype === TypeHelper.Process.toString()) {
                                   shouldAccept = _mbackend.createProcessingNodeDrop(Qt.point(drop.x-40, drop.y-20),
                                                                                     drop.getDataAsString("processorType"))
                               }
                               else if(nodetype === TypeHelper.Output.toString()) {
                                   shouldAccept = _mbackend.createOutputNodeDrop(Qt.point(drop.x-30, drop.y-10),
                                                                 drop.getDataAsString("targetDevice"),
                                                                 drop.getDataAsString("outputIndex"),
                                                                               drop.getDataAsString("valueType"))
                               }
                               else if(nodetype === TypeHelper.Input.toString()) {
                                   shouldAccept = _mbackend.createInputNodeDrop(Qt.point(drop.x-30, drop.y-10),
                                                                 drop.getDataAsString("sourceDevice"),
                                                                 drop.getDataAsString("identifier"),
                                                                 drop.getDataAsString("valueType"))
                               }

                               if(shouldAccept) {
                                   drop.acceptProposedAction()
                               }

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
            Text {
                id: windowCoordinats
                text: "%1  -  %2".arg(mainWindow.x).arg(mainWindow.y)
                x: 20; y: 20
            }
            Button {
                id: clearScenenButton
                onClicked: _mbackend.clearScene()
                text: "Clear Scene"
                anchors {
                    horizontalCenter: inputControl.horizontalCenter
                    top: inputControl.top
                    topMargin: 7
                }
            }
            Button {
                id: autoDetectButton
                text: "auto detect devices"
                anchors {
                    horizontalCenter: inputControl.horizontalCenter
                    bottom: inputControl.bottom
                }
                checkable: true
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
               id: saveAsButton
               text: "save as"
               anchors {
                   top: parent.top
                   right: parent.right
                   margins: 10
               }

               onClicked: saveFileDialog.open()//console.log(_mbackend.createUniqueId(TypeHelper.Input))

           }
           Button {
               id :loadButton
               text: "load file"
               anchors {
                   right: parent.right
                   top: saveAsButton.bottom
                   margins: 10
               }
               onClicked: loadFileDialog.open()
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
                objectName: "outputDevicesView"
                anchors{
                    top: parent.top
                    left: parent.left
                    right:parent.right
                    margins: 5
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
    FileDialog {
        id: saveFileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: _mbackend.saveAsButtonPressed(selectedFile)
//        selectedFileChanged: console.log("selected file ", selectedFile)
//        nameFilters: ["XML File(*.xml)"]
    }
    FileDialog {
        id: loadFileDialog
        fileMode: FileDialog.OpenFile
         nameFilters: ["XML files (*.xml)"]
        onAccepted: _mbackend.loadButtonPressed(selectedFile)
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:3}D{i:4}D{i:7}D{i:9}D{i:10}D{i:1}
}
##^##*/
