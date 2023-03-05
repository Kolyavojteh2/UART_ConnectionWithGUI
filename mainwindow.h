#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "TableModel_UART.h"
#include "TableProxyModel_UART_Hex.h"

#include <QSortFilterProxyModel>

#include "TableModel_UART_Input.h"

#include "My_SerialPortSettings_t.h"
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const My_SerialPortSettings_t& serialSettings, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    TableModel_UART _source_model;
    TableProxyModel_UART_Hex _hex_history_model;

    QSortFilterProxyModel _model_received;
    QSortFilterProxyModel _model_sended;

    TableProxyModel_UART_Hex _hex_model_received;
    TableProxyModel_UART_Hex _hex_model_sended;

    TableModel_UART_Input _input_model;

    QSerialPort _serial;

public slots:
    void sendMessage(const UART_Message_t& msg);
    void receiveMessage(const UART_Message_t& msg);
    void createInfoMessage(const QString& info);

    void updateSettings(const My_SerialPortSettings_t&);

private:
    void init_SerialPort(const My_SerialPortSettings_t& serialSettings);
    void init_Models();
    void init_TableViews();
    void init_InputViews();
    void init_Menu();
};
#endif // MAINWINDOW_H
