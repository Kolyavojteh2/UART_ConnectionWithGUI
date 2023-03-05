#ifndef MY_SERIALPORTSETTINGS_T_H
#define MY_SERIALPORTSETTINGS_T_H

#include <QSerialPort>

struct My_SerialPortSettings_t {
    qint32                      BaudRate;/*QSerialPort::BaudRate*/
    bool                        BreakEnabled;
    qint32                      DataBits;/*QSerialPort::DataBits*/
    QSerialPort::Direction      Direction;
    QSerialPort::FlowControl    FlowControl;
    QSerialPort::Parity         Parity;
    qint32                      Pinout;/*QSerialPort::PinoutSignal*/
    QSerialPort::StopBits       StopBits;

    QString                     Device;
};

#endif // MY_SERIALPORTSETTINGS_T_H
