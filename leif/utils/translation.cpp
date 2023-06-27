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
 * @class Utils::Translation
 *
 * @brief Describes a translated string.
 *
 * The Translation class is a helper class that defines a translated string,
 * that is a string that is paired with a QLocale object, that defines a
 * language and a country selection (we need both, as some languages are spoken
 * in several countries, with subtle differences, and some countries speak
 * several languages).
 */

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

/**
 * @brief Returns the set locale object.
 *
 * The locale object determines, for which language, this string is supposed to
 * be used.
 *
 * You can set the locale either via the constructor or by calling setLocale().
 *
 * @sa setLocale()
 *
 * @return The current locale as a QLocale object.
 */
QLocale Utils::Translation::locale() const
{
    return _locale;
}

/**
 * @brief Sets the current locale to \p locale.
 *
 * To get the currently set locale, call locale().
 *
 * @sa locale()
 *
 * @param locale The new locale to be used by this translation string.
 */
void Utils::Translation::setLocale(const QLocale &locale)
{
    _locale = locale;
}

/**
 * @brief Returns the \c Translation given it's JSON representation.
 *
 * The JSON must be an object with following format:
 * \code
 * {
 *   locale: "en_GB",
 *   string: "translated string"
 * }
 * \endcode
 *
 * \remark
 * If the \p json is \c null or not a valid JSON object, the method will return
 * an empty Translation object.
 *
 * @param json The JSON value (object) to be used to construct a Translation.
 *
 * @return The Translation object.
 */
Utils::Translation Utils::Translation::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return Translation {};

    auto locale {json.toObject().value(QStringLiteral("locale")).toString()};
    auto string {json.toObject().value(QStringLiteral("string")).toString()};

    return Translation {string, locale};
}

/**
 * @brief Returns a list of Translation objects given it's JSON representation.
 *
 * To construct the list of Transaction objects, we need to provide a \p json
 * value, which should be a JSON array with the following format:
 * \code
 * [
 *   {
 *     "string": "translated string",
 *     "locale": "en_GB"
 *   },
 *   ...
 * ]
 * \endcode
 *
 * \remark
 * If the \p json object is \c null or not an array, then this method will
 * return an empty list.
 *
 * \remark
 * Only non-empty Translation objects will be stored in the list.
 *
 * @param json The JSON array containing the Translation definitions.
 *
 * @return The list of Translation objects as a QList<Utils::Translation> object.
 */
QList<Utils::Translation> Utils::Translation::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
        return QList<Translation> {};

    constexpr auto makeTr {[](const auto &json) { return Translation::fromJson(json);}};
    constexpr auto translationNotEmpty {[](const auto &translation) { return !translation.isEmpty();}};

    const auto &jsonArray {json.toArray()};
    QList<Translation> translations {};

    std::transform(std::begin(jsonArray),
                   std::end(jsonArray),
                   std::back_inserter(translations),
                   makeTr);

    QList<Translation> validTranslations {};
    std::copy_if(std::begin(translations),
                 std::end(translations),
                 std::back_inserter(validTranslations),
                 translationNotEmpty);

    return validTranslations;
}
