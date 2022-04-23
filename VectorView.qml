import QtQuick 2.0
import QtQuick.Controls 2.5
//import VectorViewBackend
import MotionGloveInterface
//import VectorViewBackend


Item {
    id:root

//    property ScrollView parentScrollview: parent.parScroll

    width: valueColumn.implicitWidth
    height: valueColumn.implicitHeight + 10
    Component.onCompleted: console.log(width, "is the width")

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

    VectorViewBackend {
        id:backen
        objectName: "vectBackend"
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

                //                Repeater {
                //                    model: vectorData
                //                    delegate: valueRowComponent
                //                }

            }
        }
    }
    Component {
        id: valueRowComponent
        Item {
            id:item

            property real value: 0.23
            property string vName: "vv"
            property color color: "black"
            property real vMax: 1
            property real vMin: -1

            Row {
                Text {
                    //                id: xNameLabel
                    //                text: model.label
                    text: valueRowComponent.vName
                }
                Text {
                    //                id: xValueLabel
                    width: 35
                    //                text: model.value.toFixed(2)
                    text: valueRowComponent.value.toFixed(2)
                    horizontalAlignment: Text.AlignRight//Number.toString(Math.round(xVal * 100)/100)
                }
                Slider {
                    enabled: true
                    value: valueRowComponent.value
                    from: vMin
                    to: vMax

                }
                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    //anchors.topMargin: 10
                    height: 10
                    width: 10
                    radius: 5
                    color: model.color
                }
            }
        }
    }
    ListModel {
        id: vectorData
        //        ListElement {
        //            label: "X"
        //            value: _vecData.x
        //            color: "red"
        //        }
        //        ListElement {
        //            label: "Y"
        //            value: 0.4//vect.y
        //            color: "green"
        //        }
        //        ListElement {
        //            label: "Z"
        //            value: 0//vect.z
        //            color: "blue"
        //        }
    }
}
