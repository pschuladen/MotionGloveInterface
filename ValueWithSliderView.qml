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

        OutputConnector {
            anchors.verticalCenter: row.verticalCenter
            height: 10
            width: 10
            parentID: root.parentIdentifier
//            cColor: root.cColor
            vIdx: root.vIdx
            vName: root.vName
            vType: TypeHelper.SingleValue
        }

//moved to output Connector
//        Rectangle {
//            id: connector
//            anchors.verticalCenter: row.verticalCenter
//            //anchors.topMargin: 10
//            height: 10
//            width: 10
//            radius: 5
//            border {
//                width: 1
//                color: "black"
//            }
//            color: conectorMouseArea.containsMouse ? cColor.lighter(1.3) : cColor


//            //Drag function
//            MouseArea {
//                id: conectorMouseArea
//                anchors.fill: parent
//                anchors.margins: -2
////                width: parent.width * 1.5
////                height: parent.height * 1.5
//                hoverEnabled: true
//                drag.target: draggableItem
//            }
//            Item {

//                id: draggableItem
//                anchors.centerIn: connector

//                Drag.dragType: Drag.Automatic
//                objectName: "this is a dragitem"

//                property bool starting: conectorMouseArea.drag.active

//                Drag.active: conectorMouseArea.drag.active
//                Drag.onDragStarted: console.log("staaaaart")
//                Drag.mimeData: {"text/plain": root.vName,
//                    "nodeId": root.parentIdentifier,
//                    "valueType": TypeHelper.SingleValue,//ProcessNodeController.Single,
//                    "valueIndex": root.vIdx,
//                    "sourceObjectId": sourceObjectId}
////                Drag.onDragFinished: console.log("drag finished")
//            }

//        }
    }
}
