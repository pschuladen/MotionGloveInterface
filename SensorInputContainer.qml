import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id:root
    width: showhideButton.checked ? changesizeRect.x + changesizeRect.width : headerLine.childrenRect.width
    height: showhideButton.checked ? changesizeRect.y + changesizeRect.height : headerLine.implicitHeight
//    width: changesizeRect.x + changesizeRect.width
//    height:changesizeRect.y + changesizeRect.height

    Rectangle {
        id: contRect
        color: "cornflowerblue"
        border {
            width: 4
            color: "blanchedalmond"
        }
        anchors.fill: parent


        //        Column {
        //            anchors {
        //                left: parent.left
        //                right: parent.right
        //            }
        Rectangle {
            id: headerLine
            anchors {
                top:parent.top
                left: parent.left

                right: parent.right
                margins: parent.border.width

            }

            color: parent.color.lighter(1.2)
            height: childrenRect.height//sensorNameLabel.implicitHeight
            MouseArea {
                anchors.fill: parent
                drag {
                    target: root
                    minimumX: 5
                    minimumY: 5
                }

            }
            Text {
                id: sensorNameLabel
                text: root.objectName
                anchors {
                    left: parent.left
                    top: parent.top
                    margins: 10
                }
            }
            Button {
                id: showhideButton
                checkable: true
                text: checked ? "hide" : "show"
                checked: true
                anchors {
                    right: parent.right
                    margins: 5
                    verticalCenter: sensorNameLabel.verticalCenter
                }
            }
        }
        FlexScrollViewLayout {
            visible: showhideButton.checked
            anchors {
                margins: parent.border.width
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                top: headerLine.bottom
            }
        }
        //        }
    }
    Rectangle {
        id: changesizeRect
        color: "black"
        y: 200
        x: 150

        width: 8
        height: 8
        MouseArea {
            anchors.fill: parent

            drag
            {
                target: changesizeRect
//                active: showhideButton.checked
                minimumX: 20
                minimumY: 80
            }


        }
    }

}
