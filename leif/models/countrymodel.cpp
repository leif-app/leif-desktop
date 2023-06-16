#include "countrymodel.h"
#include "plugin/carbonpluginmanager.h"

CountryModel::CountryModel(QObject *parent /* = nullptr */):
    QAbstractListModel(parent)
{}

int CountryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        return 0;
    }

    return manager->territories().count();
}

QVariant CountryModel::data(const QModelIndex &index, int role) const
{
    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        return QVariant();
    }

    if(role != Qt::DisplayRole && role != Qt::UserRole)
    {
        return QVariant();
    }

    int row = index.row();
    QLocale::Country country = manager->territories().at(row);
    if(role == Qt::DisplayRole)
    {
        qDebug() << "The country: " << country;
        qDebug() << "The country name: " << QLocale::countryToString(country);

        return QLocale::countryToString(country);
    }

    return country;
}

QHash<int, QByteArray> CountryModel::roleNames() const
{
    static QHash<int, QByteArray> _names;
    if(_names.isEmpty())
    {
        _names.insert(Qt::DisplayRole, "name");
        _names.insert(Qt::UserRole, "code");
    }

    return _names;
}
