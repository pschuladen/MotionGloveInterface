import QtQuick
import MotionGloveInterface

Item {
    id: root
    //required properties dont seem to work with repeater -model
    property string parentID: "unknown" //important for identifying sender of connection

    property int vIdx: -1 //identifies potential subvalues; -1 means the whole datatype

    property int vType: TypeHelper.Undefined // type of connection that will be initiated

    property string vName: "" // just the text while dragging

    property color cColor: _typehelper.getColorForValueType(vType)

    property bool nodeHovered: conectorMouseArea.containsMouse
//    property TypeHelper.ValueType name: value

    implicitHeight: 10
    implicitWidth: 10

    Rectangle {
        id: connector
//        anchors.verticalCenter: row.verticalCenter
        anchors.fill: parent
        //anchors.topMargin: 10
//        height: 10
//        width: 10
        radius: height/2
        border {
            width: 1
            color: "black"
        }
        color: conectorMouseArea.containsMouse ? root.cColor.lighter(1.3) : root.cColor


        //Drag function
        MouseArea {
            id: conectorMouseArea
            anchors.fill: parent
            anchors.margins: -2
            hoverEnabled: true
            drag.target: draggableItem
        }
        Item {

            id: draggableItem
            anchors.centerIn: connector

            Drag.dragType: Drag.Automatic
            objectName: "this is a dragitem"

            property bool starting: conectorMouseArea.drag.active

            Drag.active: conectorMouseArea.drag.active
            Drag.mimeData: {"text/plain": root.vName,
                "sourceObjectId": root.parentID,
                "valueType": root.vType,//ProcessNodeController.Single,
                "valueIndex": root.vIdx}
//                "sourceObjectId": sourceObjectId}

        }
    }
}
