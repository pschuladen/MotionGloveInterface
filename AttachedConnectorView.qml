import QtQuick
import QtQuick.Layouts
import MotionGloveInterface


Item {
    required property var connectedTypes
    property int numConnections: connectedTypes.length
    required property string uniqueID
    required property NodeViewController viewControl
    property int idxOffset: 0
    id: root

    ColumnLayout {
        property int _topMar: 5
        id: colLayout
        anchors.fill: parent
        anchors.bottomMargin: 5
        anchors.topMargin: _topMar
        spacing: 0


        Repeater {

            model: numConnections+1
            delegate:
                Item {

                onYChanged: viewControl.setConOffsetAtIndex(idx, y + colLayout._topMar)

                id: connectorPair
                Layout.fillWidth: true
                height: 10
                Layout.minimumWidth: 20

                property bool lastInModel: model.index === numConnections

                property int valueType: lastInModel ? TypeHelper.Undefined : connectedTypes[model.index]

                property color nodeColor: lastInModel ? "white" : _typehelper.getColorForValueType(connectedTypes[model.index])
                property int idx: lastInModel ? -1 : model.index + idxOffset


                Rectangle {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left; right: parent.right
                    }
                    height: 4
                    opacity: (outConnector.nodeHovered || inConnector.nodeHovered) ? 0.25 : 0
                    color: connectorPair.nodeColor
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
