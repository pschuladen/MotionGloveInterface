import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts

Item {

    id: root
    property bool connectDevice: connectToDeviceButton.checked
    property string deviceName: "no Name"

    signal connectButtonChanged(connect: bool, deviceN: string);
    //    slot showPingReceived()

    //    Layout.preferredHeight: 80
    //    Layout.preferredWidth: 120
    width: 120
    height: 80
    Rectangle {

        //        Layout.preferredWidth: 120
        //        Layout.preferredHeight: 80
        anchors.fill: parent

        color: "darkgrey"
        border {
            width: 2
            color: "black"
        }

        Button {
            id: connectToDeviceButton
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 4
            }
            text: "con"
            bottomPadding: 8
            topPadding: 4
            leftPadding: 2
            rightPadding: 2
            checkable: true
            onCheckedChanged: root.connectButtonChanged(checked, deviceName);


        }
        Text {
            id: deviceNameLabel
            text: deviceName
            color: "white"
            anchors {
                left: parent.left
                top: parent.top
                margins: 4
            }
        }
    }

}
