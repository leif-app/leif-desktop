#ifndef CARBONPLUGIN_H
#define CARBONPLUGIN_H

#include <QCoreApplication>

#include <interfaces/IDataProvider.h>

#include "utils/carbonplugindata.h"

class CarbonPluginPrivate;

class CarbonPlugin : public IDataProvider
{
    Q_DECLARE_TR_FUNCTIONS(CarbonPlugin)
public:
    CarbonPlugin(const QString &fileName);
    virtual ~CarbonPlugin();

    //QString errorString() const;
    //QString fileName() const;
    bool isLoaded() const;
    bool load();
    bool unload();
    Utils::CarbonPluginData pluginData() const;

    virtual CarbonData carbonPerKiloWatt(const QLocale::Country country, const QString &region) override;

    static QList<CarbonPlugin*> getPlugins();

private:
    Q_DISABLE_COPY_MOVE(CarbonPlugin)
    QScopedPointer<CarbonPluginPrivate> d;
};

#endif // CARBONPLUGIN_H
