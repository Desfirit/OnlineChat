import QtQuick 2.15
import QtQuick.Controls 2.15

Popup{
    id: _popup
    anchors.centerIn: parent
    width: 280
    height: 183
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    contentItem: Item {
        id: item
        anchors.fill: _popup
        Text{
            x: 24
            y: 23
            text: "Введите ip-адрес"
            font.pointSize: 14
        }
        TextInput{
            id: _ipAddressEdit
            x: 24
            y: 79
            width: 100
            inputMask: "000.000.000.000;_"
            onAccepted: onOk()

        }
        Button{
            id: _acceptButton
            anchors.left:_cancelButton.right
            anchors.leftMargin: 9
            anchors.bottom: _cancelButton.bottom
            width: 45
            height: 36
            background: Rectangle{
                anchors.fill: _acceptButton
                color: _acceptButton.down? "#73b9f0" : "#2196f3"
            }
            palette.buttonText: "white"
            text: "OK"
            font.pointSize: 12

            onClicked: onOk()
        }
        Button{
            id: _cancelButton
            //x: 123
            //y: 130
            anchors.left: item.left
            anchors.leftMargin: 110
            anchors.bottom: item.bottom
            width: 92
            height: 36
            background: Rectangle{
                anchors.fill: _acceptButton
                color: _cancelButton.down? "#ccc" : "#fff"
                border.color: "black"
            }
            text: "Cancel"
            font.pointSize: 12
            onClicked: onCancel()
        }
    }

    function onCancel(){
        _ipAddressEdit.text = "";
        _popup.close()
    }

    function onOk(){
        App.on_connectionDialog_closed(_ipAddressEdit.text)
        _ipAddressEdit.text = "";
        _popup.close()
    }

    onOpened: {
        _ipAddressEdit.focus = true
        _ipAddressEdit.cursorPosition = 0
    }

}
