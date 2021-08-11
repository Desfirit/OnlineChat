#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <message.h>
#include <contact.h>

class ChatModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName NOTIFY userNameChanged)
    Q_PROPERTY(QString ipAddress READ ipAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
public:
    ChatModel(QObject* parent = nullptr);
    ChatModel(Contact* model, QObject* parent = nullptr);

    enum{
        TextRole = Qt::UserRole,
        IsMyMessage,
        TimeRole,
    };

    void setContact(Contact* model);
    Contact* getContact();
    void setConnectionStatus(QString connectionStatus);
    void setUserName(QString name);


    QString ipAddress();
    QString userName();
    QString connectionStatus();

    void addMessage(Message mes);
signals:
    void userNameChanged();
    void ipAddressChanged();
    void connectionStatusChanged();

private:
    Contact* _contactModel;
    QString _connectionStatus = "";

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // CHATMODEL_H
