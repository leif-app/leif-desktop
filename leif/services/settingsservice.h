#ifndef SETTINGSSERVICE_H
#define SETTINGSSERVICE_H

#include <QLocale>
#include <QObject>

class SettingsService : public QObject
{
    Q_OBJECT
public:
    explicit SettingsService(QObject *parent = nullptr);
    virtual ~SettingsService() = default;

    void saveLocation(const QLocale::Country &country, const QString &regionId);
    void saveCountry(const QLocale::Country &country);
    void saveRegionId(const QString &regionId);
    void saveAverageDischargeRate(int averageDischargeRate);

    QLocale::Country country() const;
    QString regionId() const;
    int averageDischargeRate() const;

    void saveLifetimeCarbon(float lifeTime);
    float lifeTimeCarbon() const;
    void clearLifeTimeCarbon();

signals:
    void countryChanged(const QLocale::Country country);
    void regionIdChanged(const QString &regionId);
    void lifeTimeCarbonChanged(float lifeTime);
    void averageDischargeRateChanged(int averageDischargeRate);

private:
    Q_DISABLE_COPY_MOVE(SettingsService);
};

#endif // SETTINGSSERVICE_H
