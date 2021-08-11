#include "contactsmodel.h"
#include "contact.h"
#include <QtDebug>

ContactsModel::ContactsModel()
{
    chatModel = new ChatModel(this);
}

ContactsModel::ContactsModel(QObject *parent): QAbstractListModel(parent)
{
    chatModel = new ChatModel(this);
}

void ContactsModel::addContact(Contact* contact)
{
    beginInsertRows(QModelIndex(), model.size(), model.size());

    model.append(contact);
    ipAssociation.insert(contact->ipAddress, model.size()-1);
    //chatModel->setContact(contact);

    endInsertRows();

    if(model.size() < 2) //Да, это костыль, но оно того стоит
        chatModel->setContact(contact);
    else
        setActiveContact(contact);
}

void ContactsModel::removeContact(QString ipAddress)
{
    int index = ipAssociation[ipAddress];
    beginRemoveRows(QModelIndex(),index,index);

    model.remove(index);
    ipAssociation.remove(ipAddress);

    for(auto it = ipAssociation.begin(); it != ipAssociation.end(); ++it)
    {
        if((*it) > index)
            --(*it);
    }

    endRemoveRows();
}

Contact* ContactsModel::getContact(QString contactIp)
{
    if(ipAssociation.contains(contactIp))
        return model[ipAssociation[contactIp]];
    else
        return nullptr;

}

void ContactsModel::setUserName(QString contactIp, QString name)
{
    model[ipAssociation[contactIp]]->name = name;
    if(model[ipAssociation[contactIp]] == chatModel->getContact())
        chatModel->userNameChanged();

    QModelIndex changedIndex = createIndex(ipAssociation[contactIp],0);
    dataChanged(changedIndex,changedIndex);
}

void ContactsModel::setActiveContact(QString contactIp)
{
    QModelIndex oldIndex;
    if(chatModel->getContact())
        oldIndex = createIndex(ipAssociation[chatModel->getContact()->ipAddress],0);
    else
        oldIndex = createIndex(model.size(),0);

    chatModel->setContact(model[ipAssociation[contactIp]]);


    QModelIndex newIndex = createIndex(ipAssociation[contactIp],0);
    if(oldIndex < newIndex)
        dataChanged(oldIndex,newIndex);
    else
        dataChanged(newIndex,oldIndex);
}

void ContactsModel::setActiveContact(Contact *contact)
{
    QModelIndex oldIndex;
    if(chatModel->getContact())
        oldIndex = createIndex(ipAssociation[chatModel->getContact()->ipAddress],0);
    else
        oldIndex = createIndex(model.size(),0);

    chatModel->setContact(contact);

    QModelIndex newIndex = createIndex(ipAssociation[contact->ipAddress],0);
    if(oldIndex < newIndex)
        dataChanged(oldIndex,newIndex);
    else
        dataChanged(newIndex,oldIndex);
}

ChatModel *ContactsModel::getChatModel()
{
    return chatModel;
}

void ContactsModel::addMessage(QString contactIp, Message mes)
{
    if(model[ipAssociation[contactIp]] == chatModel->getContact())
        chatModel->addMessage(mes);
    else
        model[ipAssociation[contactIp]]->AddMessage(mes);
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch (role) {
        case IpAddressRole:
            return QVariant(model.at(index.row())->ipAddress);
        case UserNameRole:
            return QVariant(model.at(index.row())->name);
        case IsActiveRole:
            return chatModel->getContact() == model[index.row()];

    }

    return QVariant();
}


int ContactsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return model.size();
}

QHash<int, QByteArray> ContactsModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[IpAddressRole] = "ipAddress";
    names[UserNameRole] = "userName";
    names[IsActiveRole] = "isActive";
    return names;
}


Qt::ItemFlags ContactsModel::flags(const QModelIndex &index) const
{
        return Qt::NoItemFlags;
}
