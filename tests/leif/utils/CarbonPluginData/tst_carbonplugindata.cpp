#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <carbonplugindata.h>

class CarbonPluginDataTest : public QObject
{
    Q_OBJECT

public:
    CarbonPluginDataTest() = default;
    virtual ~CarbonPluginDataTest() = default;

private slots:
    void defaultCtorCreatesEmptyAndInvalidObject();
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

QTEST_MAIN(CarbonPluginDataTest)
#include "tst_carbonplugindata.moc"
