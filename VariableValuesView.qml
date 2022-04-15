import QtQuick 2.0

Item {
    property var potus: [0, 1, 2]

    Column {
    Repeater {
        model: 10

        Loader {sourceComponent: rem}
    }
}
    Component {
        id: rem
        Rectangle {
        width: 20
        height: 20
        color: Qt.rgb(Math.random(), Math.random(), Math.random());
    }
    }
}
