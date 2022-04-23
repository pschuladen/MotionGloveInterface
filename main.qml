import QtQuick
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Layouts

import MotionGloveInterface

ApplicationWindow {

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
            id: inputControl
            //record button? etc..
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

//                SensorInputContainer {

//                }
//                SensorInputContainer {

//                }
                }




//                    anchors {
//                        top: parent.top
//                        left: parent.left
//                        right: parent.right
//                    }

//                    Repeater {
//                        id: testV
//                        model: 30
//                        delegate:
//                            SensorValuesView{
//                            viewmode: ValueViewBackend.Grav
//                        }
//                    }

            }

            //fill with discovered input device
            //
        }
        Rectangle {
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
        //        Rectangle {
        //            color: "cyan"
        //            height: viewColu.implicitHeight+20// viewColu.implicitHeight
        //            width: 120//viewColu.implicitWidth


        //            Column {
        //            id:viewColu
        //            width: childrenRect.width
        //            height: childrenRect.height
        ////            SensorValuesView {
        ////                                viewmode: ValueViewBackend.Quat
        ////                            }
        ////            SensorValuesView {
        ////                                viewmode: ValueViewBackend.Accel
        ////                            }
        ////            SensorValuesView {
        ////                                viewmode: ValueViewBackend.Gyro
        ////                            }
        ////            SensorValuesView {
        ////                                viewmode: ValueViewBackend.Grav
        ////                            }
        //            Repeater {
        //                model: 4
        //                delegate: SensorValuesView {
        //                    viewmode: ValueViewBackend.Quat
        //                }
        //            }
        //            }
        //        }

        //        SensorValuesView {
        //            viewmode: ValueViewBackend.Quat
        //        anchors.centerIn: parent
        //        }
        //        QuatView {
        //            anchors.centerIn: parent
        //        }


        //        StackLayout {
        //            id: upperArea
        //            anchors {
        //                left: parent.left
        //                right: parent.right
        //                top: parent.top
        //            }
        //            height: 100
        //            currentIndex: upperTabs.currentIndex


        //            Rectangle {

        //                id: discoveredDevicesArea

        //                color: "lightgrey"
        //                border.width: 2
        //                border.color: "grey"

        //                Row {

        //                    property string test: "I am here"
        //                    id: discoveredDevices
        //                    objectName: "discoveredDevicesView"
        //                    anchors.fill: parent
        //                    anchors.margins: 10
        //                    spacing: 10

        //                    Button {
        //                        id: butvis
        //                        checkable: true
        //                        checked: true
        //                    }
        //                }
        //            }
        //            CreateProcessNodeWindow {
        //            }
        //        }

        //        ScrollView {
        //            id: mainScrollView
        //            FlexScrollViewLayout{}
        //            anchors {
        //                right: parent.right
        //                left: parent.left
        //                bottom: parent.bottom
        //                top: upperArea.bottom
        //            }
        //            contentHeight: sensorInput.height
        //            contentWidth: sensorInput.width

        //            Rectangle {
        //                onChildrenChanged: {console.log("chicha")
        //                    if(children.length < 2)
        //                        return

        //                    var obj = children[children.length-1]
        //                    obj.y = childrenRect.height
        //                }
        //                onChildrenRectChanged: console.log("chirect", childrenRect)

        //                Component.onCompleted: console.log(childrenRect)

        //                objectName: "sensorInputView"
        //                width:  childrenRect.width + childrenRect.x + 10
        //                height: childrenRect.height + childrenRect.y + 10
        //                id: sensorInput
        //                visible: butvis.checked
        //            }
        //        }


        //        TabBar {
        //            id: upperTabs
        //            anchors {
        //                horizontalCenter: upperArea.horizontalCenter
        //                top: upperArea.bottom
        //            }
        //            width: 500
        //            TabButton {
        //                text: "Devices"
        //            }
        //            TabButton {
        //                text: "processing"
        //            }
        //            TabButton {
        //                text: "setting"
        //            }
        //            TabButton {
        //                text: "audio"
        //            }
        //            currentIndex: 1

        //        }
    }
}

// test dropping
//        Rectangle {
//            color: "cyan"
//            width: 100
//            height: 100
//            //        z: 1
//            anchors {
//                centerIn: mainScrollView
//            }
//            DropArea {
//                anchors.fill: parent
//                enabled: true
//                //                    keys: ["text/plain"]
//                onDropped: (drop) => {
//                               drop.acceptProposedAction()
//                               console.log("Dropped", drop.text)
//                               console.log("Dropped source", drop.source)
//                               console.log("Dropped action", drop.action)
//                               console.log("Dropped formats", drop.formats)
//                               console.log("Droppen keys", drop.keys)
//                               console.log("Dropped Data", drop.getDataAsString("text/tt"))
//                           }
//                onEntered:(drag) => {
//                              drag.accepted = true
//                              console.log("droparea entered", drag.keys)
//                          }
//            }
//        }


// image test failed
//Image {
//    id: testIma
//    source: "//images/testPic.png" //"/Users/psch/qtProjects/MotionGloveInterface/pics/testPic.png"
//    width: 100
//    height: 100
//    Component.onCompleted: console.log("completet", sourceSize)
//}
