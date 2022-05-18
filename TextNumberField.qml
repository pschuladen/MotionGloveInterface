import QtQuick
import QtQuick.Controls

Item {
    id: root
//    Layout.leftMargin: inputRect.controlMargins
//    Layout.rightMargin: inputRect.controlMargins
//    Layout.fillWidth: true
    property real value: 0
    property real minValue: -20000
    property real maxValue: 20000
    height: 12
    implicitWidth: borderRect.implicitWidth

    Rectangle {
        id: borderRect
        anchors.fill: parent

        border {color: "black"; width:1}

        TextInput {
            id: tin
            text: root.value

            font.pixelSize: 10
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            selectByMouse: true

            Keys.onReturnPressed: focus=false
            Keys.onEscapePressed: {
                text = root.value
                focus=false
            }

            onEditingFinished: {
                root.value = text
            }
            onFocusChanged: {
                if(!focus) { text = root.value}
            }
            selectionColor: "lightblue"

            validator: DoubleValidator {
                bottom: root.minValue
                top: root.maxValue
            }
        }
    }

}
