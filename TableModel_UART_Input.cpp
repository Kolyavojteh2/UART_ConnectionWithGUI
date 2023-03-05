#include "TableModel_UART_Input.h"
#include <string>

TableModel_UART_Input::TableModel_UART_Input(QObject *parent)
    : QAbstractTableModel(parent)
{
    my_data.resize(n_rows);

    for (int i = 0; i < my_data.size(); i++) {
        my_data[i].resize(n_columns);
    }

    for (int i = 0; i < my_data.size(); i++) {
        for (int j = 0; j < my_data[i].size(); j++) {
            my_data[i][j] = -1;
        }
    }

    connect(this, &TableModel_UART_Input::dataChanged,
            this, &TableModel_UART_Input::prepareDataAndEmitTextChanged);
}

QVariant TableModel_UART_Input::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    QVariant result;
    if (orientation == Qt::Orientation::Horizontal) {
        QString str = "0x" + QString("%1").arg(section, 2, 16, QLatin1Char('0'));

        result = str;
    }

    if (orientation == Qt::Orientation::Vertical) {
        QString str = "0x" + QString("%1").arg(section * 16, 2, 16, QLatin1Char('0'));

        result = str;
    }

    return result;
}

int TableModel_UART_Input::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 16;
}

int TableModel_UART_Input::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 16;
}

QVariant TableModel_UART_Input::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (my_data[index.row()][index.column()] == -1) {
            return "";
        }

        if (my_data[index.row()][index.column()] > 0xFF) {
            return "";
        }

        QString str = "0x" + QString("%1").arg(my_data[index.row()][index.column()], 2, 16, QLatin1Char('0'));
        return str;
    }

    return QVariant();
}

bool TableModel_UART_Input::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {

        if (value.toString().isEmpty()) {
            my_data[index.row()][index.column()] = -1;
            emit dataChanged(index, index, {role});
            return true;
        }

        my_data[index.row()][index.column()] = analizeStringAndGetValue(value.toString());
        if (my_data[index.row()][index.column()] != -1) {
            my_data[index.row()][index.column()] &= 0xFF;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags TableModel_UART_Input::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

uint8_t TableModel_UART_Input::analizeStringAndGetValue(QString str) const {
    if (str.isEmpty()) {
        return -1;
    }

    // Decimal value
    bool is_ok = false;
    int temp = str.toInt(&is_ok);
    if (is_ok) {
        return temp;
    }

    // Check Hex
    std::string std_str = str.toStdString();
    int pos = std_str.find("0x");
    if (pos == -1) {
        pos = std_str.find("0X");
    }

    if (pos != -1) {
        // Hex value
        std_str.erase(pos, 2);
        str = QString::fromStdString(std_str);

        temp = str.toInt(&is_ok, 16);
        if (is_ok) {
            return temp;
        }
    }

    return -1;
}

QVector<uint8_t> TableModel_UART_Input::getSendData() const {
    QVector<uint8_t> result;

    for (int i = 0; i < my_data.size(); i++) {
        for (int j = 0; j < my_data[i].size(); j++) {
            if (my_data[i][j] == -1) {
                goto EXIT_FUNCTION;
            }

            result.append(my_data[i][j]);
        }
    }

    EXIT_FUNCTION:
    return result;
}

void TableModel_UART_Input::setSendData(const QString& str) {
    unsigned int n = 0;

    std::string std_str = str.toStdString();
    for (int i = 0; i < my_data.size(); i++) {
        for (int j = 0; j < my_data[i].size(); j++) {
            if (n >= std_str.size()) {
                my_data[i][j] = -1;
            } else {
                my_data[i][j] = std_str[n];
                n++;
            }
        }
    }

    emit layoutChanged();
}

void TableModel_UART_Input::prepareDataAndEmitTextChanged() {
    auto vec = getSendData();
    QString result;
    for (auto ch : vec) {
        result.append(QChar(ch));
    }

    emit TextChanged(result);
}
