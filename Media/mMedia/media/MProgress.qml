import QtQuick 2.0

Rectangle{//背景
    id: root

    property bool running: false
    property int totalNumber : 0 //总份数,每秒一份
    property int currentNumber
    property int loadNumber

    height: 6
    radius: height/2
    color: "#dbd7d7"//灰

    signal seek(int index)

    Rectangle{
        id: loadRect
        height: parent.height
        width: (totalNumber==0) ? 0: parent.width*loadNumber/totalNumber

        radius: parent.radius
        color: "#6cd6f9"//蓝

    }
    Rectangle{
        id: rect
        height: parent.height
        width: (totalNumber==0) ? 0: parent.width*currentNumber/totalNumber

        radius: parent.radius
        color: "#e8cb8b"//橘
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            if(totalNumber == 0){
                console.log("Progress -> seek -> 媒体未加载")
            }
            else{
                var index = parseInt(mouseX/width * totalNumber)
                console.log("Progress -> seek : "+index)
                seek(index)
            }
        }
    }


    function init(dur){
        totalNumber = parseInt(dur/1000)
        currentNumber = 0
        console.log("Progress -> init -> totalNumber : " +totalNumber)
    }
}
