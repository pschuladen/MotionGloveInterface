import QtQuick
import QtQuick.Layouts
import MotionGloveInterface

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
                property bool lastInModel: model.index === numConnections

                property int valueType: lastInModel ? TypeHelper.Undefined : connectedTypes[model.index]

                property color nodeColor: lastInModel ? "white" : _typehelper.getColorForValueType(connectedTypes[model.index])
                property int idx: lastInModel ? -1 : model.index


                //                Component.onCompleted: console.log("conPosition", y)
                Rectangle {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                    }
                    height: 4
                    opacity: (outConnector.nodeHovered || inConnector.nodeHovered) ? 0.25 : 0
                    color: connectorPair.nodeColor
                    //(outConnectorMa.containsMouse || inConnectorMa.containsMouse) ? "lightgrey" : "transparent"
                }

                InputConnector {
                    id: inConnector
                    height: connectorPair.height
                    vName: _typehelper.getStringForValueType(connectorPair.valueType)
                    vType: connectorPair.valueType
                    parentID: root.uniqueID
                    vIdx: connectorPair.idx
                    anchors {
                        verticalCenter: connectorPair.verticalCenter
                        horizontalCenter: connectorPair.left
                    }
                    dropAreaEnabled: connectorPair.lastInModel
                }

                OutputConnector {
                    id: outConnector
                    height: connectorPair.height
                    vName: _typehelper.getStringForValueType(connectorPair.valueType)
                    vType: connectorPair.valueType
                    parentID: root.uniqueID
                    vIdx: connectorPair.idx
                    anchors {
                        verticalCenter: connectorPair.verticalCenter
                        horizontalCenter: connectorPair.right
                    }
                }



            }
        }
    }
}
