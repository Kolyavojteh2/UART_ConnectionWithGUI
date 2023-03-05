#include "mainwindow.h"

#include "ConnectionSettingsDialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectionSettingsDialog setting;
    setting.show();

    MainWindow *my_mainWindow = nullptr;
    QObject::connect(&setting, &ConnectionSettingsDialog::settingsHaveBeenDefined,
                     [&](const My_SerialPortSettings_t setting) {
        my_mainWindow = new MainWindow(setting);
        my_mainWindow->show();
    });
    return a.exec();
}
