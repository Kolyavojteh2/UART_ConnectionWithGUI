#ifndef TABLEPROXYMODEL_UART_HEX_H
#define TABLEPROXYMODEL_UART_HEX_H

#include <QAbstractProxyModel>
#include <QByteArray>

class TableProxyModel_UART_Hex : public QAbstractProxyModel
{
private:
    const int ColumnsCount_Default = 3;
    const int ColumnsCount_WithoutData = ColumnsCount_Default - 1;

public:
    TableProxyModel_UART_Hex(QObject* parent = nullptr);

    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& proxyIndex, int role = Qt::DisplayRole) const override;

    QModelIndex parent(const QModelIndex& index) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;

    void setNumColumns(int numColumns);

public slots:
    void slot_connected_source();
    void slot_updateCountColumns(const QModelIndex&, int, int);

private:
    int m_numColumns = 1;

    int headerHexWidth = 2;
    int dataHexWidth = 2;
private:
    void updateColumnsCount();
};

#endif // TABLEPROXYMODEL_UART_HEX_H
