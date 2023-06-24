#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include <translation.h>
#include <translatedstring.h>

class TranslatedStringTest : public QObject
{
    Q_OBJECT

public:
    TranslatedStringTest() = default;
    virtual ~TranslatedStringTest() = default;

private slots:
    void emptyCtorCreatesEmptyObjectWithEmptyIdAndTranslatedId();
    void ctorCreatesObjectWithCorrectIdAndTranslatedString();
    void isEmptyReturnsTrueOnlyIfIdIsEmpty();
    void idReturnsTheSetString();
    void translatedIdReturnsCorrectStringOnlyIfNotEmpty();
    void translateWillReturnTheStringForTheCurrentLocale();
    void fromJsonReturnsAValidObject();

    void ctorCreatesObjectWithCorrectIdAndTranslatedString_data();
    void isEmptyReturnsTrueOnlyIfIdIsEmpty_data();
    void idReturnsTheSetString_data();
    void translatedIdReturnsCorrectStringOnlyIfNotEmpty_data();
    void translateWillReturnTheStringForTheCurrentLocale_data();
    void fromJsonReturnsAValidObject_data();
};

void TranslatedStringTest::emptyCtorCreatesEmptyObjectWithEmptyIdAndTranslatedId()
{
    Utils::TranslatedString trs {};

    QVERIFY(trs.isEmpty());
    QVERIFY(trs.id().isEmpty());
    QVERIFY(trs.translatedId().isEmpty());
}

void TranslatedStringTest::ctorCreatesObjectWithCorrectIdAndTranslatedString()
{
    QFETCH(QString, id);
    QFETCH(QList<Utils::Translation>, translations);

    Utils::TranslatedString trs {id, translations};

    QCOMPARE(trs.isEmpty(), id.isEmpty());
    QCOMPARE(trs.id(), id);

    if(id.isEmpty())
    {
        QVERIFY(trs.translatedId().isEmpty());
    }

    if(!id.isEmpty() && translations.isEmpty())
    {
        QVERIFY(trs.translatedId().isEmpty());
    }

    if(!id.isEmpty() && !translations.isEmpty())
    {
        QVERIFY(!trs.translatedId().isEmpty());
        QCOMPARE(trs.translatedId(), translations.first().string());
    }
}

void TranslatedStringTest::isEmptyReturnsTrueOnlyIfIdIsEmpty()
{
    QFETCH(QString, id);
    QFETCH(QList<Utils::Translation>, translations);

    Utils::TranslatedString trs {id, translations};

    QCOMPARE(trs.isEmpty(), id.isEmpty());
}

void TranslatedStringTest::idReturnsTheSetString()
{
    QFETCH(QString, id);
    QList<Utils::Translation> translations;

    Utils::TranslatedString trs {id, translations};

    QCOMPARE(trs.id(), id);
}

void TranslatedStringTest::translatedIdReturnsCorrectStringOnlyIfNotEmpty()
{
    QFETCH(QString, id);
    QFETCH(QList<Utils::Translation>, translations);

    Utils::TranslatedString trs {id, translations};

    if(id.isEmpty())
    {
        QVERIFY(trs.translatedId().isEmpty());
    }

    if(!id.isEmpty() && translations.isEmpty())
    {
        QVERIFY(trs.translatedId().isEmpty());
    }

    if(!id.isEmpty() && !translations.isEmpty())
    {
        QVERIFY(!trs.translatedId().isEmpty());
        QCOMPARE(trs.translatedId(), translations.first().string());
    }
}

void TranslatedStringTest::translateWillReturnTheStringForTheCurrentLocale()
{
    QFETCH(QString, translatedString);
    QFETCH(QList<Utils::Translation>, translations);

    QString str = Utils::TranslatedString::translate(translations);

    QCOMPARE(str, translatedString);
}

