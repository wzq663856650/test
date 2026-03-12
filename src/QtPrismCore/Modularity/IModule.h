#pragma once

#include <QtPlugin>

class IContainerRegistry;
class IContainerProvider;

class IModule
{
public:
    virtual ~IModule() = default;
    virtual void RegisterTypes(IContainerRegistry* containerRegistry) = 0;
    virtual void OnInitialized(IContainerProvider* containerProvider) = 0;
};

#define QtPrism_IModule_iid "org.qtprism.IModule/1.0"
Q_DECLARE_INTERFACE(IModule, QtPrism_IModule_iid)
