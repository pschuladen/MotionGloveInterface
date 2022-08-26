import QtQuick
import QtQuick.Controls
import QtQuick.Shapes
import QtQuick.Layouts

import MotionGloveInterface



NodeViewController {

    id: root
    objectName: uniqueID+"-view"

    z:1
//    required property string uniqueID //set this when creating the view




    width: outerShape.implicitWidth
    height: outerShape.implicitHeight

    PN_TouchRecognizer {
        id: backend
        objectName: root.uniqueID+"-viewcontroller"

        identifier: root.uniqueID
        thresholdValue: thresholdSlider.value


    }


    Rectangle {
        id: outerShape
        width: processingTitleLabel.implicitWidth + calibrButton.implicitWidth + 5 //childrenRect.width + childrenRect.x*2
        height: thresholdSlider.y + thresholdSlider.implicitHeight +  threshValuesLayoutItem.height + 5//childrenRect.height + childrenRect.y*2
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
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            onClicked: (mouse) => {
                           focus=true
                           if(mouse.button === Qt.RightButton) {
                               contextMenu.popup()
                           }
                           if(mouse.button === Qt.LeftButton) {
                               _mbackend.showConnectionLists(root.uniqueID);

                           }
            }
            drag.target: root
        }

        Text {
            id: processingTitleLabel
            text: "touchReco"
            anchors {
                top: outerShape.top
                left: outerShape.left
                margins: 7
            }
        }
        Button {
            id: calibrButton
            text: "calib"
            onClicked: backend.sig_catchOffValues()
            anchors {
                verticalCenter: processingTitleLabel.verticalCenter
                left: processingTitleLabel.right
            }
        }

        Slider {
            id: thresholdSlider
//            value: backend.thresholdValue
//            onValueChanged: console.log("slidervalue", thresholdSlider.value)
            Component.onCompleted: value = backend.thresholdValue

            anchors {
                top: calibrButton.bottom
                left:processingTitleLabel.left
                right: calibrButton.right
                rightMargin: 5
            }

        }
        RowLayout {

            id: threshValuesLayoutItem

            anchors {
                left: thresholdSlider.left
                right: thresholdSlider.right
                top: thresholdSlider.bottom
            }
            height: 20

            Repeater {
                model: 6
                delegate: Label {
                    text: backend.touchThresholds[model.index].toFixed(0)
                }

//                Label
            }
        }


    }
    InputConnector {
        anchors {
            horizontalCenter: outerShape.left
            verticalCenter: outerShape.verticalCenter
        }
        id: inConnector
        vType: TypeHelper.List
        vName: _typehelper.getStringForValueType(vType)
        vIdx: 0//root.conType === TypeHelper.Undefined ? -1 : 0

        onYChanged: root.setInConOffsetAtIndex(0, y)
        Component.onCompleted: root.setInConOffsetAtIndex(0, y)
        parentID: root.uniqueID
    }
    OutputConnector {
        anchors {
            horizontalCenter: outerShape.right
            verticalCenter: outerShape.verticalCenter
        }
        vName: "touches"//_typehelper.getStringForValueType(vType)
        vType: TypeHelper.BoolList
        vIdx: 0
        parentID: root.uniqueID

        onYChanged: root.setOutConOffsetAtIndex(0, y)
        Component.onCompleted: root.setOutConOffsetAtIndex(0, y)

    }

//    AttachedConnectorView {
//        anchors.fill: parent
//        uniqueID: root.uniqueID
//        connectedTypes: backend.connectedTypes
//        viewControl: root
////        xxx: "hase"

//    }

    Menu {
        width: 80
        id: contextMenu
        MenuItem {text: "delete"
            onClicked: _mbackend.deleteObjectWithId(root.uniqueID)
        }
//        MenuItem {text: "do something good"}

    }
}

