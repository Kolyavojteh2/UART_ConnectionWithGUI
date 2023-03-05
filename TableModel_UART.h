#ifndef TABLEMODEL_UART_H
#define TABLEMODEL_UART_H

#include <QAbstractTableModel>

#include <QVector>
#include "UART_Message.h"

class TableModel_UART : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel_UART(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

//public slots:
    // Append data
    void appendMessage(const UART_Message_t& msg);
private:
    QVector<UART_Message_t> _data;
};

#endif // TABLEMODEL_UART_H
