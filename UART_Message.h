#ifndef UART_MESSAGE_H
#define UART_MESSAGE_H

#include <QDateTime>
#include <QByteArray>
#include <QMap>

enum UART_Message_Constants {
    UART_CountVariables = 3,
};

struct UART_Message_t {
    enum MessageDirect {
        Received,
        Sended,
        InfoMessage
    };

    QDateTime date;
    MessageDirect direct;
    QByteArray message;
};

const QMap<int, const char*> MessageDirect_str = {
    qMakePair(UART_Message_t::Received, "Received"),
    qMakePair(UART_Message_t::Sended, "Sended"),
    qMakePair(UART_Message_t::InfoMessage, "InfoMessage")
};

#endif // UART_MESSAGE_H
