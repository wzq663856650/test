#pragma once

#include "Modularity/IModule.h"
#include "IContainerRegistry.h"
#include "IContainerProvider.h"

class OrderModule : public IModule
{
public:
    void RegisterTypes(IContainerRegistry* containerRegistry) override;
    void OnInitialized(IContainerProvider* containerProvider) override;
};
