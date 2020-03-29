import QtQuick 2.0

Item {
    implicitHeight: 20
    width: parent.width
    height: 20

    property alias progress: __progress
    property alias playButton: __playButton
    property alias voiceButton: __voiceButton

    PlayButton{
        id: __playButton
        width: 20
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
    }
    Voicebutton{
        id:__voiceButton
        width: 20

        anchors.left: __playButton.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
    }
    Text {
        id: __text
        color: "white"
        anchors.left:  __voiceButton.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        text: retime(__progress.currentNumber) +
              " / " + retime(__progress.totalNumber)
    }
    MProgress{
        id: __progress
        anchors.left:  __text.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        anchors.right:  parent.right
        anchors.rightMargin: 5
    }

    function retime(time){
        function timeToStr(time){
            return time<10 ? "0"+time : time
        }
        var sec = time % 60;
        var min = parseInt(time/60)
        return qsTr(timeToStr(min) + ":" + timeToStr(sec))
    }

/*
Slide{//调节音量
    stepSize: 0.01
}
*/

}
