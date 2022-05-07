import QtQuick
import QtQuick.Layouts

Item {
    required property var connectedTypes
    property int numConnections: connectedTypes.length
    required property string uniqueID
    id: root

    //    Component.onCompleted: console.log("attached view rect", width, height)

    ColumnLayout {
        anchors.fill: parent
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        spacing: 0
        //        Component.onCompleted: console.log("layoutrect", width, height, spacing)

        Repeater {

            model: numConnections+1
            delegate:
                Item {
                id: connectorPair
                Layout.fillWidth: true
                height: 10
                Layout.minimumWidth: 20

                property color nodeColor: model.index === numConnections ? "white" : _typehelper.getColorForValueType(connectedTypes[model.index])


                //                Component.onCompleted: console.log("conPosition", y)
                Rectangle {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                    }
                    height: 4
                    opacity: (outConnectorMa.containsMouse || inConnectorMa.containsMouse) ? 0.25 : 0
                    color: connectorPair.nodeColor
                    //(outConnectorMa.containsMouse || inConnectorMa.containsMouse) ? "lightgrey" : "transparent"
                }

                Rectangle {
                    id: inputConnector
                    height: connectorPair.height
                    width: height
                    radius: height/2
                    color: connectorPair.nodeColor
                    border {
                        width: 1
                        color: inDrop.containsDrag ? "cyan" : "black"
                    }
                    //                    x: -1* radius
                    anchors {
                        verticalCenter: connectorPair.verticalCenter
                        horizontalCenter: connectorPair.left
                    }
                    MouseArea {
                        id: inConnectorMa
                        anchors.fill: parent
                        hoverEnabled: true
                    }


                    DropArea {
                        id: inDrop
                        anchors.fill: parent
                        anchors.margins: -4
                        keys: ["nodeId", "valueType", "valueIndex", "sourceObjectId"]//, "text/nodeId", "text/valueType"]
                        onEntered:(drag) =>  console.log(drag.keys)
                        onDropped: (drop) => {
                                       console.log(drop.source)
                                       if(_mbackend.connectionRequest(//drop.getDataAsString("sourceObjectId"),
                                                                      drop.getDataAsString("nodeId"), drop.getDataAsString("valueIndex"), inputConnector,
                                                                      root.uniqueID, model.index, drop.source,
                                                                      drop.getDataAsString("valueType"))) {
                                           drop.acceptProposedAction()
                                       }
                                   }
                    }
                }
                Rectangle {
                    height: connectorPair.height
                    width: height
                    radius: height/2
                    color: connectorPair.nodeColor
                    border {color: "black"; width: 1}
                    x: connectorPair.width-radius
                    anchors {
                        verticalCenter: connectorPair.verticalCenter
                        horizontalCenter: connectorPair.right
                    }
                    MouseArea {
                        id: outConnectorMa
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }

            }
        }
    }
}
