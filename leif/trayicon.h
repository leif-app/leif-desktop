#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

#include <include/carbonusagelevel.h>

class TrayIconController;
class TrayIconPrivate;

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(TrayIconController *model, QObject *parent = nullptr);
    explicit TrayIcon(TrayIconController *controller, const QIcon &icon, QObject *parent = nullptr);
    virtual ~TrayIcon();

private slots:
    void onCarbonUsageLevelChanged(CarbonUsageLevel newCarbonUsageLevel);
    void onResetStatsClicked();
    void doCheckConfigured();

private:
    void setupMenu();
    void connectModel();

private:
    QScopedPointer<TrayIconPrivate> d;
};

#endif // TRAYICON_H
