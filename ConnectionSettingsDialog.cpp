#include "ConnectionSettingsDialog.h"
#include "ui_ConnectionSettingsDialog.h"
#include <QMessageBox>
#include <QButtonGroup>

ConnectionSettingsDialog::ConnectionSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionSettingsDialog)
{
    ui->setupUi(this);

    ui->p_comboBox_DataBits->setCurrentIndex(3);
    ui->p_comboBox_Direction->setCurrentIndex(2);

    connect(this, &ConnectionSettingsDialog::accepted,
            this, &ConnectionSettingsDialog::prepareSettings);
}

ConnectionSettingsDialog::~ConnectionSettingsDialog()
{
    delete ui;
}

void ConnectionSettingsDialog::disableDevice() {
    ui->p_lineEdit_Device->setEnabled(false);
    ui->p_lineEdit_Device->clear();
}

void ConnectionSettingsDialog::prepareSettings() {
    My_SerialPortSettings_t result;

    QVector<QString> error_Messages;

    // BaudRate
    if (ui->p_radioButton_Custom->isChecked()) {
        if (ui->p_lineEdit_CustomBaudRate->text().isEmpty()) {
            error_Messages.append("The field baud rate is empty. Please enter the baud rate value.\n");
        }
        result.BaudRate = ui->p_lineEdit_CustomBaudRate->text().toInt();
    } else {
        auto radioButtons = ui->p_groupBox_BaudRate->children();

        for (auto &obj : radioButtons) {
            QRadioButton* radioBox = dynamic_cast<QRadioButton*>(obj);
            if (radioBox == nullptr) {
                continue;
            }

            if (radioBox->isChecked()) {
                QString str = radioBox->text();
                str.remove(" ");

                result.BaudRate = str.toInt();
                break;
            }
        }

        if (result.BaudRate == 0) {
            error_Messages.append("The baud rate didn't choiced. Please choice or enter the baud rate value.\n");
        }
    }

    // DataBits
    result.DataBits = ui->p_comboBox_DataBits->currentText().toInt();

    // Direction
    QString currentDirection_str = ui->p_comboBox_Direction->currentText();
    if (currentDirection_str == "Input") {
        result.Direction = QSerialPort::Direction::Input;
    } else if (currentDirection_str == "Output") {
        result.Direction = QSerialPort::Direction::Output;
    } else if (currentDirection_str == "AllDirections") {
        result.Direction = QSerialPort::Direction::AllDirections;
    } else {
        error_Messages.append("Something is wrong. The Direction ComboBox.\n");
    }

    // FlowControl
    QString currentFlowControl_str = ui->p_comboBox_FlowControl->currentText();
    if (currentFlowControl_str == "NoFlowControl") {
        result.FlowControl = QSerialPort::FlowControl::NoFlowControl;
    } else if (currentFlowControl_str == "HardwareControl") {
        result.FlowControl = QSerialPort::FlowControl::HardwareControl;
    } else if (currentFlowControl_str == "SoftwareControl") {
        result.FlowControl = QSerialPort::FlowControl::SoftwareControl;
    } else {
        error_Messages.append("Something is wrong. The FlowControl ComboBox.\n");
    }

    // Parity
    QString currentParity_str = ui->p_comboBox_Parity->currentText();
    if (currentParity_str == "NoParity") {
        result.Parity = QSerialPort::Parity::NoParity;
    } else if (currentParity_str == "EvenParity") {
        result.Parity = QSerialPort::Parity::EvenParity;
    } else if (currentParity_str == "OddParity") {
        result.Parity = QSerialPort::Parity::OddParity;
    } else if (currentParity_str == "SpaceParity") {
        result.Parity = QSerialPort::Parity::SpaceParity;
    } else if (currentParity_str == "MarkParity") {
        result.Parity = QSerialPort::Parity::MarkParity;
    } else {
        error_Messages.append("Something is wrong. The Parity ComboBox.\n");
    }

    // Pinout
    result.Pinout = QSerialPort::PinoutSignal::NoSignal;
    if (ui->p_checkBox_NoSignal->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::NoSignal;
    }
    if (ui->p_checkBox_DTR->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::DataTerminalReadySignal;
    }
    if (ui->p_checkBox_DCD->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::DataCarrierDetectSignal;
    }
    if (ui->p_checkBox_DSR->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::DataSetReadySignal;
    }
    if (ui->p_checkBox_RNG->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::RingIndicatorSignal;
    }
    if (ui->p_checkBox_RTS->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::RequestToSendSignal;
    }
    if (ui->p_checkBox_CTS->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::ClearToSendSignal;
    }
    if (ui->p_checkBox_STD->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::SecondaryTransmittedDataSignal;
    }
    if (ui->p_checkBox_SRD->isChecked()) {
        result.Pinout |= QSerialPort::PinoutSignal::SecondaryReceivedDataSignal;
    }

    // StopBits
    QString currentStopBits = ui->p_comboBox_StopBits->currentText();
    if (currentStopBits == "OneStop") {
        result.StopBits = QSerialPort::StopBits::OneStop;
    } else if (currentStopBits == "OneAndHalfStop") {
        result.StopBits = QSerialPort::StopBits::OneAndHalfStop;
    } else if (currentStopBits == "TwoStop") {
        result.StopBits = QSerialPort::StopBits::TwoStop;
    } else {
        error_Messages.append("Something is wrong. The StopBits ComboBox.\n");
    }

    // Device
    QString currentDevice_str = ui->p_lineEdit_Device->text();
    if (currentDevice_str.isEmpty() && ui->p_lineEdit_Device->isEnabled()) {
        error_Messages.append("The device field is empty. Please enter the device name.\n");
    }

    // Check is exists Fix it
    // Fix it
    result.Device = currentDevice_str;

    // Error handle
    if (!error_Messages.isEmpty()) {
        QString error_str;
        for (auto str : error_Messages) {
            error_str.append(str);
        }
        QMessageBox::critical(this, "Settings Error!", error_str);
        return;
    }

    // emit signal to send settings
    emit settingsHaveBeenDefined(result);
}
