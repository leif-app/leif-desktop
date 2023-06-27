#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <territory.h>
#include <translation.h>
#include <translatedstring.h>

#include "common.h"

class TerritoryTest : public QObject
{
    Q_OBJECT

public:
    TerritoryTest() = default;
    virtual ~TerritoryTest() = default;

private slots:
    void defaultCtorConstructsAnEmptyObject();
    void ctorConstructsObjectWithGivenArguments();
    void isValidReturnsCorrectValueDependingOnLocaleTerritory();
    void territoryReturnsTheValueSetInCtor();
    void descriptionReturnsTheValueSetInCtor();
    void hasRegionReturnsTrueOrFalseDependingOnRegionsList();
    void regionsReturnsTheValueSetInCtor();
    void fromJsonReturnsTheCorrectObject();
    void fromJsonArrayCreatesOneOrMoreObjects();

    void ctorConstructsObjectWithGivenArguments_data();
    void isValidReturnsCorrectValueDependingOnLocaleTerritory_data();
    void territoryReturnsTheValueSetInCtor_data();
    void descriptionReturnsTheValueSetInCtor_data();
    void hasRegionReturnsTrueOrFalseDependingOnRegionsList_data();
    void regionsReturnsTheValueSetInCtor_data();
    void fromJsonReturnsTheCorrectObject_data();
    void fromJsonArrayCreatesOneOrMoreObjects_data();

private:
    void genericConstructorData();
};

void TerritoryTest::defaultCtorConstructsAnEmptyObject()
{
    Utils::Territory ter {};

    QCOMPARE(ter.territory(), QLocale::AnyCountry);
    QVERIFY(ter.description().isEmpty());
    QVERIFY(!ter.isValid());
    QVERIFY(!ter.hasRegions());
    QVERIFY(ter.regions().isEmpty());
}

void TerritoryTest::ctorConstructsObjectWithGivenArguments()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);

    Utils::Territory ter {territory, description, regions};

    QCOMPARE(ter.territory(), territory);
    QCOMPARE(ter.description(), description);
    QVERIFY(Common::compareTranslatedStringLists(ter.regions(), regions));
}

void TerritoryTest::isValidReturnsCorrectValueDependingOnLocaleTerritory()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);

    Utils::Territory ter {territory, description, regions};

    QCOMPARE(ter.isValid(), territory != QLocale::AnyCountry);
}

void TerritoryTest::territoryReturnsTheValueSetInCtor()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);

    Utils::Territory ter {territory, description, regions};

    QCOMPARE(ter.territory(), territory);
}

void TerritoryTest::descriptionReturnsTheValueSetInCtor()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);

    Utils::Territory ter {territory, description, regions};

    QCOMPARE(ter.description(), description);
}

void TerritoryTest::hasRegionReturnsTrueOrFalseDependingOnRegionsList()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);

    Utils::Territory ter {territory, description, regions};

    QCOMPARE(ter.hasRegions(), !regions.isEmpty());
}

void TerritoryTest::regionsReturnsTheValueSetInCtor()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);

    Utils::Territory ter {territory, description, regions};

    QVERIFY(Common::compareTranslatedStringLists(ter.regions(), regions));
}

void TerritoryTest::fromJsonReturnsTheCorrectObject()
{
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, description);
    QFETCH(QList<Utils::TranslatedString>, regions);
    QFETCH(QJsonValue, json);

    Utils::Territory ter {Utils::Territory::fromJson(json)};

    if(json.isNull() || !json.isObject())
    {
        QVERIFY(!ter.isValid());
        QVERIFY(territory == QLocale::AnyCountry);
    }

    QCOMPARE(ter.isValid(), territory != QLocale::AnyTerritory);
    QCOMPARE(ter.territory(), territory);

    if(ter.isValid())
    {
        QCOMPARE(ter.description(), description);
        QCOMPARE(ter.hasRegions(), !regions.isEmpty());
        QVERIFY(Common::compareTranslatedStringLists(ter.regions(), regions));
    }
}

void TerritoryTest::fromJsonArrayCreatesOneOrMoreObjects()
{
    QFETCH(int, count);
    QFETCH(QJsonValue, json);

    QList<Utils::Territory> terList {Utils::Territory::fromJsonArray(json)};

    if(json.isNull() || !json.isArray())
    {
        QVERIFY(terList.isEmpty());
    }

    QCOMPARE(count, terList.count());
}

////////////////////////////////////////////////////////////////////////////////
void TerritoryTest::ctorConstructsObjectWithGivenArguments_data()
{
    genericConstructorData();
}

void TerritoryTest::isValidReturnsCorrectValueDependingOnLocaleTerritory_data()
{
    genericConstructorData();
}

void TerritoryTest::territoryReturnsTheValueSetInCtor_data()
{
    genericConstructorData();
}

void TerritoryTest::descriptionReturnsTheValueSetInCtor_data()
{
    genericConstructorData();
}

void TerritoryTest::hasRegionReturnsTrueOrFalseDependingOnRegionsList_data()
{
    genericConstructorData();
}

void TerritoryTest::regionsReturnsTheValueSetInCtor_data()
{
    genericConstructorData();
}

