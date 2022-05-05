import QtQuick
import QtQuick.Layouts

Item {
    id: root
    implicitHeight: columLayout.implicitHeight
    implicitWidth: columLayout.implicitHeight
    Rectangle {
        id: columLayout
        height: childrenRect.height
        //        width: childrenRect.width
        Layout.preferredWidth: 120
        Layout.fillWidth: true




        Text {
            text: "ip"
        }
        TextInput {
            text: "127.0.0.1"
            validator: RegularExpressionValidator {
                regularExpression: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
            }
        }
        Component.onCompleted: console.log("this row width", width)



        Text {
            text: "port"
        }
        TextInput {

            text: "55123"
            validator: IntValidator {bottom: 1025;top: 65536}
        }
    }

}
