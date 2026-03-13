#pragma once

#include "IRegion.h"
#include <memory>

class IRegionAdapter
{
public:
    virtual ~IRegionAdapter() = default;
    virtual std::shared_ptr<IRegion> Initialize(QObject* regionTarget, const QString& regionName) = 0;
};
