#include "ShellController.h"
#include <QDebug>

ShellController::ShellController(
    std::shared_ptr<IViewRegistry> viewRegistry,
    std::shared_ptr<IRegionManager> regionManager,
    std::shared_ptr<IModuleManager> moduleManager,
    QObject* parent)
    : QObject(parent)
    , m_viewRegistry(std::move(viewRegistry))
    , m_regionManager(std::move(regionManager))
    , m_moduleManager(std::move(moduleManager))
{
    connect(m_moduleManager.get(), &IModuleManager::moduleLoaded,
            this, &ShellController::refreshNavigation);
}

QUrl ShellController::currentViewUrl() const
{
    if (m_currentViewName.isEmpty() || !m_viewRegistry)
    {
        return QUrl();
    }

    if (!m_viewRegistry->HasView(m_currentViewName))
    {
        return QUrl();
    }

    return m_viewRegistry->GetViewUrl(m_currentViewName);
}

QString ShellController::currentViewName() const
{
    return m_currentViewName;
}

QVariantList ShellController::navigationItems() const
{
    QVariantList list;
    for (const auto& item : m_navItems)
    {
        bool available = m_viewRegistry && m_viewRegistry->HasView(item.viewId);
        QVariantMap map;
        map["name"] = item.name;
        map["viewId"] = item.viewId;
        map["icon"] = item.icon;
        map["moduleName"] = item.moduleName;
        map["isOnDemand"] = item.isOnDemand;
        map["available"] = available;
        list.append(map);
    }
    return list;
}

int ShellController::loadedModuleCount() const
{
    return m_loadedModuleCount;
}

void ShellController::navigateTo(const QString& viewId)
{
    qDebug() << "[ShellController] navigateTo:" << viewId;

    // Find the nav item
    for (const auto& item : m_navItems)
    {
        if (item.viewId == viewId && item.isOnDemand)
        {
            if (m_viewRegistry && !m_viewRegistry->HasView(viewId))
            {
                qDebug() << "[ShellController] Loading on-demand module:" << item.moduleName;
                if (m_moduleManager)
                {
                    m_moduleManager->LoadModule(item.moduleName);
                }
            }
            break;
        }
    }

    if (!m_viewRegistry || !m_viewRegistry->HasView(viewId))
    {
        qWarning() << "[ShellController] View not available:" << viewId;
        return;
    }

    m_currentViewName = viewId;
    emit currentViewUrlChanged();

    if (m_regionManager)
    {
        m_regionManager->RequestNavigate("MainRegion", viewId);
    }
}

bool ShellController::isViewAvailable(const QString& viewName) const
{
    return m_viewRegistry && m_viewRegistry->HasView(viewName);
}

void ShellController::addNavigationItem(
    const QString& name, const QString& viewId,
    const QString& icon, const QString& moduleName,
    bool isOnDemand)
{
    m_navItems.append({name, viewId, icon, moduleName, isOnDemand});
}

void ShellController::refreshNavigation()
{
    m_loadedModuleCount++;
    qDebug() << "[ShellController] Navigation refreshed, loaded modules:" << m_loadedModuleCount;
    emit navigationItemsChanged();

    // Auto-navigate to the first available view if nothing is currently shown
    if (m_currentViewName.isEmpty() || !isViewAvailable(m_currentViewName))
    {
        for (const auto& item : m_navItems)
        {
            if (m_viewRegistry && m_viewRegistry->HasView(item.viewId))
            {
                navigateTo(item.viewId);
                break;
            }
        }
    }
}
