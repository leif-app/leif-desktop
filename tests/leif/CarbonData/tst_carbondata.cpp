#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <carbondata.h>

class CarbonDataTest : public QObject
{
    Q_OBJECT

public:
    CarbonDataTest() = default;
    virtual ~CarbonDataTest() = default;

private slots:
    void defaultCtorCreatesEmptyObject();

    void ctorCreatesObjectUsingCorrectValues_data();
    void ctorCreatesObjectUsingCorrectValues();

    void createOkObject_data();
    void createOkObject();

    void createErrorObject_data();
    void createErrorObject();
};

void CarbonDataTest::defaultCtorCreatesEmptyObject()
{
    CarbonData cd{};

    QCOMPARE(cd.co2PerkWhNow, -1);
    QCOMPARE(cd.co2PerkWhNext, -1);
    QCOMPARE(cd.co2PerkWhLater, -1);
    QVERIFY(!cd.isValid);
    QVERIFY(cd.errorString.isEmpty());
    QCOMPARE(cd.validFrom, QDateTime());
    QCOMPARE(cd.validTo, QDateTime());
}

void CarbonDataTest::ctorCreatesObjectUsingCorrectValues_data()
{
    QTest::addColumn<int>("co2PerkWhNow");
    QTest::addColumn<int>("co2PerkWhNext");
    QTest::addColumn<int>("co2PerkWhLater");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<QString>("errorString");
    QTest::addColumn<QDateTime>("validFrom");
    QTest::addColumn<QDateTime>("validTo");

    QTest::newRow("empty") << 0 << 0 << 0 << false << QString() << QDateTime() << QDateTime();
    QTest::newRow("firstValid") << 100 << 0 << 0 << false << QString() << QDateTime() << QDateTime();
    QTest::newRow("secondValid") << 0 << 100 << 0 << false << QString() << QDateTime() << QDateTime();
    QTest::newRow("thirdValid") << 0 << 0 << 100 << false << QString() << QDateTime() << QDateTime();
    QTest::newRow("fourthValid") << 0 << 0 << 0 << true << QString() << QDateTime() << QDateTime();
    QTest::newRow("fifthValid") << 0 << 0 << 0 << false << QStringLiteral("Test_string") << QDateTime() << QDateTime();
    QTest::newRow("sixthValid") << 0 << 0 << 0 << false << QString() << QDateTime::currentDateTime() << QDateTime();
    QTest::newRow("lastValid") << 0 << 0 << 0 << false << QString() << QDateTime() << QDateTime::currentDateTime();
    QTest::newRow("allValid") << 34 << 99 << 122 << true << QStringLiteral("some_string") << QDateTime::currentDateTime() << QDateTime::currentDateTime();
}

void CarbonDataTest::ctorCreatesObjectUsingCorrectValues()
{
    QFETCH(int, co2PerkWhNow);
    QFETCH(int, co2PerkWhNext);
    QFETCH(int, co2PerkWhLater);
    QFETCH(bool, isValid);
    QFETCH(QString, errorString);
    QFETCH(QDateTime, validFrom);
    QFETCH(QDateTime, validTo);

    CarbonData cd(co2PerkWhNow, co2PerkWhNext, co2PerkWhLater, isValid, errorString, validFrom, validTo);

    QCOMPARE(cd.co2PerkWhNow, co2PerkWhNow);
    QCOMPARE(cd.co2PerkWhNext, co2PerkWhNext);
    QCOMPARE(cd.co2PerkWhLater, co2PerkWhLater);
    QCOMPARE(cd.isValid, isValid);
    QCOMPARE(cd.errorString, errorString);
    QCOMPARE(cd.validFrom, validFrom);
    QCOMPARE(cd.validTo, validTo);
}

void CarbonDataTest::createOkObject_data()
{
    QTest::addColumn<int>("co2PerkWhNow");
    QTest::addColumn<int>("co2PerkWhNext");
    QTest::addColumn<int>("co2PerkWhLater");
    QTest::addColumn<QDateTime>("validFrom");
    QTest::addColumn<QDateTime>("validTo");

    QTest::addRow("empty") << 0 << 0 << 0 << QDateTime() << QDateTime();
    QTest::addRow("firstValid") << 100 << 0 << 0 << QDateTime() << QDateTime();
    QTest::addRow("secondValid") << 0 << 100 << 0 << QDateTime() << QDateTime();
    QTest::addRow("thirdValid") << 0 << 0 << 100 << QDateTime() << QDateTime();
    QTest::addRow("fourthValid") << 0 << 0 << 0 << QDateTime::currentDateTime() << QDateTime();
    QTest::addRow("allValid") << 100 << 200 << 399 << QDateTime::currentDateTime() << QDateTime::currentDateTime();
}

void CarbonDataTest::createOkObject()
{
    QFETCH(int, co2PerkWhNow);
    QFETCH(int, co2PerkWhNext);
    QFETCH(int, co2PerkWhLater);
    QFETCH(QDateTime, validFrom);
    QFETCH(QDateTime, validTo);

    CarbonData cd = CarbonData::ok(co2PerkWhNow, co2PerkWhNext, co2PerkWhLater, validFrom, validTo);

    QCOMPARE(cd.co2PerkWhNow, co2PerkWhNow);
    QCOMPARE(cd.co2PerkWhNext, co2PerkWhNext);
    QCOMPARE(cd.co2PerkWhLater, co2PerkWhLater);
    QCOMPARE(cd.validFrom, validFrom);
    QCOMPARE(cd.validTo, validTo);

    QVERIFY(cd.isValid);
    QVERIFY(cd.errorString.isEmpty());
}

void CarbonDataTest::createErrorObject_data()
{
    QTest::addColumn<QString>("errorString");

    QTest::addRow("empty") << QString();
    QTest::addRow("full") << QStringLiteral("An error has occured.");
}

void CarbonDataTest::createErrorObject()
{
    QFETCH(QString, errorString);

    CarbonData cd = CarbonData::error(errorString);

    QVERIFY(!cd.isValid);
    QCOMPARE(cd.errorString, errorString);
}

QTEST_MAIN(CarbonDataTest)
#include "tst_carbondata.moc"
