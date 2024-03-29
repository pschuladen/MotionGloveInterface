import QtQuick
import QtQuick.Controls
import MotionGloveInterface

InputNodeController {

    id: root
    z:1

//    required property string uniqueId

//    required property string inputPath

//    required property string sourceDevice//: "/fla/acc/0"

//    property int valueType: 0

    height: sourceAddressLabel.implicitHeight + 10
    width: sourceAddressLabel.implicitWidth + 18

    Rectangle {
        property color rectColor: "lightgrey"
        color: root.mouseHover ? rectColor.lighter(1.5) : rectColor
        anchors.fill: parent
        border {width:1; color:"black"}
        radius: 2

    }
    Text {
        id: sourceAddressLabel
        anchors {
            left: parent.left
            top: parent.top
            margins: 5
        }
        text: root.inputPath
        font.underline: true
        font.pixelSize: 10
    }

    MouseArea {
        id: mArea
        drag.target: root
        anchors.fill:root
        hoverEnabled: true
        onContainsMouseChanged: root.mouseHover = containsMouse
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse) => {
                       if(mouse.button === Qt.RightButton) {
                           contextMenu.popup()
                       }
                   }
    }

    OutputConnector {

        parentID: root.uniqueID

        anchors {
            horizontalCenter: root.right
            verticalCenter: sourceAddressLabel.verticalCenter
        }

        vName: _typehelper.getStringForValueType(vType)
        vType: root.valueType// _typehelper.valueTypeForSensor(viewmode)


        onYChanged: root.setOutConOffsetAtIndex(0, y)

//        vIdx: 0

    }
    Menu {
        width: 80
        id: contextMenu
        MenuItem {text: "delete"
            onClicked: _mbackend.deleteObjectWithId(root.uniqueID)
        }
    }
}
