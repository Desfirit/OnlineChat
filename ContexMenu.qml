import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Rectangle{
    property string contactIpAddress: ""

    id: _contexMenu
    width: 100
    height: 80

    border.color: "black";


    Button{
        id: _removeButton

        anchors.top: _contexMenu.top
        width: 100
        height: 40

        text: "Удалить контакт"

        background: Rectangle{
            color: _removeButton.down ? "#eee" : "#fff"
            border.color: "#cccccc"
        }
        onClicked: {
            App.removeContact(contactIpAddress);
            _contexMenu.destroy();
        }
        }

    Button{
        id: _reconnectButton

        anchors.top: _removeButton.bottom
        width: 100
        height: 40

        text: "Переподключиться"


        background: Rectangle{
            color: _reconnectButton.down ? "#eee" : "#fff"
            border.color: "#cccccc"
        }

        onClicked: {
            console.log("reconnect");
            _contexMenu.destroy();
        }
    }



    DropShadow {
            anchors.fill: _contexMenu
            cached: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 16
            color: "#80000000"
            source: _contexMenu
        }

    MouseArea{
        x: 0
        y: 0
        width: 100
        height: 80
        hoverEnabled: true
        acceptedButtons: Qt.NoButton
        propagateComposedEvents: true;
        onExited: {
            _contexMenu.destroy();
        }
    }
}


