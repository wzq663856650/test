#include "RegionManager.h"
#include <QDebug>

RegionManager::RegionManager(QObject* parent)
    : IRegionManager(parent)
{
}

void RegionManager::AddRegion(const QString& regionName, std::shared_ptr<IRegion> region)
{
    if (m_regions.contains(regionName))
    {
        qWarning() << "[QtPrism] Region already exists:" << regionName;
        return;
    }

    m_regions[regionName] = std::move(region);
    qDebug() << "[QtPrism] Region added:" << regionName;
    emit regionAdded(regionName);
}

void RegionManager::RemoveRegion(const QString& regionName)
{
    if (m_regions.remove(regionName) > 0)
    {
        emit regionRemoved(regionName);
    }
}

std::shared_ptr<IRegion> RegionManager::GetRegion(const QString& regionName) const
{
    return m_regions.value(regionName);
}

bool RegionManager::ContainsRegion(const QString& regionName) const
{
    return m_regions.contains(regionName);
}

QStringList RegionManager::RegionNames() const
{
    return m_regions.keys();
}

void RegionManager::RegisterViewWithRegion(const QString& regionName, const QString& viewName)
{
    m_viewRegistrations[regionName].append(viewName);
    qDebug() << "[QtPrism] View" << viewName << "registered with region" << regionName;
}

QStringList RegionManager::GetViewsForRegion(const QString& regionName) const
{
    return m_viewRegistrations.value(regionName);
}

void RegionManager::RequestNavigate(
    const QString& regionName,
    const QString& viewName,
    const QVariantMap& parameters)
{
    auto region = GetRegion(regionName);
    if (!region)
    {
        qWarning() << "[QtPrism] Region not found:" << regionName;
        return;
    }

    region->RequestNavigate(viewName, parameters, [regionName, viewName](bool success)
    {
        if (success)
        {
            qDebug() << "[QtPrism] Navigation to" << viewName << "in" << regionName << "succeeded";
        }
        else
        {
            qWarning() << "[QtPrism] Navigation to" << viewName << "in" << regionName << "failed";
        }
    });
}
