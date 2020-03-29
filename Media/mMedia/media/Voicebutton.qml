import QtQuick 2.0

Canvas{
    id: root
    width: 20
    height: width

    property color color: "white"
    property bool running: true
    signal click()

    MouseArea{
        anchors.fill: parent
        onClicked: {
            click()
            console.log("VoiceButton -> onclick -> isClose = " + running)
        }
    }

    onRunningChanged: {
        root.requestPaint()
    }

    onPaint: {
        var ctx = getContext("2d");
        ctx.clearRect(0,0,width,height)

        function draw(ctx, r1){
            ctx.beginPath()
            ctx.arc(0, width/2, r1, -Math.PI/6, Math.PI/6)
            ctx.stroke()
        }
        var r = width/10
        ctx.strokeStyle = color
        ctx.lineWidth = r

        draw(ctx, 9*r)
        draw(ctx, 7*r)

        var di = 4*r/Math.sqrt(3)
        ctx.beginPath()
        ctx.lineWidth = 2
        ctx.moveTo(0, width/2)
        ctx.lineTo(4*r, width/2 + di)
        ctx.lineTo(4*r, width/2 - di)
        ctx.closePath()
        ctx.fill()
        ctx.stroke()

        if(!running){
            ctx.beginPath()
            ctx.strokeStyle = "red"
            ctx.lineWidth = 2
            ctx.moveTo(0, 0)
            ctx.lineTo(width, width)
            ctx.moveTo(0, width)
            ctx.lineTo(width, 0)
            ctx.stroke()
        }
    }
}
