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

    void ctorCreatesObjectWithGivenArguments_data();

private:
    static bool compareTerritoryLists(const QList<Utils::Territory> &lhs, const QList<Utils::Territory> &rhs);
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
    QVERIFY(compareTerritoryLists(cpd.territoryList(), territories));
}

////////////////////////////////////////////////////////////////////////////////
void CarbonPluginDataTest::ctorCreatesObjectWithGivenArguments_data()
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

QTEST_MAIN(CarbonPluginDataTest)
#include "tst_carbonplugindata.moc"
