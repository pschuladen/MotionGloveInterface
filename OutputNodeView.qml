import QtQuick
import QtQuick.Controls
import MotionGloveInterface

OutputNodeController {
    id: root

//    required property string uniqueId
//    Component.onCompleted: console.log("outputnode", uniqueId, outputAddress)

//    property string outputaddress: ""

//    property int valueType: 0
    z: 1

    height: outputAddressLabel.implicitHeight +10
    width: outputAddressLabel.implicitWidth + 18

    mouseHover: mArea.containsMouse
    property bool lightenView: mouseHover || mouseRemoteHover

    Rectangle {
        property color rectColor: "lightgrey"
        color: lightenView ? rectColor.lighter(1.5) : rectColor
        anchors.fill: parent
        border {width:1; color:"black"}
        radius: 2

    }


    Text {
        id: outputAddressLabel
        anchors {
            verticalCenter: root.verticalCenter
            right: root.right
            rightMargin: 4
        }
        text: root.outputAddress
        font.pixelSize: 10
        font.underline: true
        horizontalAlignment: Text.AlignRight
    }

    MouseArea {
        id: mArea
        drag.target: root
        hoverEnabled: true
        anchors.fill:root
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse) => {
                       if(mouse.button === Qt.RightButton) {
                           contextMenu.popup()
                       }
                   }
    }

    InputConnector {
        parentID: root.uniqueID

        anchors {
            horizontalCenter: root.left
            verticalCenter: outputAddressLabel.verticalCenter
        }

        vName: _typehelper.getStringForValueType(root.valueType)
        vType: root.valueType
//        vIdx: 0

        onYChanged: root.setInConOffsetAtIndex(0, y)

    }
    Menu {
        width: 80
        id: contextMenu
        MenuItem {
            text: "delete"
            onClicked: _mbackend.deleteObjectWithId(root.uniqueID)
        }
    }
}

