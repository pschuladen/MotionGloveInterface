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

    property int conType: backend.connectedTypes[0]

    PN_SplitComponents {
        id: backend
        objectName: root.uniqueID+"-viewcontroller"
        identifier: root.uniqueID
    }

    Rectangle {
        id: outerShape
        width: processingTitleLabel.implicitWidth + 15//childrenRect.width + childrenRect.x*2
        height: colLayout.implicitHeight + processingTitleLabel.y + processingTitleLabel.height + 5 //childrenRect.height + childrenRect.y*2
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
            text: "split"
            anchors {
                top: outerShape.top
                left: outerShape.left
                margins: 7
            }
        }

    }

    InputConnector {
        id: inConnector
        vType: root.conType
        vName: _typehelper.getStringForValueType(vType)
        vIdx: 0//root.conType === TypeHelper.Undefined ? -1 : 0
        anchors {
            verticalCenter: outerShape.verticalCenter
            horizontalCenter: outerShape.left
        }
        onYChanged: root.setInConOffsetAtIndex(0, y)
        Component.onCompleted: root.setInConOffsetAtIndex(0, y)
        parentID: root.uniqueID
    }


    ColumnLayout {
        id: colLayout

        property int _topMar: processingTitleLabel.y + processingTitleLabel.height
//        height: root.height
        anchors.bottom: outerShape.bottom
        anchors.top: outerShape.top
        width: 20
        anchors.bottomMargin: 10
        anchors.topMargin: _topMar

        anchors.right: outerShape.right
        spacing: 2
        Layout.bottomMargin: 10

//        implicitHeight: height

        Repeater {
            model: backend.valueNumber//_typehelper.getSizeForValueType(root.conType)
            delegate:
                Item {
                id:delegItem

                property int idxOfComp: model.index
                property string valueName: _typehelper.getNameForValueComponent(root.conType, idxOfComp)

                onYChanged: root.setOutConOffsetAtIndex(idxOfComp, y + colLayout._topMar)
                Component.onCompleted: root.setOutConOffsetAtIndex(idxOfComp, y + colLayout._topMar)

                Layout.fillWidth: true

                Layout.minimumHeight: 10
                Layout.minimumWidth: 20
                Text {
                    id: _vNameLabel
                    text: delegItem.valueName// _typehelper.getNameForValueComponent(root.conType)
                    anchors.left: delegItem.left

                    anchors.verticalCenter: outConnector.verticalCenter
                }
                OutputConnector {
                    id: outConnector
                    vName: delegItem.valueName
                    vType: TypeHelper.SingleValue
                    vIdx: delegItem.idxOfComp

                    anchors {
                        verticalCenter: delegItem.verticalCenter
                        horizontalCenter: delegItem.right
                    }
                    parentID: root.uniqueID
                }
            }
        }
    }



//    AttachedConnectorView {
//        id: attachedConnectors
////        anchors.fill: parent
//        anchors {
//            left: parent.left
//            right: parent.right
////            verticalCenter: parent.verticalCenter
//        }

//        uniqueID: root.uniqueID
//        connectedTypes: backend.connectedTypes
//        viewControl: root

////        useOutputType: true
////        outputType: TypeHelper.SingleValue
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
