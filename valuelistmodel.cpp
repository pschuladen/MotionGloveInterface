#include "valuelistmodel.h"

ValueListModel::ValueListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant ValueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex ValueListModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex ValueListModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int ValueListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int ValueListModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant ValueListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    const DataComponent &data = m_data.at(index.row());

    // FIXME: Implement me!
    return QVariant();
}
