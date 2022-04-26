import QtQuick
import QtQuick.Controls 2.5
import MotionGloveInterface

Item {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    objectName: identifier
    property alias viewmode: backend.viewmode
    property alias identifier: backend.nodeIdentifier
    property alias backendObject: backend
    //    required property ValueViewBackend backend
    property bool globalBool_valuesHidden: parent ? parent.valuesHiddenBool : false
    property bool viewCollapsed: true
//    onGlobalShowSensorChanged: console.log("global sensor show changed", globalShowSensor)

    ValueViewBackend {
        id: backend
        objectName: "valuebackend"
        emitvalues: !root.viewCollapsed && !root.globalBool_valuesHidden
    }

    Rectangle {
        id: contentRect
        width:  valueColumn.implicitWidth
        height: childrenRect.height + childrenRect.y + 5//collapseButton.checked ? heightForm : 2* collapseButton.y + collapseButton.height
        property real heightForm: columnWrapper.height + columnWrapper.y + 10
        radius: 2
        border {
            width: 1
            color: "black"
        }
        property color bgcolor: "lightgrey"
        color: mouse_ar.containsMouse ? bgcolor.lighter(1.4) : bgcolor
        clip: false

        Text {
            id: titleLable
            text: backend.nodeIdentifier
            font.pixelSize: 10
            font.underline: true
            anchors {
                left: contentRect.left
                leftMargin: 10
                top: contentRect.top
                topMargin: 10
//                verticalCenter: collapseButton.verticalCenter
            }

            //            Component.onCompleted: console.log(titleLable.font.pointSize, titleLable.font.pixelSize)
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

        Rectangle {
            color: "blue"
            width: 10
            height: width
            radius:width/2
            anchors {
                verticalCenter: titleLable.verticalCenter
                horizontalCenter: contentRect.right
            }
            border {
                width: 1
                color: "black"
            }
        }

        Item {
            id: columnWrapper
            clip: true
            anchors {
                left: contentRect.left
                leftMargin: 5
                top: titleLable.bottom
                topMargin: 10
//                bottom: contentRect.bottom
            }
            width: valueColumn.implicitWidth
            height: root.viewCollapsed ? 0 : valueColumn.implicitHeight

            Column {

                id: valueColumn
                property bool showValues: !root.viewCollapsed

    //            onShowValuesChanged: console.log(collapseButton.checked, showValues)
                visible: false //controled by height animation

                anchors.fill: parent

                Repeater {
                    model: backend.valueCount
                    delegate: ValueWithSliderView {
                        vName: backend.valname[model.index]
                        vMax: backend.maxvalue
                        vMin: backend.minvalue
                        cVal: backend.values[model.index]
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
            hoverEnabled: true

            onClicked: root.viewCollapsed = !root.viewCollapsed
        }
    }
}
