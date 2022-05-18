import QtQuick
import QtQuick.Controls
import MotionGloveInterface


Item {
    id: root
    property string identifier: "setthisvalue"
    signal connectButtonChanged(connect: bool, deviceN: string);

    width:  childrenRect.width+childrenRect.x
    height: childrenRect.height+childrenRect.y + 10

    OscInputViewController {
        objectName: "oscviewcontroller"
        id: oscviewcontrol
        connectDevice: connectDeviceButton.checked
    }

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
            font.bold: true
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
        height: sensorView.implicitHeight

        Column {
            // all SensorValuesView go in here, create by mainbackend
            // the property valuesHiddenBool should always be present when using SensorValuesView
            id: sensorView
            objectName: "sensorViewContainer"
            property bool valuesHiddenBool: !sensorsShow.checked
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
