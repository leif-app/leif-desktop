/**
 * @brief Implements the TranslatedString utility class.
 *
 * @sa TranslatedString
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */

#include <QJsonArray>
#include <QJsonObject>

#include "translatedstring.h"

/**
 * @class Utils::TranslatedString
 *
 * @brief Defines a translated string.
 *
 * A translated string is basically a string ID (id()) that is paired with a
 * translation or a translated ID (translatedID()). The translation process
 * happens during construction, where the ID and a list of translations is
 * provided. The correct one, as determined by the current systems locale,
 * will be used to set the translated ID.
 *
 * @sa id()
 * @sa translatedId()
 */

/**
 * @brief Constructs a translated string.
 *
 * You need to provide an \p id, which will be used if no matching translation
 * can be found among the \p translations.
 *
 * After construction, the translatedId() will contain the translated string, if
 * a matching translation could be found among the \p translations.
 *
 * \remark
 * A matching locale means that one of the translations contains a locale, that
 * fits the current system locale.
 *
 * @param id The "raw" string, that should be translated.
 * @param translations A list with translations to consider.
 */
Utils::TranslatedString::TranslatedString(const QString &id, const QList<Translation> &translations):
    _id {id}, _translatedId {TranslatedString::translate(translations)}
{}

/**
 * @brief Returns \c true if this translated string is empty.
 *
 * The translated string is considered empty, when the id is empty.
 *
 * @sa id()
 *
 * @return \arg \c true Yes, this translated string is empty.
 *         \arg \c false No, this translated string is not empty.
 */
bool Utils::TranslatedString::isEmpty() const
{
    return id().isEmpty();
}

/**
 * @brief Returns the ID of this translated string.
 *
 * The ID is the \c raw string, that should be translated. If the translation
 * was successfull, then translatedId() will contain the translated ID.
 *
 * @sa translatedId()
 *
 * @return The unstranslated ID of this string as a QString.
 */
QString Utils::TranslatedString::id() const
{
    return _id;
}

/**
 * @brief Returns the translated ID of this string.
 *
 * This method returns the translated version of the ID. If the object isn't
 * empty.
 *
 * @return The translated ID as a QString.
 */
QString Utils::TranslatedString::translatedId() const
{
    if(isEmpty())
        return QString();

    return _translatedId;
}

/**
 * @brief Returns the correct translation.
 *
 * This \c static helper method recieves a list with \p translations, and
 * attempts to find the one, which has the same locale as the system. If it
 * does find it, its string will be returned and can be used to set the
 * translated ID of this string.
 *
 * @param translations A list with translation objects.
 * @return The translation string as a QString.
 */
QString Utils::TranslatedString::translate(const QList<Translation> &translations)
{
    auto localeFits = [](const Utils::Translation &translation) -> bool {
        if(translation.isEmpty()) return false;

        auto trLocaleName {translation.locale().name()};
        auto localeName {QLocale().name()};

        return trLocaleName == localeName;
    };

    auto result = std::find_if(std::begin(translations), std::end(translations), localeFits);

    if(result != std::end(translations))
        return result->string();

    return QString {};
}

/**
 * @brief Returns a translated string from a JSON string.
 *
 * This static helper method will create a Utils::TranslatedString object from a
 * JSON value, which should be an object with the following format:
 * \code
 * {
 *   "id": "the untranslated id",
 *   "translations": [
 *     {
 *       "locale": "some locale",
 *       "string: "translated string"
 *     },
 *     ...
 *   ]
 * }
 * \endcode
 *
 * @param json The json object, containg the translated string.
 * @return The translated string as a Utils::TranslatedString object.
 */
Utils::TranslatedString Utils::TranslatedString::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return TranslatedString {};

    const auto &jsonObject {json.toObject()};

    auto id {jsonObject.value(QStringLiteral("id")).toString()};

    QList<Translation> translations {};

    if(jsonObject.contains(QStringLiteral("translations")))
        translations = Translation::fromJsonArray(jsonObject.value(QStringLiteral("translations")));

    return TranslatedString {id, translations};
}

/**
 * @brief Returns a list of translated strings from a json array value object.
 *
 * This static helper method will create a list of Utils::TranslatedString
 * objects from a JSON value, which should be an array of translated strings.
 * This is the format:
 * \code
 * [
 *   {
 *     "id": "the untranslated id",
 *     "translations": [
 *       {
 *         "locale": "some locale",
 *         "string": "translated string"
 *       },
 *       {
 *         "locale": "some other locale",
 *         "string": "translated string"
 *       }
 *     ]
 *   },
 *   {
 *     "id: "other untranslated id",
 *     "translations": [
 *       ...
 *     ]
 *   }
 * ]
 * \endcode
 *
 * @param json The JSON array as a value object.
 * @return A list of translated strings as a QList object.
 */
QList<Utils::TranslatedString> Utils::TranslatedString::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
        return QList<TranslatedString> {};

    constexpr auto makeTrString {[](const auto &json) { return TranslatedString::fromJson(json);}};
    constexpr auto stringNotEmpty {[](const auto &str) { return !str.isEmpty();}};

    QList<TranslatedString> translatedStrings {};
    const auto &jsonArray {json.toArray()};

    std::transform(std::begin(jsonArray),
                   std::end(jsonArray),
                   std::back_inserter(translatedStrings),
                   makeTrString);

    QList<TranslatedString> validStrings {};
    std::copy_if(std::begin(translatedStrings),
                 std::end(translatedStrings),
                 std::back_inserter(validStrings),
                 stringNotEmpty);

    return validStrings;
}
