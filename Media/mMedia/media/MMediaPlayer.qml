import QtQuick 2.0
import QtQuick.Dialogs 1.2
import QtMultimedia 5.5

Rectangle{
    id: root
    color: "black"

    property string source : "";//访问视频的编号

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onMouseXChanged: {
            animation1.start()
            hoverEnabled = false
            conBarTimer.start()
        }
    }

    MediaPlayer {
        //文件下载被保存的根目录
        property string rootDir: "file://" + mediaSourse.absDir()
        property int tsLen//Ts文件数量
        property int currentTs//正在播放第几个Ts文件
        property int loadTs//最大哪个Ts文件已经载

        id: player;
        autoLoad: true

        onStatusChanged: {
            if(status == MediaPlayer.Loaded){
                controlBar.progress.init(player.duration)
                controlBar.playButton.click()
            }
            else if(status == MediaPlayer.InvalidMedia){
                playErrDig.open()
            }
        }

        onStopped: {
            controlBar.playButton.running = false
            loadTimer.stop()
        }

        function loadTsfile(curTs){
            for(var i=0; i<3 && curTs<tsLen ; i++){
                curTs += 1
                if(mediaSourse.loadTsFile(curTs) === 0) {
                    loadTs = curTs
                }
            }
        }

        onLoadTsChanged: {
            console.log("player.loadTs = ind" + loadTs)
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
    Binding {
        target: controlBar
        property: "progress.currentNumber"
        value: parseInt(player.position/1000)
    }
    Binding {
        target: controlBar
        property: "progress.loadNumber"
        value: (player.loadTs === -1)? 0 : mediaSourse.getDuration(player.loadTs) / 1000
    }

    Connections{//控制启动按钮
        target: controlBar.playButton
        onClick:{
            if(player.status === MediaPlayer.Loaded ||
               player.status === MediaPlayer.Buffered ||
               player.status === MediaPlayer.EndOfMedia ||
               player.status === MediaPlayer.InvalidMedia)
            {

                if(!controlBar.playButton.running)
                {
                    if(player.status == MediaPlayer.EndOfMedia ||
                       player.status === MediaPlayer.InvalidMedia)
                    {//重播
                        controlBar.progress.init(player.duration)
                        player.seek(0)
                    }
                    player.play()
                    loadTimer.start()
                }
                else{
                    player.pause()
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
            var pos = index*1000
            var ind = mediaSourse.poslocation(pos)
            if(ind === 6){
                console.log("66666")
            }

            if(mediaSourse.isTsFileOk(ind) === false){
                player.loadTsfile(ind)
            }
            player.currentTs = ind;
            player.seek(pos)
        }
    }

    PropertyAnimation{
        id:animation1
        target: controlBar
        property: "opacity"
        to: 1
        duration: 1000
    }
    PropertyAnimation{
        id:animation2
        target: controlBar
        property: "opacity"
        to: 0
        duration: 1000
    }

    Timer{
        id : conBarTimer
        onTriggered: {
            animation2.start()
            mouseArea.hoverEnabled = true
        }
    }
    Timer{
        id : loadTimer
        interval: 2000
        repeat: true
        onTriggered: {

            var curind = mediaSourse.poslocation(player.position)
            console.log("curind " + curind)

            if(curind === mediaSourse.length-1){
                loadTimer.stop()
                console.log("timer stop ")
            }

            if(curind > player.currentTs){
                player.currentTs = curind
                console.log("currentTs " + player.currentTs)
                player.loadTsfile(curind)
                console.log("loadTsfile")
            }
        }
    }

    function init(){
        console.log("init " + source)
        if(source === "" || source === undefined){
            console.log("未设置源")
            return
        }

        var url = player.rootDir + "/" + source + "/index.m3u8"
        if(!mediaSourse.loadSourse(source)){
            console.log("mediaSourse.loadSourse false")
            return
        }
        player.source = url
        controlBar.progress.init(player.duration)

        if((player.tsLen = mediaSourse.length) != -1){
            console.log("init media ok")
            player.currentTs = -1;
            player.loadTs = -1;
            player.loadTsfile(0);
        }
    }

    onSourceChanged: {
        console.log("onSourceChanged " + source)
        player.stop()
        loadTimer.stop()
        if(source !== ""){
            mediaSourse.clear()
            init()
        }
    }

    Dialog{
        id: playErrDig
        title: "Blue sky dialog"

        contentItem: Rectangle {
            color: Qt.rgba(0.58, 0.61, 0.61, 1)
            implicitWidth: 400
            implicitHeight: 100
            Text {
                text: qsTr("媒体无法播放!")
                color: "navy"
                anchors.centerIn: parent
            }
        }
        modality: Qt.NonModal
    }
}
