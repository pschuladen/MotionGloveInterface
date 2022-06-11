import QtQuick
import MotionGloveInterface
import QtQuick.Shapes
import QtQuick.Controls

ConnectionViewController {

    id: root
//z: -1

antialiasing: true

    Shape {
        id: rshape
        anchors.fill: root
        property color cColor: _typehelper.getColorForValueType(root.valueType)

        property int sWidth: 10

        containsMode: Shape.FillContains




//layer.enabled: true
//layer.samples: 2
        ShapePath {
            id: visPath
            strokeColor: hoha.hovered ? rshape.cColor.lighter(1.5) : rshape.cColor
            strokeWidth: 3
            fillColor: "transparent"
            startX: root.targetXhigher ? 0 : root.width;
            startY: root.targetYhigher ? 0 : root.height
            PathCubic {
                x: root.targetXhigher ? root.width : 0
                y: root.targetYhigher ? root.height : 0
                control1X: spath.startX + spath.ctlPointOff
                control1Y: spath.startY - 5
                control2X: x - spath.ctlPointOff
                control2Y: y
            }
        }

        ShapePath {
            id: spath
            strokeColor: "transparent" //hoha.hovered ? rshape.cColor.lighter(1.5) : rshape.cColor//_typehelper.getColorForValueType(root.valueType)
            strokeWidth: 1
            fillColor: "transparent" //hoha.hovered ? rshape.cColor.lighter(1.5) : rshape.cColor
            property int ctlPointOff: 35



            startX: root.targetXhigher ? 0 : root.width;
            startY: root.targetYhigher ? 0 + rshape.sWidth : root.height + rshape.sWidth
            PathCubic {
                id:lowPa
                x: root.targetXhigher ? root.width : 0
                y: root.targetYhigher ? root.height + rshape.sWidth : 0 + rshape.sWidth
                control1X: spath.startX + spath.ctlPointOff
                control1Y: spath.startY + rshape.sWidth
                control2X: x - spath.ctlPointOff
                control2Y: y + rshape.sWidth
            }
            PathLine {
                id: rightPa
                x: lowPa.x
                y: lowPa.y - rshape.sWidth
            }
            PathCubic {
                id: hiPa
                x: spath.startX
                y: spath.startY - rshape.sWidth
                control1X: lowPa.control2X
                control1Y: lowPa.control2Y - rshape.sWidth
                control2X: lowPa.control1X
                control2Y: lowPa.control1Y - rshape.sWidth
            }
            PathLine {
                x: spath.startX
                y: spath.startY
            }
        }

        HoverHandler {
            id: hoha

//            onHoveredChanged: console.log("hoverrr")
        }
        TapHandler {
            id: ma
            acceptedButtons: Qt.RightButton
            onTapped: contextMenu.popup()//_mbackend.deleteConnectionWithId(root.uniqueID) //console.log("clickclick")
        }

    }
    Menu {
        width: 80
        id: contextMenu
        MenuItem {text: "delete"
            onClicked: _mbackend.deleteConnectionWithId(root.uniqueID)
        }
    }
}
