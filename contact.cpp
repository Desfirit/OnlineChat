#include "contact.h"

Contact::Contact(QString ipAddress, QString name)
{
    this->ipAddress = ipAddress;
    this->name = name;
}

void Contact::AddMessage(Message mes)
{
    chat.append(mes);
}
