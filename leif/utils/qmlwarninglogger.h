#ifndef QMLWARNINGLOGGER_H
#define QMLWARNINGLOGGER_H

#include <QQmlError>
#include <QObject>

class QQmlEngine;

namespace Utils {
class QmlWarningLogger : public QObject
{
    Q_OBJECT
public:
    explicit QmlWarningLogger(QQmlEngine *engineToConnectTo, QObject *parent = nullptr);
    virtual ~QmlWarningLogger() = default;

public slots:
    void onWarnings(const QList<QQmlError> &warnings);
};
}

#endif // QMLWARNINGLOGGER_H
