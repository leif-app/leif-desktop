#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <carbonplugindata.h>

#include "common.h"

class CarbonPluginDataTest : public QObject
{
    Q_OBJECT

public:
    CarbonPluginDataTest() = default;
    virtual ~CarbonPluginDataTest() = default;

private slots:
    void defaultCtorCreatesEmptyAndInvalidObject();
    void ctorCreatesObjectWithGivenArguments();
    void isValidReturnsCorrectValueDependingOnNameAndTerritories();
    void nameReturnsValueSetInCtor();
    void descriptionReturnsValueSetInCtor();
    void territoriesReturnsValueSetInCtor();
    void territoryListReturnsTerritoryCodesGivenATerritoryList();
    void territoryNamesReturnsTerritoryNamesGivenATerritoryList();
    void regionsReturnsListOfCorrectRegionsForAGivenTerritory();
    void regionIdsReturnsTheCorrectStringListForAGivenTerritory();
    void translatedRegionIdReturnsCorrectValuesForATerritoryAndRegionId();
    void fromJsonReturnsAnObjectWithNameDescriptionAndTerritories();

    void ctorCreatesObjectWithGivenArguments_data();
    void isValidReturnsCorrectValueDependingOnNameAndTerritories_data();
    void nameReturnsValueSetInCtor_data();
    void descriptionReturnsValueSetInCtor_data();
    void territoriesReturnsValueSetInCtor_data();
    void territoryListReturnsTerritoryCodesGivenATerritoryList_data();
    void territoryNamesReturnsTerritoryNamesGivenATerritoryList_data();
    void regionsReturnsListOfCorrectRegionsForAGivenTerritory_data();
    void regionIdsReturnsTheCorrectStringListForAGivenTerritory_data();
    void translatedRegionIdReturnsCorrectValuesForATerritoryAndRegionId_data();
    void fromJsonReturnsAnObjectWithNameDescriptionAndTerritories_data();

private:
    static bool compareTerritoryLists(const QList<Utils::Territory> &lhs, const QList<Utils::Territory> &rhs);
    void genericConstructorData();
};

void CarbonPluginDataTest::defaultCtorCreatesEmptyAndInvalidObject()
{
    Utils::CarbonPluginData data {};

    QVERIFY(!data.isValid());
    QVERIFY(data.name().isEmpty());
    QVERIFY(data.description().isEmpty());
    QVERIFY(data.territoryList().isEmpty());
    QVERIFY(data.territories().isEmpty());
    QVERIFY(data.territoryNames().isEmpty());
}

void CarbonPluginDataTest::ctorCreatesObjectWithGivenArguments()
{
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {name, description, territories};

    QCOMPARE(cpd.name(), name);
    QCOMPARE(cpd.description(), description);
    QVERIFY(compareTerritoryLists(cpd.territories(), territories));
}

void CarbonPluginDataTest::isValidReturnsCorrectValueDependingOnNameAndTerritories()
{
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {name, description, territories};

    QCOMPARE((name.isEmpty() || territories.isEmpty()), !cpd.isValid());
}

void CarbonPluginDataTest::nameReturnsValueSetInCtor()
{
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {name, description, territories};

    QCOMPARE(name, cpd.name());
}

void CarbonPluginDataTest::descriptionReturnsValueSetInCtor()
{
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {name, description, territories};

    QCOMPARE(description, cpd.description());
}

void CarbonPluginDataTest::territoriesReturnsValueSetInCtor()
{
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {name, description, territories};

    QVERIFY(CarbonPluginDataTest::compareTerritoryLists(territories, cpd.territories()));
}

void CarbonPluginDataTest::territoryListReturnsTerritoryCodesGivenATerritoryList()
{
    QFETCH(QList<QLocale::Territory>, territoryList);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {QStringLiteral("some_name"), QStringLiteral("some_desc"), territories};

    QCOMPARE(cpd.territoryList(), territoryList);
}

void CarbonPluginDataTest::territoryNamesReturnsTerritoryNamesGivenATerritoryList()
{
    QFETCH(QStringList, territoryNames);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {QStringLiteral("some_name"), QStringLiteral("some_desc"), territories};

    QCOMPARE(cpd.territoryNames(), territoryNames);
}

