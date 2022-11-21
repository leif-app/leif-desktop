#ifndef CARBONPROCESSOR_H
#define CARBONPROCESSOR_H

#include <QObject>

class CarbonProcessorPrivate;

class CarbonProcessor : public QObject
{
    Q_OBJECT
    explicit CarbonProcessor(QObject *parent = nullptr);
    virtual ~CarbonProcessor();

public:
    static CarbonProcessor *Instance();
    static void Destroy();

    enum CarbonUsageLevel { VeryLow, Low, Medium, High, VeryHigh };
    Q_ENUM(CarbonUsageLevel)

    int sessionCarbon() const;
    int lifetimeCarbon() const;
    CarbonUsageLevel carbonUsageLevel() const;

public slots:
    void clearStats();

signals:
    void sessionCarbonChanged();
    void lifetimeCarbonChanged();
    void carbonUsageLevelChanged();

private slots:
    void calculateCarbon();
    void calculateUsageLevel(int co2PerkWh);

private:
    void setSessionCarbon(int newSessionCarbon);
    void setLifetimeCarbon(int newLifetimeCarbon);
    void setCarbonUsageLevel(CarbonUsageLevel newLevel);

private:
    Q_DISABLE_COPY_MOVE(CarbonProcessor);
    CarbonProcessorPrivate *d;
};

#endif // CARBONPROCESSOR_H
