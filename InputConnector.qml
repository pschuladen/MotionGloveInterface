import QtQuick
import MotionGloveInterface
import QtQuick.Controls

Item {
    id:root

    property string parentID: "unknown" //important for identifying sender of connection

    property int vIdx: -1 //identifies potential subvalues; -1 means the whole datatype

    property int vType: TypeHelper.Undefined // type of connection that will be initiated

    property string vName: "" // just the text while dragging

    property color cColor: _typehelper.getColorForValueType(vType)

    property bool nodeHovered: inConnectorMa.containsMouse

    property bool dropAreaEnabled: true

    implicitHeight: 10
    implicitWidth: 10

    Rectangle {
        id: inputConnector
        anchors.fill: parent
        radius: height/2
        color: cColor//connectorPair.nodeColor
        border {
            width: 1
            color: inDrop.containsDrag ? "cyan" : "black"
        }
        MouseArea {
            id: inConnectorMa
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.RightButton | Qt.LeftButton
            onClicked: (mouse) => {
                           if(mouse.button === Qt.RightButton) {
                               contextMenu.popup()
                           }
                       }
        }


        DropArea {
            id: inDrop
            enabled: root.dropAreaEnabled
            anchors.fill: parent
            anchors.margins: -8
            keys: ["valueType", "valueIndex", "sourceObjectId"]//, "text/nodeId", "text/valueType"]"nodeId",
//            onEntered:(drag) =>  console.log(drag.keys)
            onDropped: (drop) => {
//                           console.log(drop.source)
                           if(_mbackend.connectionRequest(//drop.getDataAsString("sourceObjectId"),
                                                          drop.getDataAsString("sourceObjectId"),
                                                          drop.getDataAsString("valueIndex"),
                                                          //inputConnector,
                                                          root.parentID,
                                                          root.vIdx,
                                                          //drop.source,
                                                          drop.getDataAsString("valueType"))) {
                               drop.acceptProposedAction()

                           }
                       }

        }

    }
    Menu {
        width: 80
        id: contextMenu
        MenuItem {text: "del connection"
            onClicked: _mbackend.deleteReceiveConnectionForObjectAtIdx(root.parentID, vIdx)
        }
    }
}
