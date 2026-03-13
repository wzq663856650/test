#pragma once

#include "Regions/IViewRegistry.h"
#include "Regions/IRegionManager.h"
#include "Modularity/IModuleManager.h"

#include <QObject>
#include <QUrl>
#include <QVariantList>
#include <memory>

class ShellController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl currentViewUrl READ currentViewUrl NOTIFY currentViewUrlChanged)
    Q_PROPERTY(QString currentViewName READ currentViewName NOTIFY currentViewUrlChanged)
    Q_PROPERTY(QVariantList navigationItems READ navigationItems NOTIFY navigationItemsChanged)
    Q_PROPERTY(int loadedModuleCount READ loadedModuleCount NOTIFY navigationItemsChanged)

public:
    struct NavItem
    {
        QString name;
        QString viewId;
        QString icon;
        QString moduleName;
        bool isOnDemand;
    };

    explicit ShellController(
        std::shared_ptr<IViewRegistry> viewRegistry,
        std::shared_ptr<IRegionManager> regionManager,
        std::shared_ptr<IModuleManager> moduleManager,
        QObject* parent = nullptr);

    QUrl currentViewUrl() const;
    QString currentViewName() const;
    QVariantList navigationItems() const;
    int loadedModuleCount() const;

    Q_INVOKABLE void navigateTo(const QString& viewId);
    Q_INVOKABLE bool isViewAvailable(const QString& viewName) const;

    void addNavigationItem(const QString& name, const QString& viewId,
                           const QString& icon, const QString& moduleName,
                           bool isOnDemand = false);

    void refreshNavigation();

signals:
    void currentViewUrlChanged();
    void navigationItemsChanged();

private:
    std::shared_ptr<IViewRegistry> m_viewRegistry;
    std::shared_ptr<IRegionManager> m_regionManager;
    std::shared_ptr<IModuleManager> m_moduleManager;
    QList<NavItem> m_navItems;
    QString m_currentViewName;
    int m_loadedModuleCount = 0;
};
