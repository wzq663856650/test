#pragma once

#include "ModuleInfo.h"
#include <QList>

class IModuleCatalog
{
public:
    virtual ~IModuleCatalog() = default;

    virtual QList<ModuleInfo>& Modules() = 0;
    virtual void AddModule(const ModuleInfo& moduleInfo) = 0;
    virtual QList<ModuleInfo> CompleteListWithDependencies(const QList<ModuleInfo>& modules) const = 0;
    virtual void Initialize() = 0;
};
