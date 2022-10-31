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

    int sessionCarbon() const;
    int lifetimeCarbon() const;

public slots:
    void clearStats();

signals:
    void sessionCarbonChanged();
    void lifetimeCarbonChanged();

private slots:
    void calculateCarbon();

private:
    void setSessionCarbon(int newSessionCarbon);
    void setLifetimeCarbon(int newLifetimeCarbon);

private:
    Q_DISABLE_COPY_MOVE(CarbonProcessor);
    CarbonProcessorPrivate *d;
};

#endif // CARBONPROCESSOR_H
