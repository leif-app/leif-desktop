#include <QQmlEngine>

#include "qmlwarninglogger.h"

#include "log/log.h"

using namespace Utils;

QmlWarningLogger::QmlWarningLogger(QQmlEngine *engineToConnectTo,
                                   QObject *parent /* = nullptr */)
    : QObject{parent == nullptr ? engineToConnectTo : parent}
{
    if(engineToConnectTo != nullptr)
    {
        connect(engineToConnectTo, &QQmlEngine::warnings, this, &QmlWarningLogger::onWarnings);
    }
}

void QmlWarningLogger::onWarnings(const QList<QQmlError> &warnings)
{
    for(const QQmlError &error : warnings)
    {
        if(error.isValid())
        {
            WRN(error.toString());
        }
    }
}
