#pragma once

#include "Modularity/IModuleCatalog.h"
#include <QList>
#include <QMap>

class ModuleCatalog : public IModuleCatalog
{
public:
    ModuleCatalog() = default;
    ~ModuleCatalog() override = default;

    QList<ModuleInfo>& Modules() override;
    void AddModule(const ModuleInfo& moduleInfo) override;
    QList<ModuleInfo> CompleteListWithDependencies(const QList<ModuleInfo>& modules) const override;
    void Initialize() override;

    void AddModule(const QString& name, const QString& ref,
                   InitializationMode mode = InitializationMode::WhenAvailable,
                   const QStringList& deps = {});

private:
    void Validate() const;
    QList<ModuleInfo> TopologicalSort(const QList<ModuleInfo>& modules) const;

    QList<ModuleInfo> m_modules;
};
