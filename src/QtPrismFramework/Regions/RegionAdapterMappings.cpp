#include "RegionAdapterMappings.h"
#include <QDebug>

void RegionAdapterMappings::RegisterMapping(
    const QString& controlType,
    std::shared_ptr<IRegionAdapter> adapter)
{
    m_mappings[controlType] = std::move(adapter);
}

std::shared_ptr<IRegionAdapter> RegionAdapterMappings::GetAdapter(
    const QString& controlType) const
{
    auto it = m_mappings.find(controlType);
    if (it != m_mappings.end())
    {
        return it.value();
    }
    qWarning() << "[QtPrism] No adapter registered for:" << controlType;
    return nullptr;
}
