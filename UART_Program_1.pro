QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConnectionSettingsDialog.cpp \
    TableModel_UART.cpp \
    TableModel_UART_Input.cpp \
    TableProxyModel_UART_Hex.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ConnectionSettingsDialog.h \
    My_SerialPortSettings_t.h \
    TableModel_UART.h \
    TableModel_UART_Input.h \
    TableProxyModel_UART_Hex.h \
    UART_Message.h \
    mainwindow.h

FORMS += \
    ConnectionSettingsDialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
