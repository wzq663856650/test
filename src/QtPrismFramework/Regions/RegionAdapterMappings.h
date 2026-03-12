#pragma once

#include "Regions/IRegionAdapter.h"
#include <QMap>
#include <QString>
#include <memory>

class RegionAdapterMappings
{
public:
    RegionAdapterMappings() = default;
    ~RegionAdapterMappings() = default;

    void RegisterMapping(const QString& controlType, std::shared_ptr<IRegionAdapter> adapter);
    std::shared_ptr<IRegionAdapter> GetAdapter(const QString& controlType) const;

private:
    QMap<QString, std::shared_ptr<IRegionAdapter>> m_mappings;
};
