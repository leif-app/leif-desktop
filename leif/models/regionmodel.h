#ifndef REGIONMODEL_H
#define REGIONMODEL_H

#include "QtCore/qlocale.h"
#include <QObject>
#include <QAbstractListModel>
#include <QQmlEngine>

class RegionModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QLocale::Country country READ country WRITE setCountry NOTIFY countryChanged)
    QML_ELEMENT
public:
    explicit RegionModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    const QLocale::Country &country() const;
    void setCountry(const QLocale::Country &newCountry);
signals:
    void countryChanged();

private:
    QLocale::Country m_country;
};

#endif // REGIONMODEL_H
