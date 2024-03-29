import QtQuick
import QtQuick.Controls

import MotionGloveInterface

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
                horizontalAlignment: Text.Center
            }
            MouseArea {
                id: moAr
                anchors.fill: parent
                hoverEnabled: true
            }

            Item {
                id: draggableItem
                objectName: "this is a dragitem"

                property bool starting: dragHandle.active

                onStartingChanged: {
                    if(starting) {
                        Drag.startDrag()
                    }
                }
                Drag.active: dragHandle.active
                Drag.mimeData: {"text/plain": model.name, "processorType": model.type, "nodeType": TypeHelper.Process}
            }


            DragHandler {
                id: dragHandle
                target: draggableItem
                onActiveChanged: {
                    if (active) {

                    }

                    cursorShape: Qt.CrossCursor

                }


            }
        }
        ListModel {
            id: createProcessModel
            ListElement {
                name: "sum\ncomps"
                type: TypeHelper.SumComponents
            }
            ListElement {
                name: "scale"
                type: TypeHelper.Scale
            }
            ListElement {
                name: "absValue"
                type: TypeHelper.AbsValue
            }
            ListElement {
                name: "diffValue"
                type: TypeHelper.DiffValue
            }
            ListElement {
                name: "split\nComp"
                type: TypeHelper.SplitComponents
            }
            ListElement {
                name: "Touch\nRecog"
                type: TypeHelper.TouchRecognizer
            }
            ListElement {
                name: "Note\nSelector"
                type: TypeHelper.NoteSelector
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1}D{i:5}
}
##^##*/
