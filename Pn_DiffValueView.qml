import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import MotionGloveInterface


NodeViewController {
    id: root
    objectName: uniqueID+"-view"
    z: 1

    width: outerShape.implicitWidth
    height: outerShape.implicitHeight

    PN_DiffValue {
        id: backend
        objectName: root.uniqueID+"-viewcontroller"
        identifier: root.uniqueID
    }

    Rectangle {
        id: outerShape
        width: processingTitleLabel.implicitWidth + 15//childrenRect.width + childrenRect.x*2
        height: attachedConnectors.implicitHeight + processingTitleLabel.y + processingTitleLabel.implicitHeight//childrenRect.height + childrenRect.y*2
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
            text: "diff"
            anchors {
                top: outerShape.top
                left: outerShape.left
                margins: 7
            }
        }

    }

    AttachedConnectorView {
        id: attachedConnectors
//        anchors.fill: parent
        anchors {
            left: parent.left
            right: parent.right
//            verticalCenter: parent.verticalCenter
        }

        uniqueID: root.uniqueID
        connectedTypes: backend.connectedTypes
        viewControl: root

//        useOutputType: true
//        outputType: TypeHelper.SingleValue
    }

    Menu {
        width: 80
        id: contextMenu
        MenuItem {text: "delete"
            onClicked: _mbackend.deleteObjectWithId(root.uniqueID)
        }
//        MenuItem {text: "do something good"}

    }

}
