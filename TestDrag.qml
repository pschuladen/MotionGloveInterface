import QtQuick 2.12

Item {
    width: 200; height: 200

    Rectangle {
        anchors.centerIn: parent
        width: texttt.implicitWidth + 20; height: texttt.implicitHeight + 10
        color: "green"
        radius: 5

        Drag.active: dragHandler.active
        Drag.dragType: Drag.Automatic
        Drag.supportedActions: Qt.CopyAction
        Drag.mimeData: {
            "text/plain": "Copied text"
        }

        Text {
            id: texttt
            anchors.centerIn: parent
            text: "Drag me"
        }

        DragHandler {
            id: dragHandler
            onActiveChanged:
                if (active) {
                    parent.grabToImage(function(result) {
                        parent.Drag.imageSource = result.url;
                    })
                }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2}D{i:3}D{i:1}
}
##^##*/
