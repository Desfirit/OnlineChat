#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{
public:
    Message();
    Message(QString text, bool isMyMessage, QString time);

    QString text;
    bool isMyMessage;
    QString time;
};

#endif // MESSAGE_H
