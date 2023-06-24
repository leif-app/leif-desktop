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

Utils::TranslatedString::TranslatedString(const QString &id, const QList<Translation> &translations):
    _id {id}, _translatedId {TranslatedString::translate(translations)}
{}

bool Utils::TranslatedString::isEmpty() const
{
    return id().isEmpty();
}

QString Utils::TranslatedString::id() const
{
    return _id;
}

QString Utils::TranslatedString::translatedId() const
{
    if(isEmpty())
        return QString();

    return _translatedId;
}

QString Utils::TranslatedString::translate(const QList<Translation> &translations)
{
    auto localeFits = [](const Utils::Translation &translation) -> bool {
        if(translation.isEmpty()) return false;

        auto trLocaleName {translation.locale().name()};
        auto localeName {QLocale().name()};

        return trLocaleName == localeName;
    };

    auto result = std::find_if(std::begin(translations), std::end(translations), localeFits);

    if(result == std::end(translations))
        return QString();

    return result->string();
}

Utils::TranslatedString Utils::TranslatedString::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
        return TranslatedString();

    auto jsonObject = json.toObject();

    auto id = jsonObject.value(QStringLiteral("id")).toString();

    QList<Translation> translations;

    if(jsonObject.contains(QStringLiteral("translations")))
        translations = Translation::fromJsonArray(jsonObject.value(QStringLiteral("translations")));

    return TranslatedString (id, translations);
}

QList<Utils::TranslatedString> Utils::TranslatedString::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
        return QList<TranslatedString>();

    QList<TranslatedString> translatedStrings;

    auto addTranslatedString {[&](const auto &jsonValue) {
        auto translatedString {TranslatedString::fromJson(jsonValue)};

        if(!translatedString.isEmpty())
            translatedStrings << translatedString;
    }};

    const auto &jsonArray {json.toArray()};
    std::for_each(std::begin(jsonArray), std::end(jsonArray), addTranslatedString);

    return translatedStrings;
}
