#ifndef MARKERMODEL_H
#define MARKERMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

class MarkerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    using QAbstractListModel::QAbstractListModel;
    enum MarkerRoles{
                        positionRole = Qt::UserRole + 1,
                        textRole
                    };

    Q_INVOKABLE
    void addMarker(const QGeoCoordinate &coordinate, QStringList listData) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_coordinates.append(coordinate);
        listName.append(listData);
        endInsertRows();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent)        
        return m_coordinates.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{
        if (index.row() < 0 || index.row() >= m_coordinates.count())
            return QVariant();
        if(role== MarkerModel::positionRole)
            return QVariant::fromValue(m_coordinates[index.row()]);

        if (index.row() < 0 || index.row() >= listName.count())
            return QVariant();
        if(role== MarkerModel::textRole)
            return QVariant::fromValue(listName[index.row()]);

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const{
        return {
            { positionRole, "position" },
            { textRole, "textInfo"}
        };
    }

private:
    QList<QGeoCoordinate> m_coordinates;
    // name, idCoordinates, MeteoStation/GidroPost
    QList<QStringList> listName;

};

#endif // MARKERMODEL_H
