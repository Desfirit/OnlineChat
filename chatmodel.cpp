#include "chatmodel.h"
#include "contact.h"



ChatModel::ChatModel(QObject* parent): QAbstractListModel(parent)
{
}

ChatModel::ChatModel(Contact *model, QObject *parent):QAbstractListModel(parent)
{
    _contactModel = model;
}

void ChatModel::setContact(Contact *model)
{
    beginResetModel();
    _contactModel = model;
    endResetModel();
    userNameChanged();
}

Contact *ChatModel::getContact()
{
    return _contactModel;
}

void ChatModel::setConnectionStatus(QString connectionStatus)
{
    _connectionStatus = connectionStatus;
    connectionStatusChanged();
}

void ChatModel::setUserName(QString name)
{
    _contactModel->name = name;
    userNameChanged();
}

QString ChatModel::ipAddress()
{
    if(_contactModel)
        return _contactModel->ipAddress;
    else
        return "";
}

QString ChatModel::userName()
{
    if(_contactModel)
        return _contactModel->name;
    else
        return "";
}

QString ChatModel::connectionStatus()
{
    return _connectionStatus;
}

void ChatModel::addMessage(Message mes)
{
    if(_contactModel)
    {
        beginInsertRows(QModelIndex(), _contactModel->chat.size(), _contactModel->chat.size());
        _contactModel->chat.append(mes);
        endInsertRows();
    }
}


int ChatModel::rowCount(const QModelIndex &parent) const
{
    if(_contactModel)
        return _contactModel->chat.size();
    else
        return 0;
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !_contactModel)
        return QVariant();

    switch (role) {
        case TextRole:
            return _contactModel->chat.at(index.row()).text;
        case IsMyMessage:
            return _contactModel->chat.at(index.row()).isMyMessage;
        case TimeRole:
            return _contactModel->chat.at(index.row()).time;
    }

    return QVariant();
}


QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[TextRole] = "text";
    names[IsMyMessage] = "isMyMessage";
    names[TimeRole] = "time";

    return names;
}


Qt::ItemFlags ChatModel::flags(const QModelIndex &index) const
{
        return Qt::NoItemFlags;
}
