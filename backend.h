#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <contactsmodel.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtSql/QSqlDatabase>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    ContactsModel* contactsModel;
    ChatModel* chatModel;

    Q_INVOKABLE void on_connectionDialog_closed(const QString ipAddress);
    Q_INVOKABLE void on_sendButton_clicked(QString message);
    Q_INVOKABLE void openContact(QString ipAddress);
    Q_INVOKABLE void removeContact(QString ipAddress);
    Q_INVOKABLE void reconnectContact(QString ipAddress);

private slots:
    void incomming_connection();
    void ready_read();
    void on_successful_connection();
    void ready_read_connection_params();
    void on_disconnected();


private:
    const quint16 defaultPort = 25565;
    QTcpServer* server;
    QList<QTcpSocket*> connections;
    QString _userName;
    QSqlDatabase _database;


    void make_connection(const QString ipAddress);
    void getUserName();
    void startServer();
    bool checkIpAddress(QString ipAddress);
    QTcpSocket* findSocket(QString ipAddress);
    QTcpSocket* getCurrentSocketSender();

    bool openDatabase(QString database, QString databaseName);
    bool checkTables(QList<QString>& tables) const;
    bool createDatabaseTables();
    ContactsModel* loadDataFromDatabase();
    bool saveMessage(Message mes, QString ipAddress);
    bool saveContact(Contact* contact);
    bool updateUserName(QString ipAddress,QString name);
    bool deleteContact(QString ipAddress);


};

#endif // BACKEND_H
