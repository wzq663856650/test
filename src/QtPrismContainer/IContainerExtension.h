#pragma once

#include "IContainerRegistry.h"
#include "IContainerProvider.h"

class IContainerExtension : public IContainerRegistry, public IContainerProvider
{
public:
    virtual ~IContainerExtension() = default;
    virtual void FinalizeExtension() = 0;
};
