#ifndef TABLEMODEL_UART_INPUT_H
#define TABLEMODEL_UART_INPUT_H

#include <QAbstractTableModel>
#include <QVector>

class TableModel_UART_Input : public QAbstractTableModel
{
    Q_OBJECT

private:
    const int n_rows = 16;
    const int n_columns = 16;

public:
    explicit TableModel_UART_Input(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVector<uint8_t> getSendData() const;

public slots:
    void setSendData(const QString& str);
    void prepareDataAndEmitTextChanged();
    //void validateModel();

signals:
    void TextChanged(const QString& str);
private:
    QVector<QVector<int>> my_data;

    uint8_t analizeStringAndGetValue(QString str) const;
};

#endif // TABLEMODEL_UART_INPUT_H
