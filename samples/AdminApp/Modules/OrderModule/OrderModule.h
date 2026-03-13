#pragma once

#include "Modularity/IModule.h"
#include "IContainerRegistry.h"
#include "IContainerProvider.h"

#include <QObject>

class OrderModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)
    Q_PLUGIN_METADATA(IID QtPrism_IModule_iid FILE "order_module.json")

public:
    explicit OrderModule(QObject* parent = nullptr) : QObject(parent) {}

    void RegisterTypes(IContainerRegistry* containerRegistry) override;
    void OnInitialized(IContainerProvider* containerProvider) override;
};
