/**
 * \brief Defines the CarbonData reply structure.
 *
 * The CarbonData reply structure contains the cost of a kilowatt hour in
 * grams of CO2 and may contain an error, if there was an issue getting the data.
 *
 * \sa IDataProvider
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 *
 * \date 17.09.2022
 */
#ifndef CARBONDATA_H
#define CARBONDATA_H

#include <QDateTime>
#include <QString>

struct CarbonData
{
    inline CarbonData();
    inline CarbonData(int _co2Now,
                      int _co2Next,
                      int _co2Later,
                      bool _isValid,
                      const QString &_errorString,
                      const QDateTime &_validFrom,
                      const QDateTime &_validTo);

    inline static CarbonData ok(int _co2Now,
                                int _co2Next,
                                int _co2Later,
                                const QDateTime &_validFrom,
                                const QDateTime &_validTo);

    inline static CarbonData error(const QString &_errorString);

    int co2PerkWhNow;
    int co2PerkWhNext;
    int co2PerkWhLater;
    bool isValid;
    QString errorString;
    QDateTime validFrom;
    QDateTime validTo;
};

CarbonData::CarbonData():
    CarbonData(-1, -1, -1, false, QString(), QDateTime(), QDateTime())
{}

CarbonData::CarbonData(int _co2Now,
                       int _co2Next,
                       int _co2Later,
                       bool _isValid,
                       const QString &_errorString,
                       const QDateTime &_validFrom,
                       const QDateTime &_validTo):
    co2PerkWhNow {_co2Now},
    co2PerkWhNext {_co2Next},
    co2PerkWhLater {_co2Later},
    isValid {_isValid},
    errorString {_errorString},
    validFrom {_validFrom},
    validTo {_validTo}
{}

/* static */
CarbonData CarbonData::ok(int _co2Now,
                          int _co2Next,
                          int _co2Later,
                          const QDateTime &_validFrom,
                          const QDateTime &_validTo)
{
    return CarbonData(_co2Now, _co2Next, _co2Later, true, QString(),
                      _validFrom, _validTo);
}

/* static */
CarbonData CarbonData::error(const QString &_errorString)
{
    return CarbonData(-1, -1, -1, false, _errorString, QDateTime(), QDateTime());
}

#endif // CARBONDATA_H
