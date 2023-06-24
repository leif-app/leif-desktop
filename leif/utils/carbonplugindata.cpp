/**
 * @brief Implements the CarbonPluginData class.
 *
 * @author Dariusz Scharsig
 * @date 26.09.2022
 *
 * @copyright Tim Stone 2022
 */
#include <QJsonObject>
#include <QJsonArray>

#include "carbonplugindata.h"

/**
 * @brief Creates a CarbonPluginData object.
 *
 * A \p name, \p description and a list with \p territories must be provided. As
 * long as a \p name and a non-empty list of \p territories is provided the
 * obect will be considered valid and isValid() will return \c true.
 *
 * @param name The name of the plugin.
 * @param description The description of the plugin.
 * @param territories A list with country definitions for this plugin.
 */
Utils::CarbonPluginData::CarbonPluginData(const QString &name,
                                          const QString &description,
                                          const QList<Territory> &territories):
    _name {name}, _description {description}, _territories {territories}
{}

/**
 * @brief Checks if the plugin data is valid or not.
 *
 * For the plugin data to be valid a name must be present and at least one
 * territory definition.
 *
 * @sa name()
 * @sa territoryList()
 *
 * @return \arg \c true  The plugin data is valid.
 *         \arg \c false The plugin data is invalid. Disregard it.
 */
bool Utils::CarbonPluginData::isValid()
{
    return(!name().isEmpty() && !territoryList().isEmpty());
}

/**
 * @brief Returns the name of the plugin.
 *
 * The name can be set in the json file.
 *
 * @return The name as a QString.
 */
QString Utils::CarbonPluginData::name() const
{
    return _name;
}

/**
 * @brief Returns the description of the plugin.
 *
 * The description can be set in the json file.
 *
 * @return The description as a QString.
 */
QString Utils::CarbonPluginData::description() const
{
    return _description;
}

/**
 * @brief Returns the list of stored territories.
 *
 * This method returns the raw data stored in the json file. You can iterate it
 * yourself or you can call one of the helper methods defined in this class.
 *
 * @return A QList with all the Utils::Territory definitions.
 */
QList<Utils::Territory> Utils::CarbonPluginData::territoryList() const
{
    return _territories;
}

/**
 * @brief Returns a list of all the countries this plugin can handle.
 *
 * This method returns a list of all the country/territory codes this plugin can
 * handle according to it's json file.
 *
 * @remark If you want the names of the countries, you can get them yourself from
 * QLocale or you can call the territoryNames() method.
 *
 * @sa territoryNames()
 *
 * @return A QList with QLocale::Country codes representing the countries.
 */
QList<QLocale::Country> Utils::CarbonPluginData::territories() const
{
    QList<QLocale::Country> territories;

    std::transform(std::begin(territoryList()), std::end(territoryList()), std::back_inserter(territories),
                   [](const Territory &territory) {
                       return territory.territory();});

    return territories;
}

/**
 * @brief Returns the names of the countries this plugin can handle.
 *
 * This convenience method will return a list of all the countries that are
 * defined in the json file.
 *
 * @return The list of country names as a QStringList.
 */
QStringList Utils::CarbonPluginData::territoryNames() const
{
    QStringList territoryNames;

    std::transform(std::begin(territoryList()), std::end(territoryList()), std::back_inserter(territoryNames),
                   [](const Territory &territory) {
                       return QLocale::countryToString(territory.territory());
    });

    return territoryNames;
}

/**
 * @brief Returns a list with regions for a given \p territory.
 *
 * This method will return all regions for a given country. If the plugin data
 * doesn't contain this \p territory, or the \p territory doesn't define any
 * regions, an empty list will be returned.
 *
 * @sa Utils::TranslatedStrings
 *
 * @param territory The country we want the region IDs for.
 * @return A list with the regions as a QList of Utils::TranslatedString objects.
 */
QList<Utils::TranslatedString> Utils::CarbonPluginData::regionList(const QLocale::Country country) const
{
    QList<TranslatedString> regions;

    auto territoryResult {std::find_if(std::begin(territoryList()), std::end(territoryList()), [&](const Territory &territory) {
        return territory.isValid() && territory.territory() == country;
    })};

    if(territoryResult != std::end(territoryList()))
        regions = territoryResult->regions();

    return regions;
}

/**
 * @brief Returns a list with region IDs for a given \p territory.
 *
 * This convenience method will return all region IDs for a given country. If the
 * plugin data doesn't contain this \p territory, or the \p territory doesn't
 * define any regions, an empty QStringList will be returned.
 *
 * @param territory The country we want the region IDs for.
 * @return A list with region IDs as a QStringList.
 */
QStringList Utils::CarbonPluginData::regionIds(const QLocale::Country territory) const
{
    const auto &regions = regionList(territory);

    QStringList regionIdList;

    std::transform(std::begin(regions), std::end(regions), std::back_inserter(regionIdList), [](const TranslatedString &region) {
        return region.id();
    });

    return regionIdList;
}

/**
 * @brief Returns a translated string for a particular country and region ID.
 *
 * If you have a country and a region ID, you can get the translated version of
 * the region name, should this translation be available within the plugin json
 * file.
 *
 * @remark The translation will be delivered for the current locale as returned
 * by QLocale().
 *
 * If no translation can be found for the provided \p country and \p regionId,
 * an empty QString will be returned.
 *
 * @param territory The country/territory code we want the translation for.
 * @param regionId The region ID we want the translated version of.
 * @return The translated version for the current locale as a QString.
 */
QString Utils::CarbonPluginData::translatedRegion(const QLocale::Country territory, const QString &regionId) const
{
    const auto &regions = regionList(territory);

    auto result = std::find_if(std::begin(regions), std::end(regions), [&](const TranslatedString &region) {
        return region.id() == regionId;
    });

    QString translated;
    if(result != std::end(regions))
        translated = result->translatedId().isEmpty() ? result->id() : result->translatedId();

    return translated;
}

/**
 * @brief Creates a CarbonPluginData object from the JSON MetaData object.
 *
 * The meta data needs to be read and supplied to this static method. If it is
 * more or less OK it will be parsed and a CarbonPluginData object will be
 * constructed and returned.
 *
 * If something was wrong an invalid CarbonPluginData object will be returned.
 * You can check by calling isValid().
 *
 * @sa isValid()
 *
 * @param json The json value containing the MetaData object.
 *
 * @return A CarbonPluginData object representing the plugin meta data.
 */
Utils::CarbonPluginData Utils::CarbonPluginData::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return CarbonPluginData();

    auto jsonObject {json.toObject()};

    auto name {jsonObject.value(QStringLiteral("name")).toString()};
    auto description {jsonObject.value(QStringLiteral("description")).toString()};
    auto territories {Territory::fromJsonArray(jsonObject.value(QStringLiteral("territories")))};

    return CarbonPluginData {name, description, territories};
}
