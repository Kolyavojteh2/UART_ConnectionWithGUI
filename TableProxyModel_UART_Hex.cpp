#include "TableProxyModel_UART_Hex.h"

TableProxyModel_UART_Hex::TableProxyModel_UART_Hex(QObject* parent)
    : QAbstractProxyModel(parent)
{
    connect(this, &TableProxyModel_UART_Hex::sourceModelChanged,
            this, &TableProxyModel_UART_Hex::slot_connected_source);
}

void TableProxyModel_UART_Hex::updateColumnsCount() {
    int max_bytes = 1;
    for (int i = 0; i < sourceModel()->rowCount(); i++) {
        QModelIndex dataIndex = sourceModel()->index(i, 2);
        QByteArray bytes = sourceModel()->data(dataIndex).toByteArray();

        if (bytes.size() > max_bytes) {
            max_bytes = bytes.size();
        }
    }

    setNumColumns(max_bytes + ColumnsCount_Default - 1);
}

void TableProxyModel_UART_Hex::setNumColumns(int numColumns)
{
    m_numColumns = numColumns;
    emit layoutChanged();
}

QModelIndex TableProxyModel_UART_Hex::mapFromSource(const QModelIndex& sourceIndex) const
{
    if (sourceIndex.column() < ColumnsCount_WithoutData) {
        return createIndex(sourceIndex.row(), sourceIndex.column());
    } else {
        return createIndex(sourceIndex.row(), (sourceIndex.column() - ColumnsCount_WithoutData) * m_numColumns);
    }
}

QModelIndex TableProxyModel_UART_Hex::mapToSource(const QModelIndex& proxyIndex) const
{
    if (proxyIndex.column() <= (ColumnsCount_Default - 1)) {
        return sourceModel()->index(proxyIndex.row(), proxyIndex.column());
    } else {
        return sourceModel()->index(proxyIndex.row(), 2);
    }
}

int TableProxyModel_UART_Hex::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return sourceModel()->rowCount();
}

int TableProxyModel_UART_Hex::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_numColumns;
}

QVariant TableProxyModel_UART_Hex::data(const QModelIndex& proxyIndex, int role) const
{
    if (!proxyIndex.isValid())
        return QVariant();

    int columnOffset = ColumnsCount_Default - 1;
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

        if (proxyIndex.column() == 0 || proxyIndex.column() == 1) {
            QModelIndex sourceIndex = mapToSource(proxyIndex);
            return sourceModel()->data(sourceIndex);
        }

        const int col = proxyIndex.column();

        QModelIndex sourceIndex = mapToSource(proxyIndex);
        if (!sourceIndex.isValid())
            return QVariant();

        QByteArray data = sourceIndex.data().toByteArray();
        if (data.isEmpty())
            return QVariant();

        const int index = (col - columnOffset) % m_numColumns;
        if (index >= data.size())
            return QVariant();

        QString dataInTheField = QString("%1").arg(data.at(index), dataHexWidth, 16, QLatin1Char('0'));
        dataInTheField = dataInTheField.toUpper();
        dataInTheField = "0x" + dataInTheField;

        return QVariant(dataInTheField);
    }

    return QVariant();
}

QVariant TableProxyModel_UART_Hex::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (orientation == Qt::Orientation::Vertical) {
            return section;
        }

        if (orientation == Qt::Orientation::Horizontal) {
            if (section >= ColumnsCount_WithoutData) {
                const int index = section - ColumnsCount_WithoutData;

                QString HeaderName = QString("%1").arg(index, headerHexWidth, 16, QLatin1Char('0'));
                HeaderName = HeaderName.toUpper();
                HeaderName = "0x" + HeaderName;

                return QVariant(HeaderName);
            }

        }
    }

    return QAbstractProxyModel::headerData(section, orientation, role);
}

QModelIndex TableProxyModel_UART_Hex::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // визначаємо відповідний стовпець у моделі джерелі даних
    int sourceColumn;
    if (/*column - (ColumnsCount_Default - 1) <= 0*/ column == 0 || column == 1) {
        sourceColumn = column;
    } else {
        sourceColumn = ColumnsCount_Default - 1;
    }

    // визначаємо позицію стовпця для даних, які містяться в моделі джерелі даних
    const int sourceColumnOffset = (column - (ColumnsCount_Default - 1)) % m_numColumns;

    // отримуємо індекс у джерелі даних
    const QModelIndex sourceIndex = sourceModel()->index(row, sourceColumn, parent);

    // використовуємо createIndex() для створення індексу у проксі-моделі
    return createIndex(row, column, sourceIndex.row() * (m_numColumns - (ColumnsCount_Default - 1)) + sourceColumnOffset);
}

QModelIndex TableProxyModel_UART_Hex::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    return QModelIndex();

    /*
    // визначаємо індекс батьківського елемента у джерелі даних
    const int sourceRow = index.internalId() / m_numColumns;

    // викликаємо parent() для отримання батьківського індексу у джерелі даних
    const QModelIndex sourceParent = sourceModel()->index(sourceRow, index.column() / m_numColumns);

    // використовуємо createIndex() для створення індексу батьківського елемента у проксі-моделі
    return createIndex(sourceParent.row(), index.column(), sourceRow * m_numColumns);
    */
}

void TableProxyModel_UART_Hex::slot_connected_source() {
    auto source_model = sourceModel();
    connect(source_model, &QAbstractTableModel::rowsInserted,
            this, &TableProxyModel_UART_Hex::slot_updateCountColumns);

    updateColumnsCount();
}

void TableProxyModel_UART_Hex::slot_updateCountColumns(const QModelIndex&, int, int) {
    updateColumnsCount();
}
