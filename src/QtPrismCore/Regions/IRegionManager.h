#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <memory>

class IRegion;

class IRegionManager : public QObject
{
    Q_OBJECT

public:
    explicit IRegionManager(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IRegionManager() = default;

    virtual void AddRegion(const QString& regionName, std::shared_ptr<IRegion> region) = 0;
    virtual void RemoveRegion(const QString& regionName) = 0;
    virtual std::shared_ptr<IRegion> GetRegion(const QString& regionName) const = 0;
    virtual bool ContainsRegion(const QString& regionName) const = 0;
    virtual QStringList RegionNames() const = 0;

    virtual void RegisterViewWithRegion(const QString& regionName, const QString& viewName) = 0;
    virtual QStringList GetViewsForRegion(const QString& regionName) const = 0;

    Q_INVOKABLE virtual void RequestNavigate(
        const QString& regionName,
        const QString& viewName,
        const QVariantMap& parameters = {}) = 0;

signals:
    void regionAdded(const QString& regionName);
    void regionRemoved(const QString& regionName);
};
