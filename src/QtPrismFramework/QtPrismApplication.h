#pragma once

#include "IContainerExtension.h"
#include "Modularity/IModuleCatalog.h"
#include "Modularity/IModuleManager.h"
#include "Regions/IRegionManager.h"
#include "Regions/IViewRegistry.h"
#include "Regions/RegionAdapterMappings.h"

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <memory>

class QtPrismApplication : public QObject
{
    Q_OBJECT

public:
    explicit QtPrismApplication(QGuiApplication* app, QQmlApplicationEngine* engine,
                                QObject* parent = nullptr);
    ~QtPrismApplication() override = default;

    void Initialize();

    std::shared_ptr<IContainerExtension> Container() const { return m_container; }
    QQmlApplicationEngine* Engine() const { return m_engine; }

protected:
    virtual std::shared_ptr<IContainerExtension> CreateContainerExtension();
    virtual std::shared_ptr<IModuleCatalog> CreateModuleCatalog();
    virtual void RegisterRequiredTypes(IContainerRegistry* registry);
    virtual void RegisterTypes(IContainerRegistry* registry) = 0;
    virtual void ConfigureModuleCatalog(IModuleCatalog* catalog) = 0;
    virtual QUrl CreateShell() = 0;
    virtual void OnInitialized();

private:
    void InitializeModules();
    void RegisterFrameworkQmlTypes();

    QGuiApplication* m_app;
    QQmlApplicationEngine* m_engine;
    std::shared_ptr<IContainerExtension> m_container;
    std::shared_ptr<IModuleCatalog> m_catalog;
};
