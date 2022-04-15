import QtQuick
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Layouts

//import MotionGloveInterface

Window {
    id: mainWindow
    width: 640
    height: 480
    x:300
    y:200
    visible: true
    title: qsTr("Motion Glove Interface")
    color: "black"



    //    flags: Qt.Popup

    Rectangle {

        id: discoveredDevicesArea
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 100
        color: "lightgrey"
        border.width: 2
        border.color: "grey"

        Row {
//             Component.onCompleted: console.log(childrenRect)
            property string test: "I am here"
            id: discoveredDevices
            objectName: "discoveredDevicesView"
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

//            VectorView {}

//            DeviceStatusView {
//                deviceName: "hans"
//                //                Layout.preferredHeight: 80
//                //                Layout.preferredWidth: 120
//            }
            Button {
                id: butvis
                checkable: true
                checked: true
//                onCheckedChanged: sensorInput.visible

            }



        }
    }





    ScrollView {
        id: mainScrollView
        FlexScrollViewLayout{}
        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
            top: discoveredDevicesArea.bottom
        }
        contentHeight: sensorInput.height
        contentWidth: sensorInput.width
//        contentWidth: sensorInput.contentWidth
//        contentHeight: sensorInput.contentHeight
//        width: sensorInput.implicitWidth
//        height: sensorInput.implicitHeight
//        background: Rectangle{color: "cyan"}

        Rectangle {
//            scale: scaleSlider.value
            onChildrenChanged: {console.log("chicha")
                if(children.length < 2)
                    return
//                for(var i = 1) {

//                }
                var obj = children[children.length-1]
                obj.y = childrenRect.height
            }
            onChildrenRectChanged: console.log("chirect", childrenRect)

//boundsBehavior: Flickable.StopAtBounds
//            onWidthChanged: console.log(childrenRect)
            Component.onCompleted: console.log(childrenRect)


            objectName: "sensorInputView"
//contentWidth: childrenRect.width + childrenRect.x + 10
//contentHeight: childrenRect.height + childrenRect.y + 10
            width:  childrenRect.width + childrenRect.x + 10
            height: childrenRect.height + childrenRect.y + 10
            id: sensorInput
            visible: butvis.checked


        }
    }
    Button {
        anchors {
            horizontalCenter: discoveredDevicesArea.horizontalCenter
            top: discoveredDevicesArea.bottom
        }
        text: "add proc"
    }


    Rectangle {
        color: "red"
        width: 40
        height: 40
        anchors {
            centerIn: mainScrollView
        }
        MouseArea {
            anchors.fill: parent
           onReleased: console.log("red released in red")
           hoverEnabled: true

           preventStealing: true
        }
//        DropArea {
//            anchors.fill: parent
//            onDropped: console.log("Dropped")
//        }
    }
    Rectangle {
        color: "blue"
        width: 20
        height: 20
        MouseArea {
            anchors.fill: parent
            drag.target: parent
        }
//        Component.onCompleted: console.log("in qml" ,_mbackend.testNumber, _mbackend.myNumber)
    }

//    Item {
//        anchors {
//            right: discoveredDevicesArea.right
//            top: discoveredDevicesArea.bottom
//            margins: 5
//        }
//        width:scaleSlider.implicitWidth
//        Slider {
//            id: scaleSlider
//            to: 2.0
//            value: 1.0

//        }
//    }

}
