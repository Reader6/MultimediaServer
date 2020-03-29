import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.5

import QtQuick.Controls 2.0

import "media"

Window {
    visible: true
    width: 640
    height: 600
    title: qsTr("Hello World")

    MMediaPlayer{
        id: media
        width: 640
        height: 480
        source: "2"
    }


    Button{
        id : _1
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        onClicked: {
            media.source = "1"
        }
    }
    Button{
        id : _2
        anchors.left: _1.right
        anchors.bottom: parent.bottom

        onClicked:{

        }
    }
}
