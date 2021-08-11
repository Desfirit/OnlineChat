import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{

    id: _contacts
    x: 0
    y: 50
    width: 265
    height: 560
    border.color: "#cccccc"
    color: "#cbdeff"

    ListView{
        id: _view

        anchors.fill: _contacts
        clip: true


        model: contacts_model

        ScrollBar.vertical: ScrollBar {
                    active: true
        }
        delegate: Contact{
            userName:  model.userName //contacts_model.name(index)
            ipAddress:  model.ipAddress //contacts_model.ipAddress(index)
            backgroundColor: model.isActive? "#7affff" : "#fff"
            width: _contacts.width
            onClicked: {
                App.openContact(ipAddress)
            }
            optionsButton.onClicked: {
                var el = Qt.createQmlObject("ContexMenu {}", _contacts.parent);
                el.x = optionsButton.x;
                el.y = optionsButton.y + _contacts.y + y;
                console.log(ipAddress);
                el.contactIpAddress = ipAddress;
            }
        }
    }


}
