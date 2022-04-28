import QtQuick
import QtQuick.Controls
import MotionGloveInterface

Item {
    id: root

    property real cVal: 0.23
    property string vName: "vv"
    property int vIdx: -2
    property color cColor: "green"
    property real vMax: 1
    property real vMin: -1
    property string parentIdentifier: "default"
    property string vIdentifier: parentIdentifier+"-"+vName
    property string sourceObjectId: "unknown"

    height: row.implicitHeight
    width: row.implicitWidth





    Row {
        id: row

        Text {

            //                id: xNameLabel
            //                text: model.label
            width: 10
            text: vName
            font.pixelSize: 10
            anchors.verticalCenter:row.verticalCenter
        }
        Text {
            //                id: xValueLabel

            width: 35
            font.pixelSize: 10
            //                text: model.value.toFixed(2)
            text: cVal.toFixed(2)
            horizontalAlignment: Text.AlignRight//Number.toString(Math.round(xVal * 100)/100)
            anchors.verticalCenter:row.verticalCenter
        }
        Slider {
            enabled: true
            value: cVal
            from: vMin
            to: vMax
            width: 70
        }
        Rectangle {
            anchors.verticalCenter: row.verticalCenter
            //anchors.topMargin: 10
            height: 10
            width: 10
            radius: 5
            border {
                width: 1
                color: "black"
            }
            color: conectorMouseArea.containsMouse ? cColor.lighter(1.3) : cColor
            MouseArea {
                id: conectorMouseArea
                anchors.centerIn:parent
                width: parent.width * 1.5
                height: parent.height * 1.5
                hoverEnabled: true
                drag.target: draggableItem

//                drag.activeChanged: console.log("drag active", drag.active)
//                drag.dragStarted: console.log("drag started")


//                onPressed: {
//                    console.log("clicked on", vIdentifier)
//                }
//                onReleased: {
//                    console.log("released", root.vName)
//                }
//                drag.hotSpot: 2
            }
//            DragHandler {
//                id: dragHandler
//                target: draggableItem

//            }
//            Text {
//                id: draggableItem
////                onXChanged: console.log(x)
//                property bool draggg: conectorMouseArea.drag.active
//                onDragggChanged: {
////                    console.log("dragggggg")
//                    if(draggg) { Drag.start()}
//                }
//                Drag.active: conectorMouseArea.drag.active
//                text: "A"
//                Component.onCompleted: console.log(root.vName)
//                Drag.onActiveChanged: console.log("dragactive", Drag.active)
//                Drag.proposedAction: Qt.MoveAction

//                Drag.mimeData: {"text/plain": root.vName}//, "text/nodeId": root.parentIdentifier, "text/valueType": "single"};
//            }
            Item {
//                width: parent.width
//                height: parent.height
//                color: "cyan"
                id: draggableItem
                Drag.onActiveChanged: console.log("active changed")
                Drag.dragType: Drag.Automatic
                objectName: "this is a dragitem"

                property bool starting: conectorMouseArea.drag.active
//                property string custom: text
//                onStartingChanged: {
//                    if(starting) {
//                        console.log("startdrag")
//                        Drag.startDrag()
//                    }
//                }

//                text: model.name

//                visible: Drag.active
                Drag.active: conectorMouseArea.drag.active
                Drag.onDragStarted: console.log("staaaaart")
//                Drag.onActiveChanged: console.log("im moooving")
                Drag.mimeData: {"text/plain": root.vName,
                                "nodeId": root.parentIdentifier,
                                "valueType": ProcessNodeController.Single,
                                "valueIndex": root.vIdx,
                                "sourceObjectId": sourceObjectId}
                Drag.onDragFinished: console.log("drag finished")

//                Drag.dragType: Drag.Automatic
//                color: "red"
//                z: 0
            }


//            DragHandler {
//                id: dragHandle
//                target: null//draggableItem
//                onActiveChanged: {
//                    if (active) {

//                    }

//                    console.log("drag changed", model.name)}
//                cursorShape: Qt.CrossCursor


//            }

//            DropArea {
//                onDropped: console.log("Dropped")
//            }
        }
    }

}
