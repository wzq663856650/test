#pragma once

#include "Modularity/IModuleManager.h"
#include "Modularity/IModuleCatalog.h"
#include "Modularity/IModule.h"
#include "IContainerExtension.h"

#include <QMap>
#include <QPluginLoader>
#include <memory>

class ModuleManager : public IModuleManager
{
    Q_OBJECT

public:
    ModuleManager(std::shared_ptr<IModuleCatalog> catalog,
                  std::shared_ptr<IContainerExtension> container,
                  QObject* parent = nullptr);
    ~ModuleManager() override;

    void Run() override;
    void LoadModule(const QString& moduleName) override;

private:
    void InitializeModule(ModuleInfo& info);
    IModule* LoadPlugin(const QString& pluginPath);

    std::shared_ptr<IModuleCatalog> m_catalog;
    std::shared_ptr<IContainerExtension> m_container;
    QMap<QString, QPluginLoader*> m_loaders;
    QMap<QString, IModule*> m_loadedModules;
};
