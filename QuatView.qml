import QtQuick 2.0
import QtQuick.Controls 2.5
import MotionGloveInterface

Item {
    id:root

    width: valueColumn.implicitWidth
    height: valueColumn.implicitHeight + 10

//    onXChanged: {
//        if (x+width > parent.width) {//parentScrollview.contentWidth) {
////            parentScrollview.contentWidth = x+width;
//            parent.width = x + width
//        }
//    }
//    onYChanged: {
//        if (y+height > parent.height) {//parentScrollview.contentWidth) {
////            parentScrollview.contentWidth = x+width;
//            parent.height = y + height
//        }
//    }

    property int sensIndex: 0
    property string sensorName: "acc/1"

    QuatViewBackend {
        id:backen
        objectName: "quatBackend"
    }


    Rectangle {
        id: valueArea
        color: "#919191"
        radius: 5
        border.width: 2

        anchors {
            fill: parent
            //        margins: 5
        }

        Item {
            anchors {
                fill: parent
                margins: 5
            }
            MouseArea {
                anchors.fill: parent
                drag.target: root
                drag.minimumX: 0
                drag.minimumY: 0
            }

            Column {

                id:valueColumn
                spacing: 10

                Text {
                    id: sensorLabel
                    text: sensorName
                }
                //                Slider {
                //                    value: bake.xValue
                //                }

                //                var first = valueRowComponent.createObject(parent)
                //                first.value: _vecData.x


                ValueWithSliderView {
                    cVal: backen.xValue
                    vName: "X"
                    cColor: "red"
                }
                ValueWithSliderView {
                    cVal: backen.yValue
                    vName: "Y"
                    cColor: "green"
                }
                ValueWithSliderView {
                    cVal: backen.zValue
                    vName: "Z"
                    cColor: "blue"
                }
                ValueWithSliderView {
                    cVal: backen.wValue
                    vName: "W"
                    cColor: "yellow"
                }

                //                Repeater {
                //                    model: vectorData
                //                    delegate: valueRowComponent
                //                }

            }
        }

    }
}