void CarbonPluginDataTest::regionsReturnsListOfCorrectRegionsForAGivenTerritory()
{
    QFETCH(QList<Utils::TranslatedString>, regions);
    QFETCH(QList<Utils::Territory>, territories);
    QFETCH(QLocale::Territory, territory);

    Utils::CarbonPluginData cpd {QStringLiteral("some_name"), QStringLiteral("some_desc"), territories};

    QVERIFY(Common::compareTranslatedStringLists(cpd.regions(territory), regions));
}

void CarbonPluginDataTest::regionIdsReturnsTheCorrectStringListForAGivenTerritory()
{
    QFETCH(QList<Utils::TranslatedString>, regions);
    QFETCH(QList<Utils::Territory>, territories);
    QFETCH(QLocale::Territory, territory);

    Utils::CarbonPluginData cpd {QStringLiteral("some_name"), QStringLiteral("some_desc"), territories};

    QStringList regionIds;
    auto idTrans {[](const Utils::TranslatedString &tStr) { return tStr.id(); }};
    std::transform(std::begin(regions), std::end(regions), std::back_inserter(regionIds), idTrans);

    if(!cpd.isValid())
    {
        QVERIFY(cpd.regionIds(QLocale::UnitedKingdom).isEmpty());
    }

    if(territory == QLocale::AnyTerritory)
    {
        QVERIFY(cpd.regionIds(territory).isEmpty());
    }

    QCOMPARE(cpd.regionIds(territory), regionIds);
}

void CarbonPluginDataTest::translatedRegionIdReturnsCorrectValuesForATerritoryAndRegionId()
{
    QFETCH(QList<Utils::Territory>, territories);
    QFETCH(QLocale::Territory, territory);
    QFETCH(QString, regionId);
    QFETCH(QString, translatedRegionId);

    Utils::CarbonPluginData cpd {QStringLiteral("some_name"), QStringLiteral("some_desc"), territories};

    QCOMPARE(cpd.translatedRegionId(territory, regionId), translatedRegionId);
}

void CarbonPluginDataTest::fromJsonReturnsAnObjectWithNameDescriptionAndTerritories()
{
    QFETCH(QJsonValue, json);
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QList<Utils::Territory>, territories);

    Utils::CarbonPluginData cpd {Utils::CarbonPluginData::fromJson(json)};

    QCOMPARE(cpd.name(), name);
    QCOMPARE(cpd.description(), description);

    if(json.isNull())
    {
        QVERIFY(!cpd.isValid());
    }

    if(!json.isObject())
    {
        QVERIFY(!cpd.isValid());
    }

    QVERIFY(CarbonPluginDataTest::compareTerritoryLists(cpd.territories(), territories));
}

////////////////////////////////////////////////////////////////////////////////
void CarbonPluginDataTest::ctorCreatesObjectWithGivenArguments_data()
{
    genericConstructorData();
}

void CarbonPluginDataTest::isValidReturnsCorrectValueDependingOnNameAndTerritories_data()
{
    genericConstructorData();
}

void CarbonPluginDataTest::nameReturnsValueSetInCtor_data()
{
    genericConstructorData();
}

void CarbonPluginDataTest::descriptionReturnsValueSetInCtor_data()
{
    genericConstructorData();
}

void CarbonPluginDataTest::territoriesReturnsValueSetInCtor_data()
{
    genericConstructorData();
}

void CarbonPluginDataTest::territoryListReturnsTerritoryCodesGivenATerritoryList_data()
{
    QTest::addColumn<QList<QLocale::Territory>>("territoryList");
    QTest::addColumn<QList<Utils::Territory>>("territories");

    QList<QLocale::Territory> territoryList = {QLocale::UnitedKingdom, QLocale::Gambia, QLocale::Malta, QLocale::Germany};

    constexpr auto terTransform {[](const QLocale::Territory &t) { return Utils::Territory {t, QString(), QList<Utils::TranslatedString>()};}};
    QList<Utils::Territory> territories {};

    std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

    QTest::addRow("empty") << QList<QLocale::Territory> {} << QList<Utils::Territory> {};
    QTest::addRow("full") << territoryList << territories;
}

