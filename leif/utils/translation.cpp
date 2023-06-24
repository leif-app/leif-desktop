/**
 * @brief Implements the Translation class.
 *
 * @sa Translation
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#include <QJsonArray>
#include <QJsonObject>

#include "translation.h"

/**
 * @brief Creates an empty translation object.
 */
Utils::Translation::Translation():
    Translation {QString(), QLocale {QLocale::AnyLanguage, QLocale::AnyCountry}}
{}

/**
 * @brief Creates a translation for the string \p string and the \p localeName locale.
 *
 * The \p localeName needs to be in following format: $LANGUAGE_$COUNTRY. Where
 * \c $LANGUAGE is for example \b en or \b de and $COUNTRY is \b US or \b DE.
 *
 * @param string The string to be translated.
 * @param localeName The name of the locale for this translation.
 */
Utils::Translation::Translation(const QString &string, const QString &localeName):
    Translation {string, QLocale{localeName}}
{}

/**
 * @brief Creates a translation for the string \p string and the \p locale.
 *
 * @param string The string to be translated.
 * @param locale The locale for this translation.
 */
Utils::Translation::Translation(const QString &string, const QLocale &locale):
    _string {string}, _locale{locale}
{}

/**
 * @brief Return \c true when this translation object is empty.
 *
 * The translation object is considered empty, when the string itself is empty.
 *
 * @sa string()
 *
 * @return \arg \c true The translation object is empty.
 *         \arg \c false The translation object is not empty.
 */
bool Utils::Translation::isEmpty() const
{
    return _string.isEmpty();
}

/**
 * @brief Returns the stored string value.
 *
 * It can be set with setString() or by creating the object.
 *
 * @sa setString()
 *
 * @return The stored string value as a QString.
 */
QString Utils::Translation::string() const
{
    return _string;
}

/**
 * @brief Sets the translation string to \p srtring.
 *
 * You can read the string with string().
 *
 * @sa string()
 *
 * @param string The new translation string.
 */
void Utils::Translation::setString(const QString &string)
{
    _string = string;
}

QLocale Utils::Translation::locale() const
{
    return _locale;
}

void Utils::Translation::setLocale(const QLocale &locale)
{
    _locale = locale;
}

Utils::Translation Utils::Translation::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return Translation {};

    auto locale {json.toObject().value(QStringLiteral("locale")).toString()};
    auto string {json.toObject().value(QStringLiteral("string")).toString()};

    return Translation {string, locale};
}

QList<Utils::Translation> Utils::Translation::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
        return QList<Translation>();

    const auto &jsonArray {json.toArray()};
    QList<Translation> translations;

    auto addToList = [&](const auto &jsonValue) {
        auto translation {Translation::fromJson(jsonValue)};
        if(!translation.isEmpty())
            translations << translation;
    };

    std::for_each(std::begin(jsonArray), std::end(jsonArray), addToList);

    return translations;
}
