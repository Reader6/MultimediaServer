import QtQuick 2.0

Canvas{
    id: root
    width: 20
    height: width

    signal click()
    property bool running: false
    property color color: "white"
    QtObject{
        id: self
        property int sideLength: width/4 //三角形边长,矩形长
        property int triangleHeigth: sideLength*Math.sqrt(3)/2 //三角形高
        property int edgeCenterDistance: triangleHeigth/3 //边心距
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            click()
            console.log("PlayButton -> onclick -> isplay = " + running)
        }
    }

    onRunningChanged: {
        root.requestPaint()
    }

    onPaint: {
        var ctx = getContext("2d");
        ctx.clearRect(0,0,width,height)

        ctx.save()
        ctx.translate(width/2, height/2)

        ctx.beginPath()
        ctx.strokeStyle = color
        ctx.lineWidth = width/12
        ctx.arc(0,0,width/2-2,0,Math.PI*2)
        ctx.stroke()

        if(!running){
            ctx.beginPath()
            ctx.fillStyle = color
            ctx.rotate(Math.PI/2)
            ctx.moveTo(-self.sideLength/2, self.edgeCenterDistance)
            ctx.lineTo(self.sideLength/2, self.edgeCenterDistance)
            ctx.lineTo(0, self.edgeCenterDistance-self.triangleHeigth)
            ctx.lineTo(-self.sideLength/2, self.edgeCenterDistance)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
        }
        else{
            ctx.beginPath()
            var len = self.sideLength
            ctx.fillStyle = color
            ctx.fillRect(-len/2,-len/2,len/3 ,len)
            ctx.fillRect(len/6,-len/2,len/3,len)
            ctx.stroke()
        }
        ctx.restore()
    }
}
