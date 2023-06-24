#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <territory.h>
#include <translation.h>
#include <translatedstring.h>

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

    void ctorConstructsObjectWithGivenArguments_data();
    void isValidReturnsCorrectValueDependingOnLocaleTerritory_data();
    void territoryReturnsTheValueSetInCtor_data();

private:
    static bool compareTranslatedStringLists(const QList<Utils::TranslatedString> &lhs, const QList<Utils::TranslatedString> &rhs);
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
    QVERIFY(compareTranslatedStringLists(ter.regions(), regions));
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


////////////////////////////////////////////////////////////////////////////////
void TerritoryTest::ctorConstructsObjectWithGivenArguments_data()
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

void TerritoryTest::isValidReturnsCorrectValueDependingOnLocaleTerritory_data()
{
    ctorConstructsObjectWithGivenArguments_data();
}

void TerritoryTest::territoryReturnsTheValueSetInCtor_data()
{
    ctorConstructsObjectWithGivenArguments_data();
}

////////////////////////////////////////////////////////////////////////////////
bool TerritoryTest::compareTranslatedStringLists(const QList<Utils::TranslatedString> &lhs,
                                                 const QList<Utils::TranslatedString> &rhs)
{
    auto predicate = [](const Utils::TranslatedString &lhs, const Utils::TranslatedString &rhs) {
        if(lhs.id() != rhs.id())
            return false;

        if(lhs.translatedId() != rhs.translatedId())
            return false;

        return true;
    };

    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), predicate);
}

QTEST_MAIN(TerritoryTest)
#include "tst_territory.moc"

