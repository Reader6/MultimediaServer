import QtQuick 2.0

Rectangle{//背景
    id: root

    property bool running: false
    property int totalNumber : 0 //总份数,每秒一份
    property int currentNumber : 0 //总份数

    height: 4
    radius: height/2
    color: "#dbd7d7"

    signal seek(int index)
    signal stop()
    signal start()
    signal pause()
    signal restart()

    Rectangle{
        id: rect
        height: parent.height
        width: (totalNumber==0) ? 0: parent.width*currentNumber/totalNumber

        radius: parent.radius
        color: "#6cd6f9"
    }

    Timer{//控制进度条移动
        id: timer
        interval: 1000
        repeat: true
        onTriggered: {
            currentNumber += 1
            console.log("Progress -> Timer -> currentNumber " + currentNumber)
            if(currentNumber == totalNumber){
                root.stop()
                console.log("Progress -> Timer -> 结束")
            }
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            if(totalNumber == 0){
                console.log("Progress -> seek -> 媒体未加载")
            }
            else{
                var ind = parseInt(mouseX/width * totalNumber)
                seek(ind)
            }
        }
    }

    onStart: {
        console.log("Progress -> onStart")
        running = true
        timer.start()
    }

    onStop:{
        console.log("Progress -> onStop")
        running = false
        timer.stop()
    }

    onPause: {
        console.log("Progress -> onPause")
        running = false
        timer.stop()
    }

    onRestart: {
        console.log("Progress -> onRestart")
        currentNumber = 0
        running = true
        timer.start()
    }

    onSeek: {
        console.log("Progress -> onSeek -> permillage = " + index)
        currentNumber = index
    }

    function init(dur){
        console.log("Progress -> init -> duration " +dur)
        totalNumber = parseInt(dur/1000)
        currentNumber = 0
    }
}
