import QtQuick
import QtQuick.Controls

Item {
    id: root
//    Layout.leftMargin: inputRect.controlMargins
//    Layout.rightMargin: inputRect.controlMargins
//    Layout.fillWidth: true
    property alias value: tin.value
    property real minValue: -20000
    property real maxValue: 20000
    height: 12
    implicitWidth: borderRect.implicitWidth

    Rectangle {
        id: borderRect
        anchors.fill: parent

        border {color: "black"; width:1}
        //childrenRect.height
//                    MouseArea {
//                        anchors.fill: parent
//                        onClicked: console.log("click")
//                        Component.createObject: TextField
//                    }

        TextInput {
            id: tin
            text: "0"
            onTextChanged: console.log("text chagned", text)
            property real value: 0
            onValueChanged: console.log("value is", value)
            font.pixelSize: 10
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            selectByMouse: true

            Keys.onReturnPressed: focus=false
            Keys.onEscapePressed: {
                text = value
                focus=false
            }

            onEditingFinished: {
                value = text
            }
            onFocusChanged: {
                if(!focus) { text = tin.value}
            }
            selectionColor: "lightblue"

            validator: DoubleValidator {
                bottom: root.minValue
                top: root.maxValue
            }
        }
    }

}
