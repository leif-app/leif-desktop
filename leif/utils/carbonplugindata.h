/**
 * @brief Defines the CarbonPluginData class.
 *
 * The CarbonPluginData class offers information regarding a carbon plugin that
 * is stored in the plugin json file.
 *
 * @author Dariusz Scharsig
 * @date 26.09.2022
 *
 * @copyright Tim Stone 2022
 */
#ifndef CARBONPLUGINDATA_H
#define CARBONPLUGINDATA_H

#include <QJsonValue>
#include <QLocale>

#include "territory.h"
#include "translatedstring.h"

namespace Utils {
class CarbonPluginData
{
public:
    CarbonPluginData() = default;
    CarbonPluginData(const QString &name, const QString &description, const QList<Territory> &territories);
    ~CarbonPluginData() = default;

    bool isValid();

    QString name() const;
    QString description() const;

    QList<Territory> territoryList() const;
    QList<QLocale::Country> territories() const;
    QStringList territoryNames() const;
    QList<TranslatedString> regionList(const QLocale::Country territory) const;
    QStringList regionIds(const QLocale::Country territory) const;
    QString translatedRegion(const QLocale::Country territory, const QString &regionId) const;

    static CarbonPluginData fromJson(const QJsonValue &json);

private:
    QString _name;
    QString _description;
    QList<Territory> _territories;
};
}

#endif // CARBONPLUGINDATA_H
