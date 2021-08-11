#ifndef CONTACT_H
#define CONTACT_H

#include <QVector>
#include <message.h>

class Contact
{
public:
    Contact(QString ipAddress = QString(), QString name = "Unknown");


    QString ipAddress;
    QString name;

    QVector<Message> chat;

    void AddMessage(Message mes);

};

#endif // CONTACT_H
