#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QLocale>
#include <QQmlEngine>

class SettingsService;

class SettingsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QLocale::Country country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(QString regionId READ regionId WRITE setRegionId NOTIFY regionIdChanged)
    Q_PROPERTY(float lifetimeCarbon READ lifetimeCarbon WRITE setLifetimeCarbon NOTIFY lifetimeCarbonChanged)

public:
    explicit SettingsController(SettingsService *settings, QObject *parent = nullptr);

    QLocale::Country country() const;
    void setCountry(const QLocale::Country &newCountry);

    QString regionId() const;
    void setRegionId(const QString &newRegionId);

    float lifetimeCarbon() const;
    void setLifetimeCarbon(float newLifetimeCarbon);

signals:
    void countryChanged();
    void regionIdChanged();
    void lifetimeCarbonChanged();

private:
    struct SettingsControllerPrivate
    {
        SettingsService *settings;
    } d;
};

#endif // SETTINGSCONTROLLER_H
