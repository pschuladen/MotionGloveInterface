import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: root

    property real cVal: 0.23
    property string vName: "vv"
    property color cColor: "red"
    property real vMax: 1
    property real vMin: -1
    property string parentIdentifier: "default"
    property string vIdentifier: parentIdentifier+"-"+vName

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
            color: cColor
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    console.log("clicked on", vIdentifier)
                }
                onReleased: {
                    console.log("released", root.vName)
                }
//                drag.hotSpot: 2
            }
            border {
                width: 1
                color: "black"
            }
//            DropArea {
//                onDropped: console.log("Dropped")
//            }
        }
    }
}
