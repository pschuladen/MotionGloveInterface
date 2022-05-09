import QtQuick
import QtQuick.Layouts
import MotionGloveInterface

Item {
    Component.onCompleted: console.log("created view at idx", viewIdx)
    id: root

    property string uniqueID: ""

    property string oscAddress: controller.oscPaths[viewIdx]//"/output"
//    onOscAddressChanged:
    property OscViewController controller: null
    property int connectedType: TypeHelper.Undefined
    property int viewIdx: 0
    implicitHeight: 24
    implicitWidth: 120
    Rectangle {
        id: addressRect
        anchors.fill: parent
        anchors.margins: 2

        TextInput {
            id: outputPathTextInput
            anchors {
                fill: parent
                margins: 4
            }

            text: root.oscAddress
            validator: RegularExpressionValidator {
                regularExpression: /^\/.+/
            }
            font.pixelSize: 10
            horizontalAlignment: Text.AlignRight
            selectByMouse: true
            Keys.onReturnPressed: focus=false
            Keys.onEscapePressed: {
                text = root.controller.oscPaths[root.viewIdx]
                focus = false
            }
            onEditingFinished: root.controller.setOscPathAtIndex(text, root.viewIdx)//root.ipAddress = text
            onFocusChanged: {
                if(!focus) {text = root.controller.oscPaths[root.viewIdx]}
            }
        }
        InputConnector {
            anchors {
                verticalCenter: addressRect.verticalCenter
                horizontalCenter: addressRect.left
            }
            id: outConnector
            height: 10
            vType: controller.valueTypes[root.viewIdx]
            parentID: root.uniqueID
            vIdx: root.viewIdx
        }

//Component.onCompleted: controller.addOscPath(outputPathTextInput.text)//console.log("implicit dimension", implicitWidth, )
    }
}
