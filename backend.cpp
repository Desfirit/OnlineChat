#include "backend.h"
#include "contact.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcessEnvironment>
#include <QDebug>
#include <QTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

Backend::Backend(QObject *parent) : QObject(parent)
{
    getUserName();

    if(openDatabase("QSQLITE", "chat.db"))
    {
        qDebug() << "success open database";


        QList<QString> tables; //Можно как-то получше этот участок сделать?
        tables.append("contacts");
        tables.append("messages");


        if(checkTables(tables))
        {
            contactsModel = loadDataFromDatabase();
        }
        else
        {
            if(!createDatabaseTables())
            {
                qDebug() << "Exception: not full created database";
                _database.close();
            }
            contactsModel = new ContactsModel(this);
        }

    }

    startServer();

    connect(server, &QTcpServer::newConnection, this, &Backend::incomming_connection);
}

Backend::~Backend()
{
}

void Backend::on_connectionDialog_closed(const QString ipAddress)
{
    if(checkIpAddress(ipAddress))
        make_connection(ipAddress);
}

void Backend::incomming_connection()
{
    qDebug() << "new connection from user";
    QTcpSocket* newConnection = server->nextPendingConnection();
    connections.append(newConnection);

    Contact* contact = new Contact(newConnection->peerAddress().toString());
    contactsModel->addContact(contact);
    if(!saveContact(contact))
        qDebug() << "Failed save contact: " << contact->name << "ip: " << contact->ipAddress;

    connect(newConnection, &QTcpSocket::readyRead, this, &Backend::ready_read_connection_params);

    newConnection->write(_userName.toUtf8());
}

void Backend::ready_read()
{
    QTcpSocket * socket = getCurrentSocketSender();
    qDebug() << "Новое сообщение от " << socket->peerAddress();

    QString message(socket->readAll());
    qDebug() << message;

    Message mes(Message(message, false,QTime::currentTime().toString("h:m:s ap")));
    contactsModel->addMessage(socket->peerAddress().toString(), mes);
    if(!saveMessage(mes, socket->peerAddress().toString()))
        qDebug() << "Exception failed save message for contact: " << socket->peerAddress().toString();
}

void Backend::on_successful_connection()
{
    QTcpSocket * socket = getCurrentSocketSender();

    connections.append(socket);

    socket->write(_userName.toUtf8());

    connect(socket, &QTcpSocket::readyRead, this, &Backend::ready_read_connection_params);
    //disconnect(socket, &QTcpSocket::connected, this, &Backend::on_successful_connection);
}

void Backend::ready_read_connection_params()
{
    QTcpSocket * socket = getCurrentSocketSender();

    QString userName(socket->readAll());

    contactsModel->getChatModel()->setConnectionStatus("Подключено");
    contactsModel->setUserName(socket->peerAddress().toString(), userName);
    if(!updateUserName(socket->peerAddress().toString(), userName))
        qDebug() << "Exception: failed update username ip:" << socket->peerAddress().toString() << " new name: " << userName;

    disconnect(socket, &QTcpSocket::readyRead, this, &Backend::ready_read_connection_params);
    connect(socket, &QTcpSocket::readyRead, this, &Backend::ready_read);
    connect(socket, &QTcpSocket::disconnected, this, &Backend::on_disconnected);
}

void Backend::on_disconnected()
{
    QTcpSocket* socket = getCurrentSocketSender();
    if(connections.removeOne(socket))
    {
        qDebug() << "disconnected";

        if(contactsModel->getChatModel()->getContact() == contactsModel->getContact(socket->peerAddress().toString())) //Открыт ли чат
            contactsModel->getChatModel()->setConnectionStatus("Отключено");
        disconnect(socket, &QTcpSocket::readyRead, this, &Backend::ready_read);

        //delete socket; Почему из-за этого вылет?!
    }
}