void TranslatedStringTest::fromJsonReturnsAValidObject()
{
    QFETCH(QString, id);
    QFETCH(QString, translatedString);
    QFETCH(QJsonValue, json);

    Utils::TranslatedString trs {Utils::TranslatedString::fromJson(json)};

    if(json.isNull() || !json.isObject())
    {
        QVERIFY(trs.isEmpty());
    }

    QCOMPARE(trs.id(), id);
    QCOMPARE(trs.translatedId(), translatedString);
    QCOMPARE(trs.isEmpty(), id.isEmpty());
}

////////////////////////////////////////////////////////////////////////////////

void TranslatedStringTest::ctorCreatesObjectWithCorrectIdAndTranslatedString_data()
{
    QTest::addColumn<QString>("id");
    QTest::addColumn<QList<Utils::Translation>>("translations");

    QList<Utils::Translation> translations;
    translations << Utils::Translation(QStringLiteral("some_string"), QLocale());

    QTest::addRow("empty") << QString() << QList<Utils::Translation>();
    QTest::addRow("id_only") << QStringLiteral("some_id") << QList<Utils::Translation>();
    QTest::addRow("trans_only") << QString() << translations;
    QTest::addRow("id_and_trans") << QStringLiteral("some_id") << translations;
}

void TranslatedStringTest::isEmptyReturnsTrueOnlyIfIdIsEmpty_data()
{
    ctorCreatesObjectWithCorrectIdAndTranslatedString_data();
}

void TranslatedStringTest::idReturnsTheSetString_data()
{
    QTest::addColumn<QString>("id");

    QTest::addRow("empty") << QString();
    QTest::addRow("string1") << QStringLiteral("some_id");
    QTest::addRow("string2") << QStringLiteral("another_id");
}

void TranslatedStringTest::translatedIdReturnsCorrectStringOnlyIfNotEmpty_data()
{
    ctorCreatesObjectWithCorrectIdAndTranslatedString_data();
}

void TranslatedStringTest::translateWillReturnTheStringForTheCurrentLocale_data()
{
    QTest::addColumn<QString>("translatedString");
    QTest::addColumn<QList<Utils::Translation>>("translations");

    QTest::addRow("empty") << QString() << QList<Utils::Translation>();

    QString translatedString {QStringLiteral("my_translated_string")};
    QList<Utils::Translation> translations;
    translations << Utils::Translation(translatedString, QLocale());
    QTest::addRow("one") << translatedString << translations;

    translations.clear();
    translations << Utils::Translation(translatedString, QLocale())
                 << Utils::Translation(QStringLiteral("something_else"), QString());
    QTest::addRow("two") << translatedString << translations;

    translations.clear();
    translations << Utils::Translation(translatedString, QLocale())
                 << Utils::Translation(QStringLiteral("something_else"), QString())
                 << Utils::Translation(QStringLiteral("another_one"), QString());
    QTest::addRow("three") << translatedString << translations;
}

void TranslatedStringTest::fromJsonReturnsAValidObject_data()
{
    QTest::addColumn<QString>("id");
    QTest::addColumn<QString>("translatedString");
    QTest::addColumn<QJsonValue>("json");

    QTest::addRow("invalid") << QString() << QString() << QJsonValue {};

    {
        QByteArray jsonStr {"{ \"id\": \"some_id\"}"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("id_only") << QStringLiteral("some_id") << QString() << json;
    }

    {
        QByteArray localeName {QLocale().name().toLatin1()};
        QByteArray jsonStr {"{\"id\": \"some_id\", \"translations\": [ { \"locale\": \"" + localeName + "\", \"string\": \"tr_string\" } ] }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("full1") << QStringLiteral("some_id") << QStringLiteral("tr_string") << json;
    }

    {
        QByteArray localeName {QLocale().name().toLatin1()};
        QByteArray jsonStr {"{\"id\": \"some_id\", \"translations\": [ { \"locale\": \"po_ID\", \"string\": \"gronk\" }, { \"locale\": \"" + localeName + "\", \"string\": \"tr_string\" } ] }"};
        QJsonValue json {QJsonDocument::fromJson(jsonStr).object()};
        QTest::addRow("full2") << QStringLiteral("some_id") << QStringLiteral("tr_string") << json;
    }
}


QTEST_MAIN(TranslatedStringTest)
#include "tst_translatedstring.moc"

