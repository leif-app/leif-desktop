/**
 * @brief Implements the utility class methods.
 *
 * @author Dariusz Scharsig
 *
 * @date 21.09.2022
 */
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScopeGuard>
#include <QUrl>
#include <QDebug>
#include "utilities.h"

/**
 * @brief Requests the carbon data from the national grid API.
 *
 * For this to happen we need to provide this static method with a valid
 * \p network access manager,a \p regionID, a \p from and a \p to date.
 *
 * \remark This method blocks until the reply is ready, but nor longer than 5
 * seconds.
 *
 * @param network The network access manager to use for the call.
 * @param regionID The region ID to ask the API for.
 * @param from The start date for the request.
 * @param to The to date for the request.
 * @return The carbon data as a CarbonData object.
 */
CarbonData Utilities::requestCarbonData(QNetworkAccessManager *network,
                                        int regionID,
                                        const QDateTime &from,
                                        const QDateTime &to)
{
    if(network == nullptr)
    {
        return CarbonData::error(QStringLiteral("No network object provided."));
    }

    QString format = QStringLiteral("yyyy-MM-ddThh:mm");
    QString fromString = from.toString(format) + "Z";
    QString toString = to.toString(format) + "Z";

    QString address = QStringLiteral("https://api.carbonintensity.org.uk/regional/intensity/%1/%2/regionid/%3");
    QUrl url = QString(address).arg(fromString, toString).arg(regionID);
    QNetworkReply *reply = network->get(QNetworkRequest(url));

    // This guarantees that reply will be cleaned up no matter when and how
    // we exit this method.
    auto cleanup = qScopeGuard([=]() { reply->deleteLater();});

    Utilities::awaitSignal(reply, &QNetworkReply::finished);
    if(!reply->isFinished())
    {
        return CarbonData::error(QStringLiteral("API didn't reply in time."));
    }

    if(reply->error() != QNetworkReply::NoError)
    {
        QString msg("Network request could not be processed. Error: %1(%2).");
        msg = msg.arg(reply->error()).arg(reply->errorString());

        return CarbonData::error(msg);
    }

    QByteArray data = reply->readAll();

    return Utilities::fromByteArray(data);
}

/**
 * @brief Creates a \c CarbonData response from a JSON \p data reply.
 *
 * This static method will transform an API JSON response, which is avaialable
 * as byte array \p data into a \c CarbonData object. For this it will parse
 * the JSON, check if it has an error response and call fromApiError() or
 * fromApiResponse().
 *
 * \sa fromApiError()
 * \sa fromApiResponse()
 *
 * @param data The JSON API response.
 * @return The parsed reply as a CarbonData object.
 */
CarbonData Utilities::fromByteArray(const QByteArray &data)
{
    if(data.isEmpty())
    {
        return CarbonData::error("Empty reply received.");
    }

    QJsonParseError parseError;

    QJsonDocument json = QJsonDocument::fromJson(data, &parseError);
    if(json.isNull())
    {
        QString errMsg("The supplied data(%1) seems not to be a valid JSON text. Error: %2(#%3) at %4.");
        errMsg = errMsg.arg(data, parseError.errorString()).arg(parseError.error).arg(parseError.offset);

        return CarbonData::error(errMsg);
    }

    if(json.isEmpty() || !json.isObject())
    {
        return CarbonData::error("Empty or invalid JSON received by API.");
    }

    QJsonObject reply = json.object();
    QMultiHash hash = Utilities::flatJsonHash(reply);

    // There is a chance that we have received an error response. In that case
    // there will be only one key named "error" containing a code and a message.
    if(reply.contains(QStringLiteral("error")))
    {
        return Utilities::fromApiError(hash);
    }

    // Now we can assume we have the actual data
    return Utilities::fromApiResponse(hash);
}

/**
 * @brief Creates a \c CarbonData reply from an API error response.
 *
 * Call this static method if your JSON response contains an error code sent by
 * the API. It will return a verbose error object.
 *
 * @param errorHash The flat JSON error code sent by the API.
 * @return The CarbonData object containing the error information.
 */
