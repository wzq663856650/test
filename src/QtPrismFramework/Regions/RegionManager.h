#pragma once

#include "Regions/IRegionManager.h"
#include "Regions/IRegion.h"

#include <QMap>
#include <memory>

class RegionManager : public IRegionManager
{
    Q_OBJECT

public:
    explicit RegionManager(QObject* parent = nullptr);
    ~RegionManager() override = default;

    void AddRegion(const QString& regionName, std::shared_ptr<IRegion> region) override;
    void RemoveRegion(const QString& regionName) override;
    std::shared_ptr<IRegion> GetRegion(const QString& regionName) const override;
    bool ContainsRegion(const QString& regionName) const override;
    QStringList RegionNames() const override;

    void RegisterViewWithRegion(const QString& regionName, const QString& viewName) override;
    QStringList GetViewsForRegion(const QString& regionName) const override;

    Q_INVOKABLE void RequestNavigate(
        const QString& regionName,
        const QString& viewName,
        const QVariantMap& parameters = {}) override;

private:
    QMap<QString, std::shared_ptr<IRegion>> m_regions;
    QMap<QString, QStringList> m_viewRegistrations;
};