void CarbonPluginDataTest::territoryNamesReturnsTerritoryNamesGivenATerritoryList_data()
{
    QTest::addColumn<QStringList>("territoryNames");
    QTest::addColumn<QList<Utils::Territory>>("territories");

    QList<QLocale::Territory> territoryList = {QLocale::UnitedKingdom, QLocale::Gambia, QLocale::Malta, QLocale::Germany};

    QStringList territoryNames {};
    constexpr auto terToNameTransform {[](const QLocale::Territory &t) { return QLocale::territoryToString(t); }};
    std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territoryNames), terToNameTransform);

    QList<Utils::Territory> territories {};
    constexpr auto terTransform {[](const QLocale::Territory &t) { return Utils::Territory {t, QString(), QList<Utils::TranslatedString>()};}};
    std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

    QTest::addRow("empty") << QStringList {} << QList<Utils::Territory> {};
    QTest::addRow("full") << territoryNames << territories;
}

void CarbonPluginDataTest::regionsReturnsListOfCorrectRegionsForAGivenTerritory_data()
{
    QTest::addColumn<QList<Utils::TranslatedString>>("regions");
    QTest::addColumn<QList<Utils::Territory>>("territories");
    QTest::addColumn<QLocale::Territory>("territory");

    QList<Utils::TranslatedString> emptyRegions {};
    QList<Utils::Territory> emptyTerritories {};

    QStringList regionIds = {QStringLiteral("RegionA"), QStringLiteral("RegionB"), QStringLiteral("RegionC")};
    QList<Utils::TranslatedString> regions {};
    constexpr auto regionTransform {[](const auto &regionId) { return Utils::TranslatedString {regionId, QList<Utils::Translation> {}}; }};
    std::transform(std::begin(regionIds), std::end(regionIds), std::back_inserter(regions), regionTransform);

    QList<QLocale::Territory> territoryList = {QLocale::UnitedKingdom, QLocale::Gambia, QLocale::Malta, QLocale::Germany};
    QList<Utils::Territory> territories {};
    auto terTransform {[&](const QLocale::Territory &t) {
        if(t == QLocale::Gambia)
            return Utils::Territory {t, QString(), regions};

        return Utils::Territory {t, QString(), QList<Utils::TranslatedString>()};
    }};
    std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

    QTest::addRow("invalid") << emptyRegions << emptyTerritories << QLocale::AnyTerritory;
    QTest::addRow("unknown_territory") << emptyRegions << territories << QLocale::Poland;
    QTest::addRow("territory_no_regions") << emptyRegions << territories << territoryList.first();
    QTest::addRow("regions") << regions << territories << territoryList.at(1);
}

void CarbonPluginDataTest::regionIdsReturnsTheCorrectStringListForAGivenTerritory_data()
{
    regionsReturnsListOfCorrectRegionsForAGivenTerritory_data();
}

