import QtQuick
import QtQuick.Controls 2.5
import MotionGloveInterface

Item {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    objectName: identifier
    property alias viewmode: backend.viewmode
    property string identifier: "ientifier not set!!"
//    onIdentifierChanged: console.log("sensor view ID", identifier);
    property alias backendObject: backend

    property bool globalBool_valuesHidden: parent ? parent.valuesHiddenBool : false
    property bool viewCollapsed: true
    property string sourceObjectId: backend.sourceObjectID//parent ? parent.root.uniqueID : "";

    property int valueType: _typehelper.valueTypeForSensor(backend.viewmode)
    property string oscInputPath: identifier

//    property point ttp: mapToGlobal(Qt.point(maCreateInput.x, maCreateInput.y))
//    onTtpChanged: console.log("global ttp point", ttp)


    InputValueViewController {
        id: backend
        objectName: "valuebackend"
        emitvalues: !root.viewCollapsed && !root.globalBool_valuesHidden
        identifier: root.identifier


    }


    Rectangle {
        id: contentRect
        width:  valueColumn.implicitWidth
        height: childrenRect.height + childrenRect.y + 5
        property real heightForm: columnWrapper.height + columnWrapper.y + 10
        radius: 2
        border {
            width: 1
            color: "black"
        }
        property color bgcolor: "lightgrey"
        color: backend.mouseHover ? bgcolor.lighter(1.4) : bgcolor
        clip: false

        Text {
            id: titleLable
            text: backend.identifier
            font.pixelSize: 10
            font.underline: true
            anchors {
                left: contentRect.left
                leftMargin: 10
                top: contentRect.top
                topMargin: 10
            }
        }
        Rectangle {
            id:muteButton

            anchors {
                right: contentRect.right
//                top: contentRect.top
                verticalCenter: titleLable.verticalCenter
                margins: 15
            }
            border {
                width: 1
                color: "black"
            }

            width: 10
            height: 10
            radius: 3
            color: "#861919"
        }

//        OutputConnector {
//            anchors {
//                verticalCenter: titleLable.verticalCenter
//                horizontalCenter: contentRect.right
//            }

//            parentID: root.identifier
//            vName: _typehelper.getStringForValueType(vType)
//            vType: _typehelper.valueTypeForSensor(viewmode)

//        }
//        Item {
//            anchors {
//                verticalCenter: titleLable.verticalCenter
//                horizontalCenter: contentRect.right
//            }
//            width: 10
//            height: 10


//            property color conColor: _typehelper.getColorForValueType(root.valueType)
//            Rectangle {
//                anchors {
//                    fill: parent
//                    leftMargin: parent.width/20
//                }
//                color: maCreateInput.containsMouse ? parent.conColor.lighter(1.8) : parent.conColor
//                radius: height/2
//            }
//            Rectangle {
//                anchors {fill: parent; rightMargin: parent.width/2}
//                color: maCreateInput.containsMouse ? parent.conColor.lighter(1.8) : parent.conColor
//            }
//            MouseArea {
//                id: maCreateInput
//                anchors.fill: parent
//                anchors.margins: -5
//                hoverEnabled: true

//                onClicked: _mbackend.createInputNode(root.sourceObjectId, root.identifier, root.valueType, mapToGlobal(Qt.point(x,y)));

//            }
//        }

        Item {
            id: columnWrapper
            clip: true
            anchors {
                left: contentRect.left
                leftMargin: 5
                top: titleLable.bottom
                topMargin: 10

            }
            width: valueColumn.implicitWidth
            height: root.viewCollapsed ? 0 : valueColumn.implicitHeight

            Column {

                id: valueColumn
                property bool showValues: !root.viewCollapsed

                visible: false //controled by height animation

                anchors.fill: parent

                Repeater {
                    id:rep
                    model:backend.valueCount
                    delegate:
                        ValueWithSliderView {
                        vName: backend.valname[model.index]
                        vMax: backend.maxvalue
                        vMin: backend.minvalue
                        cVal: backend.values[model.index]
                        parentIdentifier: backend.identifier
                        vIdx: model.index
                        sourceObjectId: root.sourceObjectId
                    }
                }
            }

            Behavior on height {
                SequentialAnimation {
                    PropertyAction {
                        target: columnWrapper; property: "clip";
                        value: true
                    }
                    PropertyAction {
                        target: valueColumn; property: "visible";
                        value: true
                    }
                    NumberAnimation {
                        duration: 250
                        easing.type: Easing.InOutCubic
                    }
                    PropertyAction {
                        target: valueColumn; property: "visible";
                        value: valueColumn.showValues
                    }
                    PropertyAction {
                        target: columnWrapper; property: "clip";
                        value: false
                    }
                }
            }
        }
        MouseArea {
            id: mouse_ar
            anchors {
                top: contentRect.top
                left: contentRect.left
                right: contentRect.right
                bottom: columnWrapper.bottom
                rightMargin: 10
                topMargin: 5
                leftMargin: 5
                bottomMargin: root.viewCollapsed ? -5 : 0
            }

            drag.target: draggableItem
            hoverEnabled: true
            onContainsMouseChanged: backend.setMouseHover(containsMouse)
            onClicked: root.viewCollapsed = !root.viewCollapsed
        }
        Item {
            id: draggableItem
//            anchors.fill: contentRect
            Drag.active: mouse_ar.drag.active

            Drag.dragType: Drag.Automatic
            Drag.mimeData: {"text/plain": root.identifier,
            "sourceDevice": root.sourceObjectId,
                "oscInputPath": root.identifier,
            "nodeType": TypeHelper.Input,
            "valueType": root.valueType}
        }
    }
}
