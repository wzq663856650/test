#include "ModuleManager.h"
#include <QDebug>
#include <QFileInfo>

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
    qDeleteAll(m_loaders);
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
        qDebug() << "Module already loaded:" << moduleName;
        return;
    }

    auto& modules = m_catalog->Modules();
    for (auto& info : modules)
    {
        if (info.moduleName == moduleName)
        {
            // Load dependencies first
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

    emit moduleLoadFailed(moduleName, "Module not found in catalog");
}

void ModuleManager::InitializeModule(ModuleInfo& info)
{
    if (info.state == ModuleState::Initialized)
    {
        return;
    }

    info.state = ModuleState::ReadyForInitialization;
    qDebug() << "[QtPrism] Loading module:" << info.moduleName;

    IModule* module = nullptr;

    if (!info.ref.isEmpty() && QFileInfo::exists(info.ref))
    {
        auto* loader = new QPluginLoader(info.ref, this);
        if (loader->load())
        {
            QObject* instance = loader->instance();
            module = qobject_cast<IModule*>(instance);
            if (module)
            {
                m_loaders[info.moduleName] = loader;
            }
            else
            {
                qWarning() << "[QtPrism] Plugin does not implement IModule:" << info.ref;
                delete loader;
            }
        }
        else
        {
            qWarning() << "[QtPrism] Failed to load plugin:" << loader->errorString();
            delete loader;
        }
    }

    if (!module)
    {
        // Try to resolve from container (for statically linked modules)
        auto resolved = m_container->ResolveNamedImpl(
            std::type_index(typeid(IModule)), info.moduleName);
        if (resolved)
        {
            module = static_cast<IModule*>(resolved.get());
        }
    }

    if (!module)
    {
        QString error = QString("Could not load module: %1").arg(info.moduleName);
        qWarning() << "[QtPrism]" << error;
        emit moduleLoadFailed(info.moduleName, error);
        return;
    }

    info.state = ModuleState::Initializing;

    module->RegisterTypes(m_container.get());
    module->OnInitialized(m_container.get());

    info.state = ModuleState::Initialized;
    m_loadedModules[info.moduleName] = module;

    qDebug() << "[QtPrism] Module initialized:" << info.moduleName;
    emit moduleLoaded(info.moduleName);
}
