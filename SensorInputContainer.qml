import QtQuick
import QtQuick.Controls
import MotionGloveInterface


Item {
    id: root
    property string identifier: "setthisvalue"
    signal connectButtonChanged(connect: bool, deviceN: string);

    width:  childrenRect.width+childrenRect.x
    height: childrenRect.height+childrenRect.y + 10
//    onWidthChanged: console.log("width", width)
//    onHeightChanged: console.log("height", height)

    Rectangle {
        anchors {
            top: header.top
            left: header.left
            right: header.right
            bottom: sensorViewCont.bottom
            bottomMargin: -10
        }
        border {
            width: 2
            color: "#616161"
        }

        color: "darkgrey"
    }

    Item {
        id: header
        width: 130
        height: childrenRect.height + childrenRect.y
        anchors {
            top: root.top
            left: root.left
        }
        Text {
            id: deviceName
            text: root.identifier
            anchors {
                top: header.top
                topMargin: 10
                left: header.left
                leftMargin: 7
            }
        }
        Button {
            id: connectDeviceButton
            checkable: true
            anchors {
                right: header.right
                verticalCenter: deviceName.verticalCenter
            }
            text: "C"
            onCheckedChanged: root.connectButtonChanged(checked, identifier);

        }

        Rectangle {
            anchors.fill: controlButtons
            color: "#6f6f6f"
        }

        Grid {
            id: controlButtons
            columns: 2
            anchors {
                top: deviceName.bottom
                topMargin: 10
                left: header.left
                right: header.right
            }

            Button {
                id: inputMuteButton
                text: "mut"
                checkable: true
                 width: 50
//                 height: 20
            }
            Button {
                id: inputRecButton
                text: "rec"
                checkable: true
                width: 50
            }
            Button {
                id: inputPlayButton
                text: "play"
                checkable: true
                width: 50
            }
            Button {
                id: sensorsShow
                text: "sen"
                checkable: true
                width: 50
                checked: true
                onCheckedChanged: checked ? showSensorAnimation.start() : hideSensorAnimation.start()
            }
        }
        Rectangle {
            anchors {
                left: header.left
                right: header.right
                top: controlButtons.bottom
                topMargin: 5
            }
            height: 7
            color: "#002c00"
            Text {
                font.pixelSize: 4
                text: "signalqualityxlajorgjpakmdmkanfklmfkanlanöfdjaknk"
                color: "white"
            }
        }
    }

    Item {
        id: sensorViewCont
        anchors {
            top: header.bottom
            topMargin: 5
            left: root.left
            leftMargin: 5
        }
        height: sensorView.implicitHeight//sensorsShow.checked ? sensorView.implicitHeight : 0
//        onHeightChanged: console.log("onHeight", sensorView.showSensorsBool)

        Column {
            id: sensorView
            objectName: "sensorViewContainer"
            property bool globalBoolValuesHidden: !sensorsShow.checked
//            onShowSensorsBoolChanged: {
//                console.log("variable", showSensorsBool)
//                showHideSensorAnimation.start()
//            }

            Repeater {
                model: 12
                delegate: SensorValuesView {
                    viewmode: ValueViewBackend.Grav
                }
            }
        }

        SequentialAnimation {
            id: showSensorAnimation
            PropertyAction {
                target: sensorViewCont; property: "clip"; value: true }
            PropertyAction {
                target: sensorView; property: "visible"; value: true }
            NumberAnimation {
                target: sensorViewCont
                property: "height"
                duration: 200
                easing.type: Easing.InOutQuad
                to: sensorView.implicitHeight
            }
//            PropertyAction {
//                target: sensorView; property: "visible"; value: sensorView.showSensorsBool }
            PropertyAction {
                target: sensorViewCont; property: "clip"; value: false }
        }
        SequentialAnimation {
            id: hideSensorAnimation
            PropertyAction {
                target: sensorViewCont; property: "clip"; value: true }
            NumberAnimation {
                target: sensorViewCont
                property: "height"
                duration: 200
                easing.type: Easing.InOutQuad
                to: 0
            }
            PropertyAction {
                target: sensorView; property: "visible"; value: false }
            PropertyAction {
                target: sensorViewCont; property: "clip"; value: false }
        }
    }
}
//        Behavior on height {
//            SequentialAnimation {
//                PropertyAction {
//                    target: sensorViewCont; property: "clip";
//                    value: true
//                }
//                PropertyAction {
//                    target: sensorView; property: "visible";
//                    value: true
//                }
//                NumberAnimation {
//                    duration: 250
//                    easing.type: Easing.InOutCubic
//                }
//                PropertyAction {
//                    target: sensorView; property: "visible";
//                    value: sensorView.showSensorsBool
//                }
//                PropertyAction {
//                    target: sensorViewCont; property: "clip";
//                    value: false
//                }
//            }
//        }





//olde
//Item {
//    id:root
//    width: showhideButton.checked ? changesizeRect.x + changesizeRect.width : headerLine.childrenRect.width
//    height: showhideButton.checked ? changesizeRect.y + changesizeRect.height : headerLine.implicitHeight
////    width: changesizeRect.x + changesizeRect.width
////    height:changesizeRect.y + changesizeRect.height

//    Rectangle {
//        id: contRect
//        color: "cornflowerblue"
//        border {
//            width: 4
//            color: "blanchedalmond"
//        }
//        anchors.fill: parent


//        //        Column {
//        //            anchors {
//        //                left: parent.left
//        //                right: parent.right
//        //            }
//        Rectangle {
//            id: headerLine
//            anchors {
//                top:parent.top
//                left: parent.left

//                right: parent.right
//                margins: parent.border.width

//            }

//            color: parent.color.lighter(1.2)
//            height: childrenRect.height//sensorNameLabel.implicitHeight
//            MouseArea {
//                anchors.fill: parent
//                drag {
//                    target: root
//                    minimumX: 5
//                    minimumY: 5
//                }

//            }
//            Text {
//                id: sensorNameLabel
//                text: root.objectName
//                anchors {
//                    left: parent.left
//                    top: parent.top
//                    margins: 10
//                }
//            }
//            Button {
//                id: showhideButton
//                checkable: true
//                text: checked ? "hide" : "show"
//                checked: true
//                anchors {
//                    right: parent.right
//                    margins: 5
//                    verticalCenter: sensorNameLabel.verticalCenter
//                }
//            }
//        }
//        FlexScrollViewLayout {
//            visible: showhideButton.checked
//            anchors {
//                margins: parent.border.width
//                left: parent.left
//                right: parent.right
//                bottom: parent.bottom
//                top: headerLine.bottom
//            }
//        }
//        //        }
//    }
//    Rectangle {
//        id: changesizeRect
//        color: "black"
//        y: 200
//        x: 150

//        width: 8
//        height: 8
//        MouseArea {
//            anchors.fill: parent

//            drag
//            {
//                target: changesizeRect
////                active: showhideButton.checked
//                minimumX: 20
//                minimumY: 80
//            }


//        }
//    }

//}
