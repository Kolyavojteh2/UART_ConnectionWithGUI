#ifndef CONNECTIONSETTINGSDIALOG_H
#define CONNECTIONSETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>

#include "My_SerialPortSettings_t.h"

namespace Ui {
class ConnectionSettingsDialog;
}

class ConnectionSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionSettingsDialog(QWidget *parent = nullptr);
    ~ConnectionSettingsDialog();

    void disableDevice();

private slots:
    void prepareSettings();

signals:
    void settingsHaveBeenDefined(const My_SerialPortSettings_t setting);

private:
    Ui::ConnectionSettingsDialog *ui;
};

#endif // CONNECTIONSETTINGSDIALOG_H
