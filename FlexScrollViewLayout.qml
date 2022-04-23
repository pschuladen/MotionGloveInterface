import QtQuick 2.3
import QtQuick.Controls 2.5

Item {

//    height: 300
//    width: 300

//    Rectangle {

//        color: "cornflowerblue" //"darkblue"
//        border {
//            color: "blanchedalmond"
//            width: 4
//        }
//        radius: 2
//        anchors.fill: parent

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
//                    console.log("muggauga")
                }
                width:  childrenRect.width + childrenRect.x + 10
                height: childrenRect.height + childrenRect.y + 10
            }
        }
//    }
}
