import QtQuick
import MotionGloveInterface
import QtQuick.Shapes

ConnectionViewController {

    id: root
//z: -1

    Shape {
        anchors.fill: root

        ShapePath {
            id: spath
            strokeColor: _typehelper.getColorForValueType(root.valueType)
            strokeWidth: 3
            fillColor: "transparent"
            property int ctlPointOff: 35

            startX: root.targetXhigher ? 0 : root.width;
            startY: root.targetYhigher ? 0 : root.height
            PathCubic {
                x: root.targetXhigher ? root.width : 0
                y: root.targetYhigher ? root.height : 0
                control1X: spath.startX + spath.ctlPointOff
                control1Y: spath.startY
                control2X: x - spath.ctlPointOff
                control2Y: y
            }
        }
    }
}
