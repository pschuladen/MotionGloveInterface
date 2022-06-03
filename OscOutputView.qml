import QtQuick
import QtQuick.Layouts
import MotionGloveInterface

Item {

    id: root

    property string uniqueID: ""

    property string oscAddress: controller.oscPaths[viewIdx]
    property OscOutputViewController controller: null
//    controller.onIndexHoveredChanged: {
//        remoteHovered = controller.indexHovered == viewIdx
//    }
    property bool viewHovered: magrabber.containsMouse
    onViewHoveredChanged: controller.viewAtIndexHovered(viewHovered, viewIdx)
    property bool remoteHovered: controller.indexHovered == viewIdx
    property bool lightenView: viewHovered || remoteHovered

    property int connectedType: TypeHelper.Undefined
    property int viewIdx: 0
    implicitHeight: 24
    implicitWidth: 120
    Rectangle {
        id: addressRect
        anchors.fill: parent
        anchors.margins: 2

        TextInput {
            id: outputPathTextInput
            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
                left: grabRect.right
                margins: 4
            }

            text: root.oscAddress
            validator: RegularExpressionValidator {
                regularExpression: /^\/.+/
            }
            font.pixelSize: 10
            horizontalAlignment: Text.AlignRight
            selectByMouse: true
            Keys.onReturnPressed: focus=false
            Keys.onEscapePressed: {
                text = root.controller.oscPaths[root.viewIdx]
                focus = false
            }
            onEditingFinished: root.controller.setOscPathAtIndex(text, root.viewIdx)//root.ipAddress = text
            onFocusChanged: {
                if(!focus) {text = root.controller.oscPaths[root.viewIdx]}
            }
        }
        Rectangle {
            id: grabRect
            property color rColor: "darkgrey"
            color: root.lightenView ? rColor.lighter(1.2 ) : rColor

            width: 20

            anchors {
                left:parent.left
                top:parent.top
                bottom:parent.bottom
            }


            MouseArea {
                id: magrabber
                anchors.fill: parent
                hoverEnabled: true

                drag.target: draggableItem
            }

            Item {
                id: draggableItem

                Drag.active: magrabber.drag.active && !root.controller.nodeAtIndexWasCreated(root.viewIdx)

                Drag.dragType: Drag.Automatic
                Drag.mimeData: {"text/plain": root.oscAddress,
//                    "sourceDevice": root.sourceObjectId,
//                    "identifier": root.identifier,
                    "nodeType": TypeHelper.Output,
                    "valueType": root.connectedType,
                    "outputIndex": root.viewIdx,
                    "targetDevice": root.uniqueID
                }
            }
        }

//        InputConnector {
//            anchors {
//                verticalCenter: addressRect.verticalCenter
//                horizontalCenter: addressRect.left
//            }
//            id: outConnector
//            height: 10
//            vType: controller.valueTypes[root.viewIdx]
//            parentID: root.uniqueID
//            vIdx: root.viewIdx
//        }
    }
}