void CarbonPluginDataTest::translatedRegionIdReturnsCorrectValuesForATerritoryAndRegionId_data()
{
    QTest::addColumn<QList<Utils::Territory>>("territories");
    QTest::addColumn<QLocale::Territory>("territory");
    QTest::addColumn<QString>("regionId");
    QTest::addColumn<QString>("translatedRegionId");

    QList<Utils::Territory> emptyTerritories {};
    QLocale::Territory anyTerritory {QLocale::AnyTerritory};
    QString emptyRegionId {};
    QString emptyTranslatedRegionId {};

    QList<QLocale::Territory> territoryList = {QLocale::UnitedKingdom, QLocale::Germany, QLocale::SouthAfrica};
    QStringList regionIds = {QStringLiteral("Region A"), QStringLiteral("Region B"), QStringLiteral("Region C")};

    QTest::addRow("invalid_cpd") << emptyTerritories << anyTerritory << emptyRegionId << emptyTranslatedRegionId;

    {
        QList<Utils::Territory> territories {};
        auto terTransform {[](const QLocale::Territory &t) {return Utils::Territory {t, QString {}, QList<Utils::TranslatedString> {}}; }};
        std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

        QTest::addRow("valid_wrong_territory") << territories << QLocale::Gambia << regionIds.at(1) << emptyTranslatedRegionId;
    }

    {
        QList<Utils::Territory> territories {};

        auto terTransform {[&](const QLocale::Territory &t) {return Utils::Territory {t, QString {}, QList<Utils::TranslatedString> {}}; }};
        std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

        QTest::addRow("valid_territory_no_regions") << territories << territoryList.at(1) << QStringLiteral("any Region") << emptyTranslatedRegionId;
    }

    {
        int translationIndex = 0;
        QList<Utils::Territory> territories {};
        QList<Utils::TranslatedString> regions {};
        QList<Utils::Translation> translations {};

        QStringList translationList = {QStringLiteral("Translation A"), QStringLiteral("Translation B"), QStringLiteral("Translation C")};

        auto terTransform {[&](const QLocale::Territory &t) {return Utils::Territory {t, QString {}, regions}; }};
        auto regTransform {[&](const QString &id) {
            QList<Utils::Translation> tr = {translations.at(translationIndex++)};
            return Utils::TranslatedString {id, tr};
        }};
        auto trTransform {[](const QString &str) { return Utils::Translation {str, QLocale()}; }};

        std::transform(std::begin(translationList), std::end(translationList), std::back_inserter(translations), trTransform);
        std::transform(std::begin(regionIds), std::end(regionIds), std::back_inserter(regions), regTransform);
        std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

        QTest::addRow("valid_territory_with_regions_unknown_region")
            << territories << territoryList.at(1) << QStringLiteral("wrong Region") << emptyTranslatedRegionId;
    }

    {
        QList<Utils::Territory> territories {};
        QList<Utils::TranslatedString> regions {};

        auto terTransform {[&](const QLocale::Territory &t) {return Utils::Territory {t, QString {}, regions}; }};
        auto regTransform {[&](const QString &id) {return Utils::TranslatedString {id, QList<Utils::Translation> {}}; }};

        std::transform(std::begin(regionIds), std::end(regionIds), std::back_inserter(regions), regTransform);
        std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

        QTest::addRow("valid_territory_with_regions_no_translation")
            << territories << territoryList.at(1) << regionIds.at(1) << regionIds.at(1);
    }

    {
        int translationIndex = 0;
        QList<Utils::Territory> territories {};
        QList<Utils::TranslatedString> regions {};
        QList<Utils::Translation> translations {};

        QStringList translationList = {QStringLiteral("Translation A"), QStringLiteral("Translation B"), QStringLiteral("Translation C")};

        auto terTransform {[&](const QLocale::Territory &t) {return Utils::Territory {t, QString {}, regions}; }};
        auto regTransform {[&](const QString &id) {
            QList<Utils::Translation> tr = {translations.at(translationIndex++)};
            return Utils::TranslatedString {id, tr};
        }};
        auto trTransform {[](const QString &str) { return Utils::Translation {str, QLocale()}; }};

        std::transform(std::begin(translationList), std::end(translationList), std::back_inserter(translations), trTransform);
        std::transform(std::begin(regionIds), std::end(regionIds), std::back_inserter(regions), regTransform);
        std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), terTransform);

        QTest::addRow("valid_territory_with_regions_and_translations")
            << territories << territoryList.at(1) << regionIds.at(1) << translationList.at(1);
    }
}

