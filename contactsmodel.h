#ifndef CONTACTSTMODEL_H
#define CONTACTSTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QMap>
#include "contact.h"
#include "chatmodel.h"

class ContactsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ContactsModel();
    ContactsModel(QObject *parent = nullptr);

    enum{
        IpAddressRole = Qt::UserRole,
        UserNameRole,
        IsActiveRole,
    };

    void addContact(Contact* contact);
    void removeContact(QString ipAddress);
    Contact* getContact(QString contactIp);
    void setUserName(QString contactIp,QString name);
    void setActiveContact(QString contactIp);
    void setActiveContact(Contact* contact);

    ChatModel* getChatModel();

    void addMessage(QString contactIp, Message mes);

private:
    QMap<QString,int> ipAssociation;
    QVector<Contact*> model;
    ChatModel* chatModel;

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // CONTACTSTMODEL_H
