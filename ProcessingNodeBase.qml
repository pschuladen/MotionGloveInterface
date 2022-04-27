import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Layouts

import MotionGloveInterface


Item {

    id: root
    objectName: uniqueID+"-view"
    //required
    property string uniqueID: "noID" //set this when creating the view
    property alias valuecount: backend.valuecount

    width: outerShape.implicitWidth
    height: outerShape.implicitHeight

    ValueViewBackend {
        id: backend
        objectName: root.uniqueID+"-backend"
        viewmode: ValueViewBackend.Custom
        valuecount: 2
    }


    Rectangle {
        id: outerShape
        width: outputRect.x + outputRect.width + inputRect.x//childrenRect.width + childrenRect.x*2
        height: outputRect.y + outputRect.height + processingTitleLabel.y//childrenRect.height + childrenRect.y*2

        color: "lightgrey"
        border {
            color: "black"
            width: 1
        }
        radius: 2

        MouseArea {
            anchors {
                fill: outerShape
//                top: outerShape.top
//                left: outerShape.left
//                right: outputRect.right
//                bottom: scalingFormer.bottom
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
                margins: 5
            }
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
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin:inputRect.controlMargins
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.fillWidth: true
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
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin:inputRect.controlMargins
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.fillWidth: true
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
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.rightMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                }

                Text {
                    text: "OUT"
                    font.pixelSize: 10
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                }
                TextNumberField {
                    Layout.leftMargin: inputRect.controlMargins
                    Layout.rightMargin: inputRect.controlMargins
                    Layout.fillWidth: true
                }
            }
        }
        Rectangle {
            id: inputPlug
            height: 10
            width: height
            radius: height/2
            color: "white"
            anchors.horizontalCenter: parent.left
            anchors.verticalCenter: parent.verticalCenter
            border {
                width: 1
                color: "black"
            }
        }
        Rectangle {
            id: outputPlug
            height: 10
            width: height
            radius: height/2
            color: "white"
            anchors.horizontalCenter: parent.right
            anchors.verticalCenter: parent.verticalCenter
            border {
                width: 1
                color: "black"
            }
        }
    }
}
