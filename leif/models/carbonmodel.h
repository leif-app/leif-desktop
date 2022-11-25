/**
 * \brief Defines the CarbonModel class.
 *
 * The CarbonModel class is a QML usable wrapper around the CarbonProcessor among others.
 * It informs about changes in the lifetime carbon counter and the session carbon counter.
 * Also it informs about the current carbon usage "intensity".
 *
 * \author Dariusz Scharsig
 *
 * \copyright Tim Stone
 *
 * \date 17.09.2022
 */
#ifndef CARBONMODEL_H
#define CARBONMODEL_H

#include <QObject>
#include <QQmlEngine>

#include "carbonprocessor.h"

class CarbonModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float sessionCarbon READ sessionCarbon NOTIFY sessionCarbonChanged)
    Q_PROPERTY(float lifetimeCarbon READ lifetimeCarbon NOTIFY lifetimeCarbonChanged)
    Q_PROPERTY(CarbonProcessor::CarbonUsageLevel carbonUsageLevel READ carbonUsageLevel NOTIFY carbonUsageLevelChanged)
    QML_ELEMENT;

public:
    explicit CarbonModel(QObject *parent = nullptr);

    float sessionCarbon() const;
    float lifetimeCarbon() const;
    CarbonProcessor::CarbonUsageLevel carbonUsageLevel() const;

public slots:
    void clearStats();

signals:
    void sessionCarbonChanged();
    void lifetimeCarbonChanged();
    void carbonUsageLevelChanged();
};

#endif // CARBONMODEL_H
