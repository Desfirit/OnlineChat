import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Window {
    id: _window

    width: 740
    height: 610
    visible: true
    title: qsTr("Larion")

    Button {
        id: _connectButton
        width: 265
        height: 50
        x: 0
        y: 0
        text: "Добавить соединение"
        background: Rectangle{
            color: _connectButton.down ? "#eee" : "#fff"
            border.color: "#cccccc"
        }
        onClicked: _popup.open()
    }

    ConnectionDialog{
        id: _popup
    }

    ContactsView{
    }

    Chat{

    }


    Connections{
        target: App
    }
}
