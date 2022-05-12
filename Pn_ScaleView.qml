import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Layouts

import MotionGloveInterface


Item {

    id: root
    objectName: uniqueID+"-view"
    //required
    required property string uniqueID //set this when creating the view
//    property alias valuecount: backend.valuecount



    width: outerShape.implicitWidth
    height: outerShape.implicitHeight

    Component.onCompleted: console.log("processnode rect", width, height)

    PN_Scale {
        id: backend
        objectName: root.uniqueID+"-viewcontroller"

        clipOutput: clipValueCheckbox.checked

    }


    Rectangle {
        id: outerShape
        width: outputRect.x + outputRect.width + inputRect.x//childrenRect.width + childrenRect.x*2
        height: outputRect.y + outputRect.height + processingTitleLabel.y//childrenRect.height + childrenRect.y*2
        implicitHeight: height
        implicitWidth: width

        color: "lightgrey"
        border {
            color: "black"
            width: 1
        }
        radius: 2

        MouseArea {
            anchors {
                fill: outerShape
            }
            hoverEnabled: true
//            onContainsMouseChanged: console.log("mouse")
            onClicked: focus=true
            drag.target: root
        }

        Text {
            id: processingTitleLabel
            text: "scale"
            anchors {
                top: outerShape.top
                left: outerShape.left
                margins: 7
            }
        }
        Text {
            id: testValueView
            text: "clip"//backend.singleInputValue.toFixed(2)
            anchors {
                verticalCenter: processingTitleLabel.verticalCenter
                right: clipValueCheckbox.left
                rightMargin: 5
            }
            horizontalAlignment: Text.AlignRight
        }
        CheckBox {
            id: clipValueCheckbox
            anchors {
                verticalCenter: processingTitleLabel.verticalCenter
                right: outerShape.right
                rightMargin: 7
            }
            checked: backend.clipOutput
        }

        Rectangle {
            id: scalingFormer
            height: 60
            width: 60
            color: "#501f1f"
            radius: 3
            border {
                width: 2
                color: "black"
            }
            anchors {
                top: processingTitleLabel.bottom
                topMargin: 7
                left: inputRect.right
            }

            Shape {
                ShapePath {
                    strokeColor: "#646464"
                    strokeWidth: 1

                    startX: 0
                    startY: scalingFormer.height
                    PathLine {
                        x: scalingFormer.width
                        y: 0
                    }
                    Component.onCompleted: console.log("stroke")
                }
            }
            Rectangle {
                width: 7
                height: width
                radius: height / 2
                color: "white"
            }
        }
        Rectangle {
            id: inputRect
            property real controlMargins: 4
            property real controlWidth: 46
            anchors {
                top: scalingFormer.top
                bottom: scalingFormer.bottom
                left: outerShape.left
                leftMargin: 7
            }
            width: controlWidth//inputControls.implicitWidth
            color: "darkgrey"//"green"
            border.width: 1
            border.color: "black"
            ColumnLayout {
                id: inputControls
                anchors.fill: parent
                TextNumberField {
                    id: inHighField
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin:inputRect.controlMargins
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                    value: backend.inHigh
                    onValueChanged: backend.setInHigh(value)
                }
                Text {
                    text: "IN"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignRight
                   Layout.alignment: Qt.AlignRight
                   Layout.rightMargin: inputRect.controlMargins
                   Layout.fillWidth: true
                }
                TextNumberField {
                    id: inLowField
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin:inputRect.controlMargins
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                    value: backend.inLow
                    onValueChanged: backend.setInLow(value)
                }
            }
        }
        Rectangle {
            id: outputRect
            anchors {
                top: scalingFormer.top
                left: scalingFormer.right
                bottom: scalingFormer.bottom
                rightMargin: 7
            }
            width: inputRect.controlWidth//42//outputControls.implicitWidth
            color: "darkgrey"//"#b9ff0000"
            border.width: 1
            border.color: "black"
            ColumnLayout {
                id: outputControls
                anchors.fill: parent

                TextNumberField {
                    id: outHighField
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.rightMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                    value: backend.outHigh
                    onValueChanged: backend.setOutHigh(value)
                }

                Text {
                    text: "OUT"
                    font.pixelSize: 10
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                }
                TextNumberField {
                    id: outLowField
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.rightMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                    value: backend.outLow
                    onValueChanged: backend.setOutLow(value)
                }
            }
        }

    }

    AttachedConnectorView {
        anchors.fill: parent
//        Component.onCompleted: console.log("parentsize", parent.width, parent.height)
        uniqueID: root.uniqueID
        connectedTypes: backend.connectedTypes//[TypeHelper.Quat, TypeHelper.Vector, TypeHelper.List]
//        numConnections:
    }
}
