#pragma once

#include "Modularity/IModule.h"
#include "IContainerRegistry.h"
#include "IContainerProvider.h"

#include <QObject>

class DashboardModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)
    Q_PLUGIN_METADATA(IID QtPrism_IModule_iid FILE "dashboard_module.json")

public:
    explicit DashboardModule(QObject* parent = nullptr) : QObject(parent) {}

    void RegisterTypes(IContainerRegistry* containerRegistry) override;
    void OnInitialized(IContainerProvider* containerProvider) override;
};
