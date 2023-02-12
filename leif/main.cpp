/**
 * @brief Defines all main application methods.
 *
 * This file contains the main() method and defined all other helper methods
 * that are used to boot up the application.
 *
 * @author Dariusz Scharsi
 *
 * @date 17.09.2022
 */
#include <QApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QScopedPointer>

#include "main.h"
#include "trayicon.h"
#include "models/trayiconmodel.h"

#include "carbonprocessor.h"
#include "leifsettings.h"

#include "log/log.h"
#include "log/predictivelogger.h"

#ifdef QT_DEBUG
#include "log/consolelogger.h"
//#include "log/logfilterbyfile.h"
#endif // ! QT_DEBUG

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false); // Very important with tray apps.
    setApplicationInfo();

#ifdef _WIN32
    setStyleSheet();
#endif

    // Initialize Log System
    Log::LogSystem::logManager()->registerLogger(new Log::PredictiveLogger);

#ifdef QT_DEBUG
    Log::LogSystem::logManager()->registerLogger(new Log::ConsoleLogger);
#endif

    INF("===============================");
    INF("Leif application is starting...");
    INF("===============================");

    /*
    QScopedPointer<QTranslator> translator(new QTranslator);
    loadTranslations(translator.data());
    */

    QScopedPointer<TrayIconModel> trayModel(new TrayIconModel);

    QScopedPointer<TrayIcon> tray(new TrayIcon(trayModel.data()));
    tray->show();

    int result = app.exec();

    INF("==================================");
    INF("Leif application is shutting down.");
    INF("==================================");

    cleanup();

    return result;
}

void setApplicationInfo()
{
    QString ver = QStringLiteral("%1.%2.%3.%4");
    ver = ver.arg(QM_MAJOR_VERSION).arg(QM_MINOR_VERSION).arg(QM_PATCH_VERSION).arg(QM_BUILD_VERSION);

    QCoreApplication::setApplicationVersion(ver);
    QCoreApplication::setOrganizationName(QStringLiteral("leif"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("leif.support"));
    QCoreApplication::setApplicationName(QStringLiteral("leif carbon tracker"));
}

void loadTranslations(QTranslator *translator)
{
    Q_ASSERT(translator != nullptr);

    if(translator->load(QLocale(), QStringLiteral("leif"), QStringLiteral("_"), QStringLiteral(":/i18n")))
    {
        QCoreApplication::installTranslator(translator);
    }
}

#ifdef _WIN32
void setStyleSheet()
{
    qApp->setStyleSheet("QMenu {"
                        "background-color: #2B2B2B;"
                        "margin: 2px;"
                        "}"
                        "QMenu::separator {"
                        "background: gray;"
                        "height: 1px;"
                        "}"
                        "QMenu::item {"
                        "color: #f8f8f8;"
                        "padding-top: 10px;"
                        "padding-bottom: 10px;"
                        "padding-left: 30px;"
                        "padding-right: 30px"
                        "}"
                        "QMenu::item:selected {"
                        "background: #3B3B3B;"
                        "}"
                        "QMenu::item:selected:disabled {"
                        "background: #2B2B2B;"
                        "}"
                        "QMenu::item::disabled {"
                        "color: #a8a8a8;"
                        "}"
                        );
}
#endif

void cleanup()
{
    CarbonProcessor::Destroy();
    LeifSettings::Destroy();
    Log::LogSystem::Destroy();
}
