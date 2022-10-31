#include "regionmodel.h"
#include "plugin/carbonpluginmanager.h"

RegionModel::RegionModel(QObject *parent)
    : QAbstractListModel{parent},
      m_country{QLocale::AnyCountry}
{}

int RegionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    CarbonPluginManager *manager = CarbonPluginManager::Instance();
    if(manager == nullptr)
    {
        return 0;
    }

    return manager->regionIds(country()).count();
}

QVariant RegionModel::data(const QModelIndex &index, int role) const
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
    const QString regionId = manager->regionIds(country()).at(row);

    if(role == Qt::DisplayRole)
    {
        return manager->translatedRegion(country(), regionId);
    }

    return regionId;
}

QHash<int, QByteArray> RegionModel::roleNames() const
{
    static QHash<int, QByteArray> _names;
    if(_names.isEmpty())
    {
        _names.insert(Qt::DisplayRole, "name");
        _names.insert(Qt::UserRole, "id");
    }

    return _names;
}

const QLocale::Country &RegionModel::country() const
{
    return m_country;
}

void RegionModel::setCountry(const QLocale::Country &newCountry)
{
    if (m_country == newCountry)
        return;

    beginResetModel();
    m_country = newCountry;
    emit countryChanged();
    endResetModel();
}
