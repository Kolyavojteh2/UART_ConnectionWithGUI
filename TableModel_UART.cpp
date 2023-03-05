#include "TableModel_UART.h"

TableModel_UART::TableModel_UART(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant TableModel_UART::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QVariant result;
    if (orientation == Qt::Orientation::Vertical) {
        result = (section + 1);
    }

    if (orientation == Qt::Orientation::Horizontal) {
        switch (section) {
        case 0:
            result = "Date";
            break;

        case 1:
            result = "Direction";
            break;

        case 2:
            result = "Message";
            break;
        default:
            result = "IDK";
            break;
        }
    }

    return result;
}

int TableModel_UART::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _data.size();
}

int TableModel_UART::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return UART_Message_Constants::UART_CountVariables;
}

QVariant TableModel_UART::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QVariant result;
    switch (index.column()) {
    case 0:
        result = _data[index.row()].date.toString("dd.MM.yyyy  hh:mm:ss.zzz"/*Qt::DateFormat::ISODateWithMs*/);
        break;

    case 1:
        result = MessageDirect_str[_data[index.row()].direct];
        //result = _data[index.row()].direct;
        break;

    case 2:
        result = _data[index.row()].message;
        break;

    default:
        result = "IDK_2";
        break;
    }

    return result;
}

void TableModel_UART::appendMessage(const UART_Message_t& msg) {
    auto newRow = _data.size();
    beginInsertRows(QModelIndex(), newRow, newRow);
        _data.append(msg);
    endInsertRows();
}
