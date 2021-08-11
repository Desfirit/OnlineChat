import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Rectangle{
    id: _chat

    x: 264
    y: 0
    width: 476
    height: 610

    border.color: "#cccccc"
    gradient: Gradient {
            GradientStop { position: 0.0; color: "#6de08f" }
            GradientStop { position: 1.0; color: "#c1f2d0" }
        }


   ChatInfo{
       id: _chatInfo
       visible: userName != ""
       userName: chat_model.userName
       connectionStatus: chat_model.connectionStatus
   }

   ListView{
       anchors.left: _chat.left
       anchors.right: _chat.right

       anchors.top: _chatInfo.bottom
       anchors.topMargin: 10

       anchors.bottom: _input.top
       anchors.bottomMargin: 10
       clip: true

       spacing: 15

       model: chat_model

       ScrollBar.vertical: ScrollBar {
                   active: true
       }
       delegate: Message{

           anchors.right: if(model.isMyMessage) parent.right
           anchors.rightMargin: if(model.isMyMessage) 10

           anchors.left: if(!model.isMyMessage) parent.left
           anchors.leftMargin: if(!model.isMyMessage) 10
           textMessage: model.text
           timeText: model.time
           maxWidthMessage: parent.width/2
       }
   }

  Rectangle{
      visible: _chatInfo.connectionStatus == "Подключено"
      id: _input
      x: 7
      y: 552 + 23 - _inputEdit.contentHeight
      width: 462
      height: _inputEdit.contentHeight + 30
      radius: 5
      border.color: "black"
      TextInput{
          id: _inputEdit
          width: _input.width - 20 -13*2 - 33
          wrapMode: TextInput.Wrap
          font.pointSize: 10
          x: 10
          y: 15
          maximumLength: 768
          onAccepted: sendMessage()
          }

      Button{
          id: _sendButton
          x: 416
          anchors.verticalCenter: _input.verticalCenter
          height: 25
          width: 25
          background: Image{
              id: _sendButtonImage
              source: "Images/send-button.svg"
          }


          ColorOverlay{
              anchors.fill: _sendButtonImage
              source: _sendButtonImage
              color: _sendButton.down ? "#9ec2ff" : "#357fff"
          }

          onClicked: sendMessage();
      }

    }

  function sendMessage(){
      if(_inputEdit.text){
      App.on_sendButton_clicked(_inputEdit.text)
      _inputEdit.text = ""
      }
  }
}
