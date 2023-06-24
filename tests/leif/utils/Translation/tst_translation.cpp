#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <translation.h>

class TranslationTest : public QObject
{
    Q_OBJECT

public:
    TranslationTest() = default;
    virtual ~TranslationTest() = default;

private slots:
    void defaultCtorCreatesEmptyObject();
    void stringAndLocaleNameCtorCreatesObject();
    void stringAndLocaleCtorCreateObject();
    void isEmptyReturnsIfStringIsEmpty();
    void stringReturnsCorrectString();
    void setStringSetsTheString();
    void localeReturnsCorrectLocale();
    void setLocaleSetsTheLocale();
    void fromJsonReturnsCorrectTranslationObject();
    void fromJsonArrayReturnsCorrectListOfTranslationObjects();

    void stringAndLocaleNameCtorCreatesObject_data();
    void stringAndLocaleCtorCreateObject_data();
    void isEmptyReturnsIfStringIsEmpty_data();
    void stringReturnsCorrectString_data();
    void setStringSetsTheString_data();
    void localeReturnsCorrectLocale_data();
    void setLocaleSetsTheLocale_data();
    void fromJsonReturnsCorrectTranslationObject_data();
    void fromJsonArrayReturnsCorrectListOfTranslationObjects_data();
};

void TranslationTest::defaultCtorCreatesEmptyObject()
{
    Utils::Translation tr {};

    QVERIFY(tr.isEmpty());
    QVERIFY(tr.string().isEmpty());
    QCOMPARE(tr.locale().country(), QLocale().country());
    QCOMPARE(tr.locale().language(), QLocale().language());
}

void TranslationTest::stringAndLocaleNameCtorCreatesObject()
{
    QFETCH(QString, string);
    QFETCH(QString, localeName);

    Utils::Translation tr {string, localeName};

    QCOMPARE(tr.isEmpty(), string.isEmpty());
    QCOMPARE(tr.string(), string);
    QCOMPARE(tr.locale().name(), QLocale(localeName).name());
}

void TranslationTest::stringAndLocaleCtorCreateObject()
{
    QFETCH(QString, string);
    QFETCH(QLocale, locale);

    Utils::Translation tr {string, locale};

    QCOMPARE(tr.isEmpty(), string.isEmpty());
    QCOMPARE(tr.string(), string);
    QCOMPARE(tr.locale(), locale);
}

void TranslationTest::isEmptyReturnsIfStringIsEmpty()
{
    QFETCH(QString, string);

    Utils::Translation tr {string, QLocale()};

    QCOMPARE(tr.isEmpty(), string.isEmpty());
}

void TranslationTest::stringReturnsCorrectString()
{
    QFETCH(QString, string);

    Utils::Translation tr {string, QLocale()};

    QCOMPARE(tr.string(), string);
}

void TranslationTest::setStringSetsTheString()
{
    QFETCH(QString, before);
    QFETCH(QString, after);

    Utils::Translation tr {before, QLocale()};

    QCOMPARE(tr.string(), before);

    tr.setString(after);

    QCOMPARE(tr.string(), after);
}

void TranslationTest::localeReturnsCorrectLocale()
{
    QFETCH(QLocale, locale);

    Utils::Translation tr {QStringLiteral("some_string"), locale};

    QCOMPARE(tr.locale().name(), locale.name());
}

void TranslationTest::setLocaleSetsTheLocale()
{
    QFETCH(QLocale, before);
    QFETCH(QLocale, after);

    Utils::Translation tr {QStringLiteral("some_string"), before};

    QCOMPARE(tr.locale().name(), before.name());

    tr.setLocale(after);

    QCOMPARE(tr.locale().name(), after.name());
}

void TranslationTest::fromJsonReturnsCorrectTranslationObject()
{
    QFETCH(QString, string);
    QFETCH(QString, locale);
    QFETCH(QByteArray, json);

    auto jsonDoc {QJsonDocument::fromJson(json)};
    auto rootObject {jsonDoc.object()};
    auto tr {Utils::Translation::fromJson(rootObject)};

    QCOMPARE(tr.string(), string);
    QCOMPARE(tr.locale().name(), QLocale(locale).name());
}

void TranslationTest::fromJsonArrayReturnsCorrectListOfTranslationObjects()
{
    QFETCH(int, count);
    QFETCH(QStringList, strings);
    QFETCH(QStringList, locales);
    QFETCH(QByteArray, json);

    auto jsonDoc {QJsonDocument::fromJson(json)};
    auto rootArray {jsonDoc.array()};
    auto translationList {Utils::Translation::fromJsonArray(rootArray)};

    QCOMPARE(translationList.count(), count);
    QCOMPARE(strings.count(), count);
    QCOMPARE(locales.count(), count);

    for(auto i = 0; i < count; i++)
    {
        auto tr {translationList.at(i)};
        auto string {strings.at(i)};
        auto locale {locales.at(i)};

        QCOMPARE(tr.string(), string);
        QCOMPARE(tr.locale().name(), QLocale(locale).name());
    }
}

