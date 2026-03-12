#include "ModuleCatalog.h"
#include <QSet>
#include <QQueue>
#include <stdexcept>

QList<ModuleInfo>& ModuleCatalog::Modules()
{
    return m_modules;
}

void ModuleCatalog::AddModule(const ModuleInfo& moduleInfo)
{
    m_modules.append(moduleInfo);
}

void ModuleCatalog::AddModule(const QString& name, const QString& ref,
                              InitializationMode mode, const QStringList& deps)
{
    ModuleInfo info;
    info.moduleName = name;
    info.ref = ref;
    info.initMode = mode;
    info.dependsOn = deps;
    info.state = ModuleState::NotStarted;
    m_modules.append(info);
}

void ModuleCatalog::Initialize()
{
    Validate();
}

void ModuleCatalog::Validate() const
{
    QSet<QString> names;
    for (const auto& m : m_modules)
    {
        if (names.contains(m.moduleName))
        {
            throw std::runtime_error(
                QString("Duplicate module name: %1").arg(m.moduleName).toStdString());
        }
        names.insert(m.moduleName);
    }

    for (const auto& m : m_modules)
    {
        for (const auto& dep : m.dependsOn)
        {
            if (!names.contains(dep))
            {
                throw std::runtime_error(
                    QString("Module '%1' depends on unknown module '%2'")
                        .arg(m.moduleName, dep).toStdString());
            }
        }
    }
}

QList<ModuleInfo> ModuleCatalog::CompleteListWithDependencies(
    const QList<ModuleInfo>& modules) const
{
    return TopologicalSort(modules);
}

QList<ModuleInfo> ModuleCatalog::TopologicalSort(const QList<ModuleInfo>& modules) const
{
    QMap<QString, ModuleInfo> nameMap;
    QMap<QString, int> inDegree;
    QMap<QString, QStringList> adjacency;

    for (const auto& m : modules)
    {
        nameMap[m.moduleName] = m;
        inDegree[m.moduleName] = 0;
    }

    for (const auto& m : modules)
    {
        for (const auto& dep : m.dependsOn)
        {
            if (nameMap.contains(dep))
            {
                adjacency[dep].append(m.moduleName);
                inDegree[m.moduleName]++;
            }
        }
    }

    QQueue<QString> queue;
    for (auto it = inDegree.begin(); it != inDegree.end(); ++it)
    {
        if (it.value() == 0)
        {
            queue.enqueue(it.key());
        }
    }

    QList<ModuleInfo> sorted;
    while (!queue.isEmpty())
    {
        auto current = queue.dequeue();
        sorted.append(nameMap[current]);

        for (const auto& neighbor : adjacency[current])
        {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0)
            {
                queue.enqueue(neighbor);
            }
        }
    }

    if (sorted.size() != modules.size())
    {
        throw std::runtime_error("Circular dependency detected among modules");
    }

    return sorted;
}
