#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QButtonGroup>

#include "ConnectionSettingsDialog.h"
#include <QMessageBox>

MainWindow::MainWindow(const My_SerialPortSettings_t& serialSettings, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _serial(serialSettings.Device)
{
    ui->setupUi(this);

    init_Menu();
    init_Models();
    init_TableViews();
    init_InputViews();

    // Serial connection
    init_SerialPort(serialSettings);
}

void MainWindow::init_Menu() {
    connect(ui->menu_Connection, &QMenu::triggered, [&](QAction* action) {
        if (action->text() != "Connection &Settings") {
            return;
        }

        ConnectionSettingsDialog *connection_dialog = new ConnectionSettingsDialog(this);

        connect(connection_dialog, &ConnectionSettingsDialog::settingsHaveBeenDefined,
                this, &MainWindow::updateSettings);

        connection_dialog->setModal(true);
        connection_dialog->disableDevice();
        connection_dialog->show();
    });

    connect(ui->menu_Connection, &QMenu::triggered, [&](QAction* action) {
        if (action->text() != "&Exit") {
            return;
        }

        this->close();
    });

    connect(ui->menu_Help, &QMenu::triggered, [&](QAction* action) {
        Q_UNUSED(action);
        QMessageBox::information(this, "About...", "<h2>Develouped by Kolya Vojtekh.</h2>");
    });
}

void MainWindow::updateSettings(const My_SerialPortSettings_t& settings) {
    if (_serial.baudRate() != settings.BaudRate) {
        createInfoMessage(QString("Changed BaudRate speed from %1 to %2 baud per second.")
                          .arg(_serial.baudRate())
                          .arg(settings.BaudRate));
        _serial.setBaudRate(settings.BaudRate, settings.Direction);
    }

    if (_serial.dataBits() != settings.DataBits) {
        createInfoMessage(QString("Changed data bits from %1 to %2 bits.")
                          .arg(_serial.dataBits())
                          .arg(settings.DataBits));
        _serial.setDataBits((QSerialPort::DataBits)settings.DataBits);
    }

    if (_serial.flowControl() != settings.FlowControl) {
        QString flowControl_str[] = {
            "NoFlowControl",
            "HardwareControl",
            "SoftwareControl"
        };

        QString from;
        for (unsigned int i = 0; i < (sizeof(flowControl_str) / sizeof(flowControl_str[0])); i++) {
            if (_serial.flowControl() == i) {
                from = flowControl_str[i];
                break;
            }
        }

        QString to;
        for (unsigned int i = 0; i < sizeof(flowControl_str) / sizeof(flowControl_str[0]); i++) {
            if (settings.FlowControl == i) {
                to = flowControl_str[i];
                break;
            }
        }

        createInfoMessage(QString("Changed FlowControl from %1 to %2.")
                          .arg(from)
                          .arg(to));
        _serial.setFlowControl(settings.FlowControl);
    }

    if (_serial.parity() != settings.Parity) {
        QString Parity_str[] = {
            "NoParity",
            "",
            "EvenParity",
            "OddParity",
            "SpaceParity",
            "MarkParity"
        };

        QString from;
        for (unsigned int i = 0; i < (sizeof(Parity_str) / sizeof(Parity_str[0])); i++) {
            if (_serial.parity() == i) {
                from = Parity_str[i];
                break;
            }
        }

        QString to;
        for (unsigned int i = 0; i < sizeof(Parity_str) / sizeof(Parity_str[0]); i++) {
            if (settings.Parity == i) {
                to = Parity_str[i];
                break;
            }
        }

        createInfoMessage(QString("Changed Parity from %1 to %2.")
                          .arg(from)
                          .arg(to));
        _serial.setFlowControl(settings.FlowControl);
    }

    if (_serial.stopBits() != settings.StopBits) {
        createInfoMessage(QString("Changed StopBits from %1 to %2 bits.")
                          .arg(_serial.stopBits())
                          .arg(settings.StopBits));
        _serial.setStopBits(settings.StopBits);
    }
}

void MainWindow::init_InputViews() {
    // Init input lineEdit and tableView
    connect(ui->p_lineEdit_SendLine, &QLineEdit::textEdited,
            &_input_model, &TableModel_UART_Input::setSendData);

    connect(&_input_model, &TableModel_UART_Input::TextChanged,
            ui->p_lineEdit_SendLine, &QLineEdit::setText);

    connect(ui->p_lineEdit_SendLine, &QLineEdit::returnPressed,
            ui->p_pushButton_Send, &QPushButton::click);
}

void MainWindow::init_TableViews() {
    // Init tableView ASCII History
    ui->p_tableView_History_ASCII->setModel(&_source_model);
    ui->p_tableView_History_ASCII->resizeColumnsToContents();
    ui->p_tableView_History_ASCII->resizeRowsToContents();

    // Init tableView HEX History
    ui->p_tableView_History_hex->setModel(&_hex_history_model);
    ui->p_tableView_History_hex->resizeColumnsToContents();
    ui->p_tableView_History_hex->resizeRowsToContents();


    // Init tableView ASCII Received
    ui->p_tableView_ReceivedData_ASCII->setModel(&_model_received);
    ui->p_tableView_ReceivedData_ASCII->resizeColumnsToContents();
    ui->p_tableView_ReceivedData_ASCII->resizeRowsToContents();
    ui->p_tableView_ReceivedData_ASCII->hideColumn(1);

    // Init tableView ASCII Sended
    ui->p_tableView_SendedData_ASCII->setModel(&_model_sended);
    ui->p_tableView_SendedData_ASCII->resizeColumnsToContents();
    ui->p_tableView_SendedData_ASCII->resizeRowsToContents();
    ui->p_tableView_SendedData_ASCII->hideColumn(1);

    // Init tableView HEX Received
    ui->p_tableView_ReceivedData_hex->setModel(&_hex_model_received);
    ui->p_tableView_ReceivedData_hex->resizeColumnsToContents();
    ui->p_tableView_ReceivedData_hex->resizeRowsToContents();
    ui->p_tableView_ReceivedData_hex->hideColumn(1);

    // Init tableView HEX Sended
    ui->p_tableView_SendedData_hex->setModel(&_hex_model_sended);
    ui->p_tableView_SendedData_hex->resizeColumnsToContents();
    ui->p_tableView_SendedData_hex->resizeRowsToContents();
    ui->p_tableView_SendedData_hex->hideColumn(1);

    // Init input View
    ui->p_tableView_SendTable->setModel(&_input_model);
    ui->p_tableView_SendTable->resizeColumnsToContents();
    ui->p_tableView_SendTable->resizeRowsToContents();

    // Resizing tableView to context
    QTableView* viewArray[] = {
        ui->p_tableView_History_ASCII,
        ui->p_tableView_History_hex,
        ui->p_tableView_SendedData_ASCII,
        ui->p_tableView_SendedData_hex,
        ui->p_tableView_ReceivedData_ASCII,
        ui->p_tableView_ReceivedData_hex,
    };

    for (unsigned int i = 0; i < (sizeof(viewArray) / sizeof(viewArray[0])); i++) {
        connect(&_source_model, &TableModel_UART::rowsInserted,
                viewArray[i], &QTableView::resizeRowsToContents);

        connect(&_source_model, &TableModel_UART::rowsInserted,
                viewArray[i], &QTableView::resizeColumnsToContents);

        connect(&_source_model, &TableModel_UART::layoutChanged,
                viewArray[i], &QTableView::resizeRowsToContents);

        connect(&_source_model, &TableModel_UART::layoutChanged,
                viewArray[i], &QTableView::resizeColumnsToContents);
    }
}

void MainWindow::init_Models() {
    // Filter source models
    // Received
    _model_received.setSourceModel(&_source_model);
    _model_received.setFilterKeyColumn(1);
    _model_received.setFilterFixedString(MessageDirect_str[UART_Message_t::Received]);

    // Sended
    _model_sended.setSourceModel(&_source_model);
    _model_sended.setFilterKeyColumn(1);
    _model_sended.setFilterFixedString(MessageDirect_str[UART_Message_t::Sended]);

    // HEX models
    // History HEX model
    _hex_history_model.setSourceModel(&_source_model);

    // Init proxy HEX model Received
    _hex_model_received.setSourceModel(&_model_received);

    // Init proxy HEX model Sended
    _hex_model_sended.setSourceModel(&_model_sended);
}

void MainWindow::init_SerialPort(const My_SerialPortSettings_t& serialSettings) {
    _serial.setBaudRate(serialSettings.BaudRate, serialSettings.Direction);
    _serial.setDataBits((QSerialPort::DataBits)serialSettings.DataBits);
    _serial.setStopBits(serialSettings.StopBits);
    _serial.setFlowControl(serialSettings.FlowControl);
    _serial.setParity(serialSettings.Parity);

    QIODevice::OpenModeFlag openMode;
    switch (serialSettings.Direction) {
    case QSerialPort::Direction::Input:
        openMode = QIODevice::OpenModeFlag::ReadOnly;
        break;

    case QSerialPort::Direction::Output:
        openMode = QIODevice::OpenModeFlag::WriteOnly;
        break;

    case QSerialPort::Direction::AllDirections:
        openMode = QIODevice::OpenModeFlag::ReadWrite;
        break;

    default:
        break;
    }

    // Connect serial port
    if (_serial.open(openMode)) {
        createInfoMessage("The connection is successful.");

        _serial.setDataTerminalReady(serialSettings.Pinout & QSerialPort::DataTerminalReadySignal);
        _serial.setRequestToSend(serialSettings.Pinout & QSerialPort::RequestToSendSignal);

        connect(&_serial, &QSerialPort::readyRead, [&]()
        {
            UART_Message_t current_message;
            current_message.date = QDateTime::currentDateTime();
            current_message.direct = UART_Message_t::Received;
            current_message.message = _serial.readAll();

            receiveMessage(current_message);
        });

        connect(ui->p_pushButton_Send, &QPushButton::clicked, [&]() {
            QByteArray arr;
            auto vec = _input_model.getSendData();
            for (const auto &value : vec) {
                arr.append(value);
            }

            UART_Message_t msg = {
                QDateTime::currentDateTime(),
                UART_Message_t::Sended,
                arr
            };

            sendMessage(msg);
        });
    } else {
        createInfoMessage("Connection failed.");
    }
}

MainWindow::~MainWindow()
{
    if (_serial.isOpen()) {
        _serial.close();
    }

    delete ui;
}

void MainWindow::createInfoMessage(const QString& info) {

    UART_Message_t msg;
    msg.date = QDateTime::currentDateTime();
    msg.direct = UART_Message_t::InfoMessage;
    msg.message = QByteArray::fromStdString(info.toStdString());

    _source_model.appendMessage(msg);

    ui->p_tableView_History_ASCII->resizeColumnsToContents();
    ui->p_tableView_History_hex->resizeColumnsToContents();

    ui->p_tableView_History_ASCII->resizeRowsToContents();
    ui->p_tableView_History_hex->resizeRowsToContents();
}

void MainWindow::receiveMessage(const UART_Message_t& msg) {
    _source_model.appendMessage(msg);
}

void MainWindow::sendMessage(const UART_Message_t& msg) {
    qint64 bytesWritten = _serial.write(msg.message);

    if (bytesWritten == msg.message.size()) {
        // Successful
        _source_model.appendMessage(msg);
    } else {
        // Error_msg 1
        UART_Message_t error_msg;// = msg;
        error_msg.date = QDateTime::currentDateTime();
        error_msg.direct = UART_Message_t::InfoMessage;

        QString error_str = QString("Message didn't send successful. Sended %1 of %2 bytes.")
                .arg(bytesWritten)
                .arg(msg.message.size());
        error_msg.message = QByteArray::fromStdString(error_str.toStdString());

        _source_model.appendMessage(error_msg);

        // Error_msg 2
        UART_Message_t error_msg_2;
        error_msg_2.date = QDateTime::currentDateTime();
        error_msg_2.direct = UART_Message_t::InfoMessage;
        QString full_str = "The message: \"" + QString::fromStdString(msg.message.toStdString()) + "\"";
        error_msg_2.message = QByteArray::fromStdString(full_str.toStdString());

        _source_model.appendMessage(error_msg_2);
    }
}
