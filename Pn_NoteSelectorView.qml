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

    PN_NoteSelector {
        id: backend
        objectName: root.uniqueID+"-viewcontroller"

        identifier: root.uniqueID

    }


    Rectangle {
        id: outerShape
        width: processingTitleLabel.x + processingTitleLabel.implicitWidth + 10 //childrenRect.width + childrenRect.x*2
        height: 50//childrenRect.height + childrenRect.y*2
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
            text: "Note\nSelector"
            horizontalAlignment: Text.Center
            anchors {
                top: outerShape.top
                left: outerShape.left
                margins: 7
            }
        }
    }
    ColumnLayout {

        width: octaveSelect.implicitWidth

        anchors {
            horizontalCenter: outerShape.left
            top: outerShape.top
            bottom: outerShape.bottom
        }

        InputConnector {

            id: octaveSelect
            vType: TypeHelper.SingleValue
            vName: _typehelper.getStringForValueType(vType)
            vIdx: 0
            onYChanged: root.setInConOffsetAtIndex(vIdx, y)
            Component.onCompleted: root.setInConOffsetAtIndex(vIdx, y)
            parentID: root.uniqueID
        }

        InputConnector {

            id: noteSelect
            vType: TypeHelper.BoolList
            vName: _typehelper.getStringForValueType(vType)
            vIdx: 1
            onYChanged: root.setInConOffsetAtIndex(vIdx, y)
            Component.onCompleted: root.setInConOffsetAtIndex(vIdx, y)
            parentID: root.uniqueID
        }
    }


    OutputConnector {
        anchors {
            horizontalCenter: outerShape.right
            verticalCenter: outerShape.verticalCenter
        }
        vName: "touches"//_typehelper.getStringForValueType(vType)
        vType: TypeHelper.IntValue
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