////////////////////////////////////////////////////////////////////////////////
void TranslationTest::stringAndLocaleNameCtorCreatesObject_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("localeName");

    QTest::addRow("empty") << QString() << QString();
    QTest::addRow("stringOnly") << QStringLiteral("some_string") << QString();
    QTest::addRow("localeOnly") << QString() << QStringLiteral("en_US");
    QTest::addRow("allFields") << QStringLiteral("another_string") << QStringLiteral("pl_PL");
}

void TranslationTest::stringAndLocaleCtorCreateObject_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QLocale>("locale");

    QTest::addRow("empty") << QString() << QLocale();
    QTest::addRow("stringOnly") << QStringLiteral("some_string") << QLocale();
    QTest::addRow("localeOnly") << QString() << QLocale(QLocale::English, QLocale::UnitedStates);
    QTest::addRow("allFields") << QStringLiteral("another_string") << QLocale(QLocale::Polish, QLocale::Poland);
}

void TranslationTest::isEmptyReturnsIfStringIsEmpty_data()
{
    QTest::addColumn<QString>("string");

    QTest::addRow("empty") << QString();
    QTest::addRow("notEmpty1") << QStringLiteral("a");
    QTest::addRow("notEmpty2") << QStringLiteral("some_string");
}

void TranslationTest::stringReturnsCorrectString_data()
{
    isEmptyReturnsIfStringIsEmpty_data();
}

void TranslationTest::setStringSetsTheString_data()
{
    QTest::addColumn<QString>("before");
    QTest::addColumn<QString>("after");

    QTest::addRow("empty_empty") << QString() << QString();
    QTest::addRow("empty_string") << QString() << QStringLiteral("some_string");
    QTest::addRow("string_empty") << QStringLiteral("another_string") << QString();
    QTest::addRow("string_string") << QStringLiteral("Before_string") << QStringLiteral("After_string");
}

void TranslationTest::localeReturnsCorrectLocale_data()
{
    QTest::addColumn<QLocale>("locale");

    QTest::addRow("empty") << QLocale();
    QTest::addRow("us") << QLocale(QStringLiteral("en_US"));
    QTest::addRow("de") << QLocale(QStringLiteral("de_DE"));
    QTest::addRow("pl") << QLocale(QStringLiteral("pl_PL"));
}

void TranslationTest::setLocaleSetsTheLocale_data()
{
    QTest::addColumn<QLocale>("before");
    QTest::addColumn<QLocale>("after");

    QTest::addRow("empty_empty") << QLocale() << QLocale();
    QTest::addRow("empty_pl") << QLocale() << QLocale(QStringLiteral("pl_PL"));
    QTest::addRow("de_empty") << QLocale(QStringLiteral("de_DE")) << QLocale();
    QTest::addRow("uk_us") << QLocale(QStringLiteral("en_GB")) << QLocale(QStringLiteral("en_US"));
}

void TranslationTest::fromJsonReturnsCorrectTranslationObject_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QByteArray>("json");

    QTest::addRow("empty") << QString() << QString() << QByteArray("{ \"string\": \"\", \"locale\": \"\"}");
    QTest::addRow("string_only") << QStringLiteral("some_string") << QString() << QByteArray("{ \"string\": \"some_string\", \"locale\": \"\"}");
    QTest::addRow("locale_only") << QString() << QStringLiteral("pl_PL") << QByteArray("{ \"string\": \"\", \"locale\": \"pl_PL\"}");
    QTest::addRow("all") << QStringLiteral("some_string") << QStringLiteral("pl_PL") << QByteArray("{ \"string\": \"some_string\", \"locale\": \"pl_PL\"}");
}

void TranslationTest::fromJsonArrayReturnsCorrectListOfTranslationObjects_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<QStringList>("strings");
    QTest::addColumn<QStringList>("locales");
    QTest::addColumn<QByteArray>("json");

    QTest::addRow("empty") << 0 << QStringList {} << QStringList {} << QByteArray("[]");

    QStringList strings = {"some_string"};
    QStringList locales = {""};
    QTest::addRow("1_string_empty") << 1 << strings << locales << QByteArray("[ {\"string\": \"some_string\", \"locale\": \"\"} ]");

    strings = {"some_string"};
    locales = {"pl_PL"};
    QTest::addRow("1_string_locale") << 1 << strings << locales << QByteArray("[ {\"string\": \"some_string\", \"locale\": \"pl_PL\"} ]");

    strings = {"some_string", "other_string"};
    locales = {"pl_PL", "de_DE"};
    QTest::addRow("2_rows") << 2 << strings << locales << QByteArray("[ {\"string\": \"some_string\", \"locale\": \"pl_PL\"}, {\"string\": \"other_string\", \"locale\": \"de_DE\"} ]");

    strings = {"some_string", "other_string", "last_string"};
    locales = {"pl_PL", "de_DE", "fr_FR"};
    QTest::addRow("3_rows") << 3 << strings << locales << QByteArray("[ {\"string\": \"some_string\", \"locale\": \"pl_PL\"}, {\"string\": \"other_string\", \"locale\": \"de_DE\"}, {\"string\": \"last_string\", \"locale\": \"fr_FR\"} ]");
}

QTEST_MAIN(TranslationTest)
#include "tst_translation.moc"
