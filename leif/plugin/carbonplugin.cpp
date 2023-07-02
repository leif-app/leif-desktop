#include <QDir>
#include <QPluginLoader>

#include "log/log.h"

#include "carbonplugin.h"

class CarbonPluginPrivate
{
public:
    ~CarbonPluginPrivate();

private:
    CarbonPluginPrivate(const QString &fileName);

    // DATA
    QScopedPointer<QPluginLoader> loader;
    IDataProvider *pluginInterface;
    Utils::CarbonPluginData pluginData;

    friend class CarbonPlugin;
};

CarbonPluginPrivate::CarbonPluginPrivate(const QString &fileName):
    loader {new QPluginLoader {fileName}},
    pluginInterface {nullptr}
{
    pluginData = Utils::CarbonPluginData::fromJson(loader->metaData().value(QStringLiteral("MetaData")));
}

CarbonPluginPrivate::~CarbonPluginPrivate()
{
    if(loader)
        loader->unload();

    pluginInterface = nullptr;
}

CarbonPlugin::CarbonPlugin(const QString &fileName):
    d {new CarbonPluginPrivate {fileName}}
{}

CarbonPlugin::~CarbonPlugin()
{}

#if 0
QString CarbonPlugin::errorString() const
{
    Q_ASSERT(d->loader != nullptr);

    return d->loader->errorString();
}

QString CarbonPlugin::fileName() const
{
    Q_ASSERT(d->loader != nullptr);

    return d->loader->fileName();
}
#endif

bool CarbonPlugin::isLoaded() const
{
    Q_ASSERT(d->loader != nullptr);

    return d->loader->isLoaded();
}

bool CarbonPlugin::load()
{
    Q_ASSERT(d->loader != nullptr);

    return d->loader->load();
}

bool CarbonPlugin::unload()
{
    Q_ASSERT(d->loader != nullptr);

    d->pluginInterface = nullptr;

    return d->loader->unload();
}

Utils::CarbonPluginData CarbonPlugin::pluginData() const
{
    return d->pluginData;
}

CarbonData CarbonPlugin::carbonPerKiloWatt(const QLocale::Country country, const QString &region)
{
    Q_ASSERT(d->loader != nullptr);

    if(!isLoaded())
        return CarbonData::error(tr("The plugin for the country: '%1' is not loade yet.")
                                 .arg(QLocale::countryToString(country)));

    if(d->pluginInterface == nullptr)
        d->pluginInterface = qobject_cast<IDataProvider*>(d->loader->instance());

    if(d->pluginInterface == nullptr)
        return CarbonData::error(tr("The plugin for the country: '%1' seems not "
                                    "to be a valid carbon data provider plugin.")
                                 .arg(QLocale::countryToString(country)));

    return d->pluginInterface->carbonPerKiloWatt(country, region);
}

QList<CarbonPlugin *> CarbonPlugin::getPlugins()
{
    QStringList pluginPaths;
    pluginPaths << QCoreApplication::applicationDirPath() + QStringLiteral("/plugins");

#ifdef Q_OS_MACOS
    // One is inside the package itself and the other is adjecent to the package.
    pluginPaths << QCoreApplication::applicationDirPath() + QStringLiteral("/../../../plugins");
#endif

    QStringList filePaths;

    auto getFilePaths {[&](const QString &pluginPath) {
        QDir dir{pluginPath};
        const QStringList &fileNames {dir.entryList(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot)};

        auto filePathTransform {[&](const QString &fileName) { return pluginPath + QStringLiteral("/") + fileName; }};
        std::transform(std::begin(fileNames), std::end(fileNames), std::back_inserter(filePaths), filePathTransform);
    }};
    std::for_each(std::begin(pluginPaths), std::end(pluginPaths), getFilePaths);


    QStringList libraryFilePaths {};
    auto isLibrary {[](const QString &filePath) { return QLibrary::isLibrary(filePath); }};
    std::copy_if(std::begin(filePaths), std::end(filePaths), std::back_inserter(libraryFilePaths), isLibrary);

    QList<CarbonPlugin*> plugins;

    auto makePlugin {[&](const QString &filePath) {
        DBG(QString("Loading plugin: '%1'.").arg(filePath));
        std::unique_ptr<CarbonPlugin> plugin = std::make_unique<CarbonPlugin>(filePath);

        QString pluginName = plugin->pluginData().name();

        struct HasName
        {
            const QString &_name;
            HasName(const QString &name): _name{name} {}
            bool operator()(const CarbonPlugin *p) const {return p->pluginData().name() == _name; }
        };

        if(std::none_of(std::begin(plugins), std::end(plugins), HasName(pluginName)))
            plugins << plugin.release();
    }};

    std::for_each(std::begin(libraryFilePaths), std::end(libraryFilePaths), makePlugin);

    return plugins;
}
