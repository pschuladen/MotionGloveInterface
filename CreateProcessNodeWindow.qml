import QtQuick
import QtQuick.Controls

Item {

    visible: true

    width: 200
    height: 100

    ListView {
        anchors.fill: parent
        orientation: ListView.Horizontal
        spacing: 10
        model: createProcessModel
        delegate: Rectangle {
            property color bgcolor: "lightgrey"
            property string comString: "proc-"+model.name
            color: moAr.containsMouse ? bgcolor.darker(1.5) : bgcolor
            width:  80
            height: 60
            border {
                color: "black"
                width: 4
            }

            Text {
                anchors.centerIn: parent
                text: model.name
            }
            MouseArea {

                id: moAr
                anchors.fill: parent
                hoverEnabled: true
//                drag.target: draggableItem
//                onContainsMouseChanged: console.log("mouse in area")
            }
            Text {
                id: draggableItem
                objectName: "this is a dragitem"

                property bool starting: dragHandle.active
                property string custom: text
                onStartingChanged: {
                    if(starting) {

                        Drag.startDrag()
                    }
                }

                text: model.name+"wadde"

                visible: Drag.active
                Drag.active: dragHandle.active
                Drag.onDragStarted: console.log("staaaaart")
//                Drag.onActiveChanged: console.log("im moooving")
                Drag.mimeData: {"text/plain": custom}
                Drag.onDragFinished: console.log("drag finished")
//                Drag.dragType: Drag.Automatic
                color: "red"
                z: 100
            }


            DragHandler {
                id: dragHandle
                target: null
                onActiveChanged: {
                    if (active) {

                    }

                    console.log("drag changed", model.name)}
                cursorShape: Qt.CrossCursor
            }


        }
    }
    ListModel {
        id: createProcessModel
        ListElement {
            name: "sum"
        }
        ListElement {
            name: "scale"
        }
    }



}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1}D{i:5}
}
##^##*/
