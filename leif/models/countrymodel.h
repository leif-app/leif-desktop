#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <QObject>
#include <QAbstractListModel>

class CountryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CountryModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // COUNTRYMODEL_H
