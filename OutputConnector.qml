import QtQuick
import MotionGloveInterface
import QtQuick.Controls

Item {
    id: root

//    Component.onCompleted: console.log("outputconnector", mapToGlobal(x, y))

    //required properties dont seem to work with repeater -model
    property string parentID: "unknown" //important for identifying sender of connection

    property int vIdx: -1 //identifies potential subvalues; -1 means the whole datatype
//    onVIdxChanged: console.log("node",parent.x,parent.y)

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
            anchors.margins: -4
            hoverEnabled: true
            drag.target: draggableItem
            acceptedButtons: Qt.RightButton | Qt.LeftButton
            onClicked: (mouse) => {
                           if(mouse.button === Qt.RightButton) {
                               contextMenu.popup()
                           }
                       }
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
    Menu {
        width: 80
        id: contextMenu
        MenuItem {text: "del connections"
            onClicked: _mbackend.deleteSendConnectionsForObjectAtIdx(root.parentID, vIdx)
        }
    }
}
