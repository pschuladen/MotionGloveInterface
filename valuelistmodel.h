#ifndef VALUELISTMODEL_H
#define VALUELISTMODEL_H

#include <QAbstractItemModel>
/*
 *Possible alternative implementation for valueviewbackend
 *NOT IMPLEMENTED YET!
 *
*/
struct DataComponent {
    DataComponent(const QString &name) : cName(name) {}
    float cVal;
    QString cName;
//    float maxVal;
//    float minVal;
};

class ValueListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles {
        NameRole = Qt::DisplayRole+1,
        ValueRole,
        MinValueRole,
        MaxValueRole
    };
    Q_ENUM(Roles)

    explicit ValueListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList <DataComponent> m_data;
};

#endif // VALUELISTMODEL_H