void Backend::make_connection(const QString ipAddress)
{
    if(!contactsModel->getContact(ipAddress))
    {
        QTcpSocket* connection = new QTcpSocket(this);
        Contact* newContact = new Contact(ipAddress);

        contactsModel->addContact(newContact);
        contactsModel->getChatModel()->setConnectionStatus("Соединение...");

        if(!saveContact(newContact))
            qDebug() << "Failed save contact: " << newContact->name << "ip: " << newContact->ipAddress;

        connection->connectToHost(ipAddress, defaultPort);
        connect(connection, &QTcpSocket::connected, this, &Backend::on_successful_connection);
    }
}

void Backend::getUserName()
{
    QString name = QProcessEnvironment::systemEnvironment().value("USER");
    if(name.isEmpty())
        name = QProcessEnvironment::systemEnvironment().value("USERNAME");
    _userName = name.toUtf8();
}

void Backend::startServer()
{
    server = new QTcpServer(this);
    bool res = server->listen(QHostAddress::AnyIPv4, defaultPort);
    qDebug() << "server listen = " << res;
    if(!res)
    {
        qDebug() << "repeat server listen = " << server->listen(QHostAddress::AnyIPv4, defaultPort+1);
    }
}

bool Backend::checkIpAddress(QString ipAddress)
{
    QRegExp re("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    qDebug() << ipAddress;
    return  re.exactMatch(ipAddress);
}

QTcpSocket *Backend::findSocket(QString ipAddress)
{
    if(connections.empty())
        return nullptr;
    for(auto it = connections.cbegin(); it != connections.end(); ++it)
        if((*it)->peerAddress().toString() == ipAddress)
            return *it;

    return nullptr;
}

QTcpSocket *Backend::getCurrentSocketSender()
{
    QObject * object = QObject::sender();
    if (!object)
        return nullptr;
    return static_cast<QTcpSocket *>(object);
}

bool Backend::openDatabase(QString database, QString databaseName)
{
    _database = QSqlDatabase::addDatabase(database,"mainbase"); //Почему нет указателя???
    _database.setDatabaseName(databaseName);
    if(!_database.open()){
        qDebug() << "can't open database: " << database << " name:" << databaseName;
        qCritical() << _database.lastError().text();
        return false;
    }

    return true;
}

bool Backend::checkTables(QList<QString> &tables) const
{
    auto databaseTables = _database.tables();
    for(auto table = tables.cbegin(); table != tables.cend(); ++table)
    {
        if(!databaseTables.contains(*table))
            return false;
    }
    return true;
}

bool Backend::createDatabaseTables()
{
    QSqlQuery query(_database);
    if(!query.exec("PRAGMA foreign_keys=on"))
        qDebug() << "Failed set up foreign_keys";

    qDebug() << "Creating table contacts...";
    if(!query.exec("CREATE TABLE contacts("
               "ip_address varchar(15) NOT NULL PRIMARY KEY,"
               "name VARCHAR(32) NOT NULL);"))
    {
        qDebug() << "failed create table contacts";
        return false;
    }

    qDebug() << "Creating table messages...";
    if(!query.exec("CREATE TABLE messages("
                           "message_PK INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                           "message TEXT NOT NULL,"
                           "is_my_message Boolean NOT NULL,"
                           "contact_FK varchar(15) NOT NULL,"
                           "time TEXT NOT NULL,"
                           "FOREIGN KEY(contact_FK) REFERENCES contacts(ip_address) On DELETE CASCADE);"))
    {
        qDebug() << "failed create table messages";
        return false;
    }
    return true;
}

ContactsModel *Backend::loadDataFromDatabase()
{
    QSqlQuery contactQuery(_database);
    QSqlQuery messageQuery(_database);

    messageQuery.prepare("SELECT message, is_my_message, time FROM messages WHERE contact_FK = ?");

    contactQuery.exec("SELECT ip_address, ip_address, name FROM contacts");
    ContactsModel* contacts = new ContactsModel(this);
    while (contactQuery.next())
    {
        Contact* contact = new Contact(contactQuery.value("ip_address").toString(), contactQuery.value("name").toString());
        messageQuery.addBindValue(contactQuery.value("ip_address").toString());
        messageQuery.exec();
        while(messageQuery.next())
        {
            Message mes(messageQuery.value("message").toString(), messageQuery.value("is_my_message").toBool(), messageQuery.value("time").toString());
            contact->AddMessage(mes);
        }
        contacts->addContact(contact);
    }
    return contacts;
}

bool Backend::saveMessage(Message mes, QString ipAddress)
{
    if(!_database.isOpen())
        return false;
    QSqlQuery query(_database);
    qDebug() << "Saving message";
    query.prepare("INSERT INTO messages(message, is_my_message, contact_fk, time) VALUES (?,?,?,?)");
    query.addBindValue(mes.text);
    query.addBindValue(mes.isMyMessage);
    query.addBindValue(ipAddress);
    query.addBindValue(mes.time);

    return query.exec();
}

bool Backend::saveContact(Contact *contact)
{
    if(!_database.isOpen())
        return false;
    QSqlQuery query(_database);
    qDebug() << "Saving contact";

    query.prepare("INSERT INTO contacts (ip_address, name) VALUES (?,?)");
    query.addBindValue(contact->ipAddress);
    query.addBindValue(contact->name);
    return query.exec();
}

bool Backend::updateUserName(QString ipAddress, QString name)
{
    if(!_database.isOpen())
        return false;
    QSqlQuery query(_database);
    qDebug() << "Updating name: " << name << " ip:" << ipAddress;

    query.prepare("UPDATE contacts SET name = ? WHERE ip_address = ?");
    query.addBindValue(name);
    query.addBindValue(ipAddress);
    return query.exec();
}

bool Backend::deleteContact(QString ipAddress)
{
    if(!_database.isOpen())
        return false;
    QSqlQuery query(_database);
    qDebug() << "Deliting contact ip address: " << ipAddress;

    query.prepare("DELETE FROM contacts WHERE ip_address == ?");
    query.addBindValue(ipAddress);
    return query.exec();
}

void Backend::on_sendButton_clicked(QString message)
{
    QString ip = contactsModel->getChatModel()->getContact()->ipAddress;
    auto socketReciever = findSocket(ip);
    if(socketReciever)
    {
        socketReciever->write(message.toUtf8());
        Message mes(Message(message, true,QTime::currentTime().toString("h:m:s ap")));
        contactsModel->getChatModel()->addMessage(mes);
        if(!saveMessage(mes, ip))
            qDebug() << "Exception failed save message for contact: " << ip;
    }
}

void Backend::openContact(QString ipAddress)
{
    Contact* contact = contactsModel->getContact(ipAddress);
    ChatModel* chatmodel = contactsModel->getChatModel();
    if(contact == chatmodel->getContact())
        return;
    if(contact) //Хотя это вроде невозможно
    {

        contactsModel->setActiveContact(contact);

        QTcpSocket* contactSocket = findSocket(ipAddress);
        if(contactSocket)
        {

            if(contactSocket->isOpen())
                chatmodel->setConnectionStatus("Подключено");
            else
                chatmodel->setConnectionStatus("Соединение...");
        }
        else
        {
            qDebug() << "Socket doesn't exist";
            chatmodel->setConnectionStatus("Не подключено (Архивная запись)");
        }
    }
}

void Backend::removeContact(QString ipAddress)
{
    QTcpSocket* socket = findSocket(ipAddress);
    if(socket)
    {
        if(socket->isOpen())
            socket->disconnected();
        connections.removeOne(socket);
    }

    Contact* contact = contactsModel->getChatModel()->getContact();
    if(contact)
    {
        if(contact->ipAddress == ipAddress)
        {
            contactsModel->getChatModel()->setContact(nullptr);
        }
    }
    contactsModel->removeContact(ipAddress);

    if(deleteContact(ipAddress))
        qDebug() << "Contact: " << ipAddress << " deleted";
}

void Backend::reconnectContact(QString ipAddress)
{
    return;
}
