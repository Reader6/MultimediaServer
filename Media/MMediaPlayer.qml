import QtQuick 2.0
import QtMultimedia 5.5

Rectangle{
    color: "black"

    property alias source: player.source

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onMouseXChanged: {
            animation1.start()
            hoverEnabled = false
            timer.start()
        }
    }
    MediaPlayer {
        id: player;
        autoPlay: false

        onStatusChanged: {
            if(status == MediaPlayer.Loaded)
                controlBar.progress.init(player.duration)
        }

        onSourceChanged: {
            controlBar.playButton.running = false
            controlBar.progress.stop()
        }

        Component.onCompleted: {
            if(source != "" && status == MediaPlayer.Loaded)
               controlBar.progress.init(player.duration)
        }
    }
    VideoOutput {
        id: video
        anchors.fill: parent
        source: player;
    }
    ControlMediaBar{
        id: controlBar
        width: parent.width
        height: 20
        opacity: 0
        anchors.bottom: parent.bottom
    }

    Connections{//控制启动按钮
        target: controlBar.playButton
        onClick:{
            function buAvailable(status){
                return status === MediaPlayer.Loaded ||
                        status === MediaPlayer.Buffered ||
                         status === MediaPlayer.EndOfMedia
            }

            if(buAvailable(player.status)){

                if(!controlBar.playButton.running) {
                    if(player.status == MediaPlayer.EndOfMedia){
                        controlBar.progress.init(player.duration)
                        player.seek(0)
                    }
                    controlBar.progress.start()
                    player.play()
                }
                else{
                    player.pause()
                    controlBar.progress.pause()
                }
                controlBar.playButton.running =
                        !controlBar.playButton.running
            }
        }
    }
    Connections{//控制声音按钮
        target: controlBar.voiceButton
        onClick:{
            player.muted = controlBar.voiceButton.running
            controlBar.voiceButton.running =
                    !controlBar.voiceButton.running
        }
    }
    Connections{//视频seek
        target: controlBar.progress
        onSeek:{
            player.seek(index*1000)
        }
    }


    PropertyAnimation{
        id:animation1//显示
        target: controlBar
        property: "opacity"
        to: 1
        duration: 1000
    }
    PropertyAnimation{
        id:animation2//隐藏
        target: controlBar
        property: "opacity"
        to: 0
        duration: 1000
    }

    Timer{
        id : timer
        onTriggered: {
            animation2.start()
            mouseArea.hoverEnabled = true
        }
    }

    function stop(){
        player.stop()
        controlBar.playButton.running = false
        controlBar.progress.stop()
    }
}
