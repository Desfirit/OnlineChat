import QtQuick 2.15
import QtGraphicalEffects 1.0

Rectangle{
    id: _chatInfo

    property string userName: ""
    property string connectionStatus: "Соединение..."

    x: 0
    y: 0

    width: _chat.width
    height: 50

    border.color: _chat.border.color
    color: "#cbdeff"

    Rectangle{
        id: _userName
        anchors.left: _chatInfo.left
        anchors.leftMargin: 8
        anchors.verticalCenter: _chatInfo.verticalCenter
        //x: 8
        //y: 9
        width: 148
        height: 32

        color: "#e0e0e0"
        radius: 20
        Text {
            text: userName
            anchors.centerIn: _userName
            font: {
                pointSize: 14.0
            }
        }
    }

    DropShadow {
            anchors.fill: _userName
            cached: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 16
            color: "#80000000"
            source: _userName
        }

    Text{
        id: _connectionStatus
        anchors.right: _chatInfo.right
        anchors.rightMargin: 20
        anchors.verticalCenter: _chatInfo.verticalCenter
        text: connectionStatus
    }
}
