import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: root

    property real cVal: 0.23
    property string vName: "vv"
    property color cColor: "black"
    property real vMax: 1
    property real vMin: -1

    height: row.implicitHeight
    width: row.implicitWidth



    Row {
        id: row
        Text {

            //                id: xNameLabel
            //                text: model.label
            width: 10
            text: vName
        }
        Text {
            //                id: xValueLabel
            width: 35
            //                text: model.value.toFixed(2)
            text: cVal.toFixed(2)
            horizontalAlignment: Text.AlignRight//Number.toString(Math.round(xVal * 100)/100)
        }
        Slider {
            enabled: true
            value: cVal
            from: vMin
            to: vMax
        }
        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            //anchors.topMargin: 10
            height: 10
            width: 10
            radius: 5
            color: cColor
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    console.log("clicked on", root.vName)
                }
                onReleased: {
                    console.log("released", root.vName)
                }
//                drag.hotSpot: 2
            }
//            DropArea {
//                onDropped: console.log("Dropped")
//            }
        }
    }
}