void TerritoryTest::fromJsonReturnsTheCorrectObject_data()
{
    QTest::addColumn<QLocale::Territory>("territory");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QList<Utils::TranslatedString>>("regions");
    QTest::addColumn<QJsonValue>("json");

    QLocale::Territory territory {QLocale::TokelauTerritory};
    QString description {QStringLiteral("My Description")};
    QList<Utils::TranslatedString> regions;

    regions << Utils::TranslatedString {QStringLiteral("id1"), QList<Utils::Translation> {}};
    regions << Utils::TranslatedString {QStringLiteral("id2"), QList<Utils::Translation> {}};
    regions << Utils::TranslatedString {QStringLiteral("id3"), QList<Utils::Translation> {}};

    QTest::addRow("empty1") << QLocale::AnyCountry << QString {} << QList<Utils::TranslatedString> {} << QJsonValue {};

    {
        QByteArray jsonStr {"[]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("empty_notobject1") << QLocale::AnyCountry << QString {} << QList<Utils::TranslatedString> {} << json;
    }

    {
        QByteArray jsonStr {"{ \"wrong\": \"" + QByteArray::number(territory) + "\" }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("no_territory") << QLocale::AnyCountry << QString {} << QList<Utils::TranslatedString> {} << json;
    }

    {
        QByteArray jsonStr {"{ \"territory\": " + QByteArray::number(territory) + " }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("territory_only") << territory << QString {} << QList<Utils::TranslatedString> {} << json;
    }

    {
        QByteArray jsonStr {"{ \"description\": \"" + description.toLatin1() + "\" }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("description_only") << QLocale::AnyCountry << description << QList<Utils::TranslatedString> {} << json;
    }

    {
        QByteArray jsonStr {"{ \"regions\": [ { \"id\": \"id1\" }, { \"id\": \"id2\" }, { \"id\": \"id3\" } ] }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("regions_only") << QLocale::AnyCountry << QString {} << regions << json;
    }

    {
        QByteArray jsonStr {"{ \"territory\": " + QByteArray::number(territory) + ", \"description\": \"" + description.toLatin1() + "\" }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("territory_description") << territory << description << QList<Utils::TranslatedString> {} << json;
    }

    {
        QByteArray jsonStr {"{ \"territory\": " + QByteArray::number(territory) + ", \"regions\": [ { \"id\": \"id1\" }, { \"id\": \"id2\" }, { \"id\": \"id3\" } ] }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("territory_regions") << territory << QString {} << regions << json;
    }

    {
        QByteArray jsonStr {"{ \"description\": \"" + description.toLatin1() + "\", \"regions\": [ { \"id\": \"id1\" }, { \"id\": \"id2\" }, { \"id\": \"id3\" } ] }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("description_regions") << QLocale::AnyTerritory << description << regions << json;
    }

    {
        QByteArray jsonStr {"{ \"territory\": " + QByteArray::number(territory) + ", \"description\": \"" + description.toLatin1() + "\", \"regions\": [ { \"id\": \"id1\" }, { \"id\": \"id2\" }, { \"id\": \"id3\" } ] }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("all") << territory << description << regions << json;
    }
}

void TerritoryTest::fromJsonArrayCreatesOneOrMoreObjects_data()
{
    QTest::addColumn<int>("count");
    QTest::addColumn<QJsonValue>("json");

    {
        QByteArray jsonStr {"[]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("empty") << 0 << json;
    }

    {
        QJsonValue json {};
        QTest::addRow("json_null") << 0 << json;
    }

    {
        QByteArray jsonStr {"{ \"id\": \"some_id\" }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("json_not_array") << 0 << json;
    }

    {
        QByteArray jsonStr {"[ { \"territory\": 246 } ]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("one_valid") << 1 << json;
    }

    {
        QByteArray jsonStr {"[ { \"territory\": 246 }, { \"description\": \"somedesc\" } ]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("one_valid_one_invalid") << 1 << json;
    }

    {
        QByteArray jsonStr {"[ { \"territory\": 246 }, { \"description\": \"somedesc\" }, { \"description\": \"other\" } ]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("one_valid_two_invalid") << 1 << json;
    }

    {
        QByteArray jsonStr {"[ { \"territory\": 246 }, { \"territory\": 247 } ]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("two_valid") << 2 << json;
    }

    {
        QByteArray jsonStr {"[ { \"territory\": 246 }, { \"territory\": 247 }, { \"dingo\": \"that sit\" } ]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("two_valid_one_invalid") << 2 << json;
    }

    {
        QByteArray jsonStr {"[ { \"territory\": 246 }, { \"territory\": 247 }, { \"territory\": 248 } ]"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).array()};
        QTest::addRow("three_valid") << 3 << json;
    }
}

////////////////////////////////////////////////////////////////////////////////
void TerritoryTest::genericConstructorData()
{
    QTest::addColumn<QLocale::Territory>("territory");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QList<Utils::TranslatedString>>("regions");

    QLocale::Territory territory {QLocale::TokelauTerritory};
    QString description {QStringLiteral("My Description")};
    QList<Utils::TranslatedString> regions;

    regions << Utils::TranslatedString {QStringLiteral("id1"), QList<Utils::Translation> {}};
    regions << Utils::TranslatedString {QStringLiteral("id2"), QList<Utils::Translation> {}};
    regions << Utils::TranslatedString {QStringLiteral("id3"), QList<Utils::Translation> {}};

    QTest::addRow("empty") << QLocale::AnyCountry << QString {} << QList<Utils::TranslatedString> {};
    QTest::addRow("territory_only") << territory << QString {} << QList<Utils::TranslatedString> {};
    QTest::addRow("description_only") << QLocale::AnyCountry << description << QList<Utils::TranslatedString> {};
    QTest::addRow("regions_only") << QLocale::AnyCountry << QString {} << regions;
    QTest::addRow("territory_description") << territory << description << QList<Utils::TranslatedString> {};
    QTest::addRow("territory_regions") << territory << QString {} << regions;
    QTest::addRow("description_regions") << QLocale::AnyTerritory << description << regions;
    QTest::addRow("all") << territory << description << regions;
}

QTEST_MAIN(TerritoryTest)
#include "tst_territory.moc"

