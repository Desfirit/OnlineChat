#include "message.h"

Message::Message()
{

}

Message::Message(QString text, bool isMyMessage, QString time)
{
    this->text = text;
    this->isMyMessage = isMyMessage;
    this->time = time;
}
