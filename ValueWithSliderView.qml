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
    width: row.implicitWidth + 10



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

//        OutputConnector {
//            anchors.verticalCenter: row.verticalCenter
//            height: 10
//            width: 10
//            parentID: root.parentIdentifier
////            cColor: root.cColor
//            vIdx: root.vIdx
//            vName: root.vName
//            vType: TypeHelper.SingleValue
//        }
    }
}
