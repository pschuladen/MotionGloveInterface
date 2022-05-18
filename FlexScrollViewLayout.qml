import QtQuick 2.3
import QtQuick.Controls 2.5

Item {

        ScrollView {
            anchors.fill: parent
            contentHeight: objectContainer.height
            contentWidth: objectContainer.width

            Item {
                objectName: "objectContainer"
                id: objectContainer

                onChildrenChanged: {
                    if(children.length < 2)
                        return
                    var obj = children[children.length-1]
                    obj.y = childrenRect.height

                }
                width:  childrenRect.width + childrenRect.x + 10
                height: childrenRect.height + childrenRect.y + 10
            }
        }
}
