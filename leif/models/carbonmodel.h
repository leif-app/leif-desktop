#ifndef CARBONMODEL_H
#define CARBONMODEL_H

#include <QObject>
#include <QQmlEngine>

class CarbonModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sessionCarbon READ sessionCarbon NOTIFY sessionCarbonChanged)
    Q_PROPERTY(int lifetimeCarbon READ lifetimeCarbon NOTIFY lifetimeCarbonChanged)
    QML_ELEMENT;

public:
    explicit CarbonModel(QObject *parent = nullptr);

    int sessionCarbon() const;
    int lifetimeCarbon() const;

public slots:
    void clearStats();

signals:
    void sessionCarbonChanged();
    void lifetimeCarbonChanged();
};

#endif // CARBONMODEL_H
