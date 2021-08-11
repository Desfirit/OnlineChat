import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

Button{
    id: _contact
    property string userName: ""
    property string ipAddress: ""
    property string backgroundColor: "#fff"

    property alias optionsButton: _options

    height: 56
    background: Rectangle{
        color: _contact.down? "#b8d2ff" : backgroundColor
    }

    Image{
        id: _icon
        x: 27
        y: 16
        width: 24
        height: 24
        source: "Images/phone-book.svg"
        ColorOverlay {
                anchors.fill: _icon
                source: _icon
                color: _contact.down? "#eee" : "#cbdeff"
        }
    }


    Text{
        id: _name
        x: 60
        y: 10
        text: userName
        font: {
            pointSize: 20.0

        }
    }
    Text{
        id: _ipAddress
        x: 60
        y: 30
        text: ipAddress
        font: {
            pointSize: 20.0

        }
        color: "#800000FF"
    }

    Button{
        id: _options
        x: 241
        y: 22
        width: 13
        height: 15

        background: Image{
            id: _optionsIcon
            source: "Images/reorder-option.svg"
        }


        ColorOverlay{
            anchors.fill: _optionsIcon
            source: _optionsIcon
            color: _options.down ? "#aaa" : "#888"
        }



    }


}
