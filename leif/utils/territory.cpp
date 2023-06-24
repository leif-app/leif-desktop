/**
 * @brief Implements the Territory class.
 *
 * @sa Territory
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#include <QJsonArray>
#include <QJsonObject>

#include "territory.h"

Utils::Territory::Territory():
    Utils::Territory(QLocale::AnyCountry, QString {}, QList<TranslatedString> {})
{}

Utils::Territory::Territory(const QLocale::Country country, const QString &description, const QList<TranslatedString> &regions):
    _country {country}, _description {description}, _regions {regions}
{}

bool Utils::Territory::isValid() const
{
    return country() != QLocale::AnyCountry;
}

QLocale::Country Utils::Territory::country() const
{
    return _country;
}

QString Utils::Territory::description() const
{
    return _description;
}

bool Utils::Territory::hasRegions() const
{
    return _regions.count() > 0;
}

QList<Utils::TranslatedString> Utils::Territory::regions() const
{
    return _regions;
}

Utils::Territory Utils::Territory::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return Territory {};

    auto territoryObject {json.toObject()};

    if(!territoryObject.contains(QStringLiteral("territory")))
        return Territory {};

    auto country {static_cast<QLocale::Country>(territoryObject.value(QStringLiteral("territory")).toInt())};
    auto description {territoryObject.value(QStringLiteral("description")).toString()};
    auto regions {TranslatedString::fromJsonArray(territoryObject.value(QStringLiteral("regions")))};

    return Territory {country, description, regions};
}

QList<Utils::Territory> Utils::Territory::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
        return QList<Territory> {};

    QList<Territory> territories;
    const auto &jsonArray = json.toArray();

    auto addTerritory {[&](const auto &json) {
        auto territory = Territory::fromJson(json);
        if(territory.isValid())
            territories << territory;
    }};

    std::for_each(std::begin(jsonArray), std::end(jsonArray), addTerritory);

    return territories;
}
