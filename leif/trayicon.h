#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

#include "models/trayiconmodel.h"

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(TrayIconModel *model, QObject *parent = nullptr);
    explicit TrayIcon(const QIcon &icon, QObject *parent = nullptr);
    virtual ~TrayIcon() = default;

private slots:
    void onSessionCarbonChanged();
    void onTotalCarbonChanged();
    void onCarbonUsageLevelChanged();
    void onChargeForecastChanged();
    void onResetStatsClicked();
    void onPreferencesClicked();
    void onConfiguredChanged();
    void doCheckConfigured();
    void doIconContrastModeToggled();

private:
    void setupMenu();
    void connectModel();

    static QString sessionCarbonLabel(double value);
    static QString totalCarbonLabel(double value);
    static QString carbonLabel(const QString &labelText, double value);
    static QString intensityLabel(CarbonProcessor::CarbonUsageLevel usageLevel);
    static QString chargeForecastLabel(CarbonProcessor::ChargeForecast chargeForecast);

    static QString co2Unit();
    static QString usageLevelToString(CarbonProcessor::CarbonUsageLevel usageLevel);
    static QString usageLevelToIconName(CarbonProcessor::CarbonUsageLevel usageLevel, bool contrastMode = false);

    static bool iconContrastMode();
    static void saveIconContrastMode(bool iconContrastMode);

private:
    struct TrayIconData
    {
        TrayIconModel *model;
        QAction *notConfiguredAction;
        QAction *sessionCarbonAction;
        QAction *totalCarbonAction;
        QAction *carbonUsageLevelAction;
        QAction *chargeForecastAction;
        QAction *iconContrastMode;
    } d;
};

#endif // TRAYICON_H
