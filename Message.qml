import QtQuick 2.15

Rectangle{
    id: _message

    property string textMessage: ""
    property string timeText: ""
    property real maxWidthMessage: 0


    width: _textMessage.width + 20
    height: _textMessage.height + 20
    radius: 4
    border.color: "black"
    color: "white"
    Text{
        anchors.centerIn: _message
        id: _textMessage
        width: contentWidth > maxWidthMessage? maxWidthMessage : contentWidth
        wrapMode: TextInput.Wrap
        text: textMessage
        font.pointSize: 11
    }

    Text{
        anchors.top: _message.bottom
        anchors.bottomMargin: 2
        anchors.right: _message.right
        id: _timeText
        text: timeText
        font.pointSize: 6
        //color: "#800000aa"
    }
}