void CarbonPluginDataTest::fromJsonReturnsAnObjectWithNameDescriptionAndTerritories_data()
{
    QTest::addColumn<QJsonValue>("json");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QList<Utils::Territory>>("territories");

    QString emptyName {};
    QString emptyDescription {};
    QList<Utils::Territory> emptyTerritories {};

    constexpr auto makeTerr {[](const auto &ter) { return Utils::Territory {ter, QString {}, QList<Utils::TranslatedString> {}}; }};
    constexpr auto makeJson {[](const auto &str) { return QJsonDocument::fromJson(str).object(); }};

    QString name {"aName"};
    QString description {"a Description"};

    QList<QLocale::Territory> territoryList = {QLocale::UnitedKingdom, QLocale::Spain, QLocale::Malta};
    QList<Utils::Territory> territories;
    QByteArray territoriesString {};

    {
        std::transform(std::begin(territoryList), std::end(territoryList), std::back_inserter(territories), makeTerr);

        QString tStr {"\"territories\": [{\"territory\": %1}, {\"territory\": %2}, {\"territory\": %3}]"};
        foreach (const auto &t, territoryList)
        {
            tStr = tStr.arg(t);
        }

        territoriesString = tStr.toLatin1();
    }

    QTest::addRow("json_null") << QJsonValue {} << emptyName << emptyDescription << emptyTerritories;

    {
        QByteArray str {"[{\"id\": \"someid\"}]"};
        QJsonValue json = QJsonDocument::fromJson(str).array();

        QTest::addRow("json_non_object") << json << emptyName << emptyDescription << emptyTerritories;
    }

    {
        QByteArray str {"{\"name\": \"" + name.toLatin1() + "\"}"};
        QJsonValue json = makeJson(str);
        QTest::addRow("name_only") << json << name << emptyDescription << emptyTerritories;
    }

    {
        QByteArray str {"{\"description\": \"" + description.toLatin1() + "\"}"};
        QJsonValue json = makeJson(str);
        QTest::addRow("description_only") << json << emptyName << description << emptyTerritories;
    }

    {
        QByteArray str {"{" + territoriesString + "}"};
        QJsonValue json = makeJson(str);
        QTest::addRow("territories_only") << json << emptyName << emptyDescription << territories;
    }

    {
        QByteArray str {"{\"name\": \"" + name.toLatin1() + "\", \"description\": \"" + description.toLatin1() + "\"}"};
        QJsonValue json = makeJson(str);
        QTest::addRow("name_and_description") << json << name << description << emptyTerritories;
    }

    {
        QByteArray str {"{\"name\": \"" + name.toLatin1() + "\", " + territoriesString + " }"};
        QJsonValue json = makeJson(str);
        QTest::addRow("name_and_territories") << json << name << emptyDescription << territories;
    }

    {
        QByteArray str {"{\"description\": \"" + description.toLatin1() + "\", " + territoriesString + " }"};
        QJsonValue json = makeJson(str);
        QTest::addRow("description_and_territories") << json << emptyName << description << territories;
    }

    {
        QByteArray str {"{\"name\": \"" + name.toLatin1() + "\", \"description\": \"" + description.toLatin1() + "\", " + territoriesString + " }"};
        QJsonValue json = makeJson(str);
        QTest::addRow("all") << json << name << description << territories;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CarbonPluginDataTest::compareTerritoryLists(const QList<Utils::Territory> &lhs,
                                                 const QList<Utils::Territory> &rhs)
{
    auto predicate = [](const Utils::Territory &lhs, const Utils::Territory &rhs) {
        if(lhs.territory() != rhs.territory())
            return false;

        if(lhs.isValid() != rhs.isValid())
            return false;

        if(lhs.description() != rhs.description())
            return false;

        if(lhs.hasRegions() != rhs.hasRegions())
            return false;

        if(!Common::compareTranslatedStringLists(lhs.regions(), rhs.regions()))
            return false;

        return true;
    };

    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), predicate);
}

void CarbonPluginDataTest::genericConstructorData()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QList<Utils::Territory>>("territories");

    QList<Utils::Territory> emptyTerritories {};
    QList<Utils::TranslatedString> emptyRegions {};
    QString emptyString {};

    QString name {QStringLiteral("A name")};
    QString description {QStringLiteral("Some description")};
    QList<Utils::Territory> territories = {Utils::Territory {QLocale::UnitedKingdom, emptyString, emptyRegions},
                                           Utils::Territory {QLocale::Gambia, emptyString, emptyRegions},
                                           Utils::Territory {QLocale::Italy, emptyString, emptyRegions}};

    QTest::addRow("empty") << emptyString << emptyString << emptyTerritories;
    QTest::addRow("name_only") << name << emptyString << emptyTerritories;
    QTest::addRow("description_only") << emptyString << description << emptyTerritories;
    QTest::addRow("territories_only") << emptyString << emptyString << territories;
    QTest::addRow("name_description") << name << description << emptyTerritories;
    QTest::addRow("name_territories") << name << emptyString << territories;
    QTest::addRow("description_territories") << emptyString << description << territories;
    QTest::addRow("all") << name << description << territories;
}

QTEST_MAIN(CarbonPluginDataTest)
#include "tst_carbonplugindata.moc"
