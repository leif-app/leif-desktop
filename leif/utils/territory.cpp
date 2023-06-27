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

/**
 * @brief Constructs an empty and invalid Territory object.
 *
 * This object has no description, no regions, and above all, no set
 * territory code, which makes it invalid.
 *
 * @sa isInvalid()
 */
Utils::Territory::Territory():
    Utils::Territory(QLocale::AnyCountry, QString {}, QList<TranslatedString> {})
{}

/**
 * @brief This initialized Constructor creates a Territory object.
 *
 * It will be initialized witht he \p territory, \p description and a list of
 * \p translations.
 *
 * @param territory The initial territory code.
 * @param description The initial description.
 * @param regions The initial list of regions.
 */
Utils::Territory::Territory(const QLocale::Territory territory, const QString &description, const QList<TranslatedString> &regions):
    _territory {territory}, _description {description}, _regions {regions}
{}

/**
 * @brief Returns if this territory object is valid or not.
 *
 * In order to be valid, a territory needs at least a territory code, which can
 * be retrieved with territory().
 *
 * @sa territory()
 *
 * @return \arg \c true  Yes, this object is valid.
 *         \arg \c false No, this Territory object is invalid.
 */
bool Utils::Territory::isValid() const
{
    return territory() != QLocale::AnyCountry;
}

/**
 * @brief Returns the territory code of this object.
 *
 * This code has to be set in the constructor. If it is QLocale::AnyTerritory,
 * then this object is considered invalid.
 *
 * @return The territory code as a QLocale::Territory value.
 */
QLocale::Territory Utils::Territory::territory() const
{
    return _territory;
}

/**
 * @brief Returns the optional description for this territory.
 *
 * The description is optional.
 *
 * @return The description as a QString.
 */
QString Utils::Territory::description() const
{
    return _description;
}

/**
 * @brief Returns if this territory has defined regions or not.
 *
 * It is possible that a territory can have no regions at all. This method will
 * return \c true if this Territory has defined regions. The regions can be
 * accessed with regions().
 *
 * @sa regions()
 *
 * @return \arg \c true  This object has regions.
 *         \arg \c false This object has no regions.
 */
bool Utils::Territory::hasRegions() const
{
    return regions().count() > 0;
}

/**
 * @brief Returns a list of regions as translated strings.
 *
 * Each region has an ID and possibly a translated version of it.
 *
 * @return A list of regions as a QList of TranslatedString objects.
 */
QList<Utils::TranslatedString> Utils::Territory::regions() const
{
    return _regions;
}

/**
 * @brief Returns a Territory object from a JSON file.
 *
 * The JSON has a following format:
 * \code
 * {
 *   "territory": 246,
 *   "description": "United Kingdom",
 *   "regions": [
 *     {
 *       "id": "North Scotland",
 *       "translations": [
 *         {
 *           "locale": "en_GB",
 *           "string": "North Scotland"
 *         },
 *         ...
 *       ]
 *     },
 *     ...
 *   ]
 * }
 * \endcode
 *
 * @remark
 * The \p json must be an object. If the JSON is not valid or not an object,
 * then an invalid (empty) Territory object will be returned.
 *
 * @param json The JSON object.
 *
 * @return The Territory object.
 */
Utils::Territory Utils::Territory::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return Territory {};

    auto territoryObject {json.toObject()};

    if(!territoryObject.contains(QStringLiteral("territory")))
        return Territory {};

    auto territory {static_cast<QLocale::Country>(territoryObject.value(QStringLiteral("territory")).toInt())};
    auto description {territoryObject.value(QStringLiteral("description")).toString()};
    auto regions {TranslatedString::fromJsonArray(territoryObject.value(QStringLiteral("regions")))};

    return Territory {territory, description, regions};
}

/**
 * @brief Returns a list of valid territories from a JSON array.
 *
 * This method needs a JSON \p json array. It expects them to be a list of
 * Territory objects as defined in fromJson().
 *
 * @sa fromJson()
 *
 * @param json The JSON array containing the Territory elements.
 * @return A list with territory objects as a QList<Territory>.
 */
QList<Utils::Territory> Utils::Territory::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
        return QList<Territory> {};

    constexpr auto makeTerritory {[](const auto &jsonValue) { return Territory::fromJson(jsonValue);}};
    constexpr auto territoryValid {[](const auto &territory) { return territory.isValid(); }};

    QList<Territory> territories {};
    const auto &jsonArray = json.toArray();

    std::transform(std::begin(jsonArray),
                   std::end(jsonArray),
                   std::back_inserter(territories),
                   makeTerritory);

    QList<Territory> validTerritories {};

    std::copy_if(std::begin(territories),
                 std::end(territories),
                 std::back_inserter(validTerritories),
                 territoryValid);

    return validTerritories;
}