CarbonData Utilities::fromApiError(const QMultiHash<QString, QVariant> &errorHash)
{
    if(errorHash.isEmpty())
    {
        return CarbonData::error("An unspecified API error was received.");
    }

    QString errorCode = errorHash.value(QStringLiteral("code"), QStringLiteral("unknown")).toString();
    QString message = errorHash.value(QStringLiteral("message"), QStringLiteral("none")).toString();

    QString errMsg("The API returned an error: %1(%2).");
    errMsg = errMsg.arg(message, errorCode);

    return CarbonData::error(errMsg);
}

/**
 * @brief Creates a CarbonData object from the API reply.
 *
 * We need the \p replyHash to be a flattened \c QVariantHash. We extract the
 * forecast, from and to values and return the correct \c CarbonData object.
 *
 * \remark
 * If one of the dates (from, to) is not present we use the current date
 * and time.
 *
 * \sa flatJsonHash()
 *
 * \param replyHash The flattend reply hash with the API reply.
 * \return The CarbonData reply.
 */
/* static */
CarbonData Utilities::fromApiResponse(const QMultiHash<QString, QVariant> &replyHash)
{
    if(replyHash.isEmpty())
    {
        return CarbonData::error("Response data seems corrupt.");
    }

    QVariantList forecastList = replyHash.values(QStringLiteral("forecast"));

    QString fromStr = replyHash.value(QStringLiteral("from")).toString();
    QString toStr = replyHash.value(QStringLiteral("to")).toString();
    int forecastNow = -1;
    int forecastNext = -1;
    int forecastLater = -1;

    if(!forecastList.isEmpty())
    {
        forecastNow = forecastList.first().toInt();
    }

    if(forecastList.count() >= 2)
    {
        forecastNext = forecastList.at(1).toInt();
    }

    if(forecastList.count() >= 3)
    {
        forecastLater = forecastList.at(2).toInt();
    }

    const QString dateTimeFormat = Utilities::dateTimeFormat();
    QDateTime from = fromStr.isEmpty() ? QDateTime::currentDateTime() : QDateTime::fromString(fromStr, dateTimeFormat).toTimeSpec(Qt::LocalTime);
    QDateTime to   = toStr.isEmpty() ? QDateTime::currentDateTime() : QDateTime::fromString(toStr, dateTimeFormat).toTimeSpec(Qt::LocalTime);

    return CarbonData::ok(forecastNow, forecastNext, forecastLater, from, to);
}

/**
 * @brief Flattens a JSON object.
 *
 * This method creates a variant hash of key/value pairs. Objects are
 * "unwinded". This is used to make parsing API calls very simple, if for
 * instance the structure of a reply is not relevant.
 *
 * @param object The JSON object we want flattened.
 * @return A variant hash with all key value pairs as a QVariantHash object.
 */
/* static */
QMultiHash<QString, QVariant> Utilities::flatJsonHash(const QJsonObject &object)
{
    QList<QVariantMap> check;
    check << object.toVariantMap();

    QMultiHash<QString, QVariant> flatHash;

    for(int i = 0; i < check.count(); i++)
    {
        QVariantMap checkMap = check.at(i);

        const QStringList keys = checkMap.keys();

        for(const QString &key : keys)
        {
            QVariant value = checkMap.value(key);

            if(value.typeId() == QMetaType::QVariantMap)
            {
                check << value.toMap();
            }
            else if(value.typeId() == QMetaType::QVariantList)
            {
                const QVariantList vList = value.toList();
                for(const QVariant &vListValue : vList)
                {
                    if(vListValue.typeId() == QMetaType::QVariantMap) {
                        check << vListValue.toMap();
                    }
                }
            }
            else
            {
                flatHash.insert(key, value);
            }
        }
    }

    return flatHash;
}

/**
 * @brief Returns the date time format used by the National Grid API.
 *
 * @return The date time format string as a QString object.
 */
QString Utilities::dateTimeFormat()
{
    return QStringLiteral("yyyy-MM-ddThh:mmt");
}
