#include "ModuleManager.h"
#include <QDebug>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonArray>

ModuleManager::ModuleManager(
    std::shared_ptr<IModuleCatalog> catalog,
    std::shared_ptr<IContainerExtension> container,
    QObject* parent)
    : IModuleManager(parent)
    , m_catalog(std::move(catalog))
    , m_container(std::move(container))
{
}

ModuleManager::~ModuleManager()
{
    for (auto* loader : m_loaders)
    {
        loader->unload();
        delete loader;
    }
    m_loaders.clear();
}

void ModuleManager::Run()
{
    m_catalog->Initialize();

    QList<ModuleInfo> whenAvailable;
    for (const auto& m : m_catalog->Modules())
    {
        if (m.initMode == InitializationMode::WhenAvailable)
        {
            whenAvailable.append(m);
        }
    }

    auto sorted = m_catalog->CompleteListWithDependencies(whenAvailable);

    for (auto& info : sorted)
    {
        InitializeModule(info);
    }
}

void ModuleManager::LoadModule(const QString& moduleName)
{
    if (m_loadedModules.contains(moduleName))
    {
        qDebug() << "[QtPrism] Module already loaded:" << moduleName;
        return;
    }

    auto& modules = m_catalog->Modules();
    for (auto& info : modules)
    {
        if (info.moduleName == moduleName)
        {
            for (const auto& dep : info.dependsOn)
            {
                if (!m_loadedModules.contains(dep))
                {
                    LoadModule(dep);
                }
            }
            InitializeModule(info);
            return;
        }
    }

    QString error = QString("Module not found in catalog: %1").arg(moduleName);
    qWarning() << "[QtPrism]" << error;
    emit moduleLoadFailed(moduleName, error);
}

IModule* ModuleManager::LoadPlugin(const QString& pluginPath)
{
    if (!QFileInfo::exists(pluginPath))
    {
        qWarning() << "[QtPrism] Plugin file not found:" << pluginPath;
        return nullptr;
    }

    auto* loader = new QPluginLoader(pluginPath, this);

    QJsonObject metaData = loader->metaData().value("MetaData").toObject();
    qDebug() << "[QtPrism] Plugin metadata:" << metaData;

    if (!loader->load())
    {
        qWarning() << "[QtPrism] Failed to load plugin:" << pluginPath
                    << "-" << loader->errorString();
        delete loader;
        return nullptr;
    }

    QObject* instance = loader->instance();
    if (!instance)
    {
        qWarning() << "[QtPrism] Plugin instance is null:" << pluginPath;
        loader->unload();
        delete loader;
        return nullptr;
    }

    IModule* module = qobject_cast<IModule*>(instance);
    if (!module)
    {
        qWarning() << "[QtPrism] Plugin does not implement IModule:" << pluginPath;
        loader->unload();
        delete loader;
        return nullptr;
    }

    QString moduleName = metaData.value("moduleName").toString();
    if (moduleName.isEmpty())
    {
        moduleName = QFileInfo(pluginPath).baseName();
    }

    m_loaders[moduleName] = loader;
    qDebug() << "[QtPrism] Plugin loaded successfully:" << pluginPath
             << "(module:" << moduleName << ")";

    return module;
}

void ModuleManager::InitializeModule(ModuleInfo& info)
{
    if (info.state == ModuleState::Initialized)
    {
        return;
    }

    info.state = ModuleState::ReadyForInitialization;
    qDebug() << "[QtPrism] === Loading module:" << info.moduleName << "===";

    IModule* module = nullptr;

    if (!info.ref.isEmpty())
    {
        module = LoadPlugin(info.ref);
    }

    if (!module)
    {
        QString error = QString("Could not load module plugin: %1 (path: %2)")
                            .arg(info.moduleName, info.ref);
        qWarning() << "[QtPrism]" << error;
        emit moduleLoadFailed(info.moduleName, error);
        return;
    }

    info.state = ModuleState::Initializing;

    qDebug() << "[QtPrism] Calling RegisterTypes for:" << info.moduleName;
    module->RegisterTypes(m_container.get());

    qDebug() << "[QtPrism] Calling OnInitialized for:" << info.moduleName;
    module->OnInitialized(m_container.get());

    info.state = ModuleState::Initialized;
    m_loadedModules[info.moduleName] = module;

    qDebug() << "[QtPrism] === Module initialized:" << info.moduleName << "===";
    emit moduleLoaded(info.moduleName);
}
