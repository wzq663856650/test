#include "QtPrismApplication.h"
#include "QtPrismContainerImpl.h"
#include "ContainerLocator.h"
#include "EventAggregator.h"
#include "Modularity/ModuleCatalog.h"
#include "Modularity/ModuleManager.h"
#include "Regions/RegionManager.h"
#include "Navigation/NavigationJournal.h"
#include "Views/ViewRegistry.h"

#include <QDebug>
#include <QQmlContext>

QtPrismApplication::QtPrismApplication(
    QGuiApplication* app,
    QQmlApplicationEngine* engine,
    QObject* parent)
    : QObject(parent)
    , m_app(app)
    , m_engine(engine)
{
}

void QtPrismApplication::Initialize()
{
    qDebug() << "[QtPrism] ========== Initialization Started ==========";

    m_container = CreateContainerExtension();
    ContainerLocator::SetContainerExtension(m_container);
    qDebug() << "[QtPrism] Step 1: Container created";

    m_catalog = CreateModuleCatalog();
    qDebug() << "[QtPrism] Step 2: Module catalog created";

    RegisterRequiredTypes(m_container.get());
    qDebug() << "[QtPrism] Step 3: Required types registered";

    RegisterTypes(m_container.get());
    qDebug() << "[QtPrism] Step 4: User types registered";

    ConfigureModuleCatalog(m_catalog.get());
    qDebug() << "[QtPrism] Step 5: Module catalog configured";

    RegisterFrameworkQmlTypes();
    qDebug() << "[QtPrism] Step 6: QML types registered";

    // Step 7: Initialize modules BEFORE loading Shell
    InitializeModules();
    qDebug() << "[QtPrism] Step 7: Modules initialized";

    // Step 8: Let subclass set up context properties, regions, controllers
    OnInitialized();
    qDebug() << "[QtPrism] Step 8: Application initialized";

    // Step 9: Load Shell QML LAST — all context properties are now ready
    QUrl shellUrl = CreateShell();
    qDebug() << "[QtPrism] Step 9: Loading shell:" << shellUrl;
    m_engine->load(shellUrl);

    qDebug() << "[QtPrism] ========== Initialization Complete ==========";
}

std::shared_ptr<IContainerExtension> QtPrismApplication::CreateContainerExtension()
{
    return std::make_shared<QtPrismContainerImpl>();
}

std::shared_ptr<IModuleCatalog> QtPrismApplication::CreateModuleCatalog()
{
    return std::make_shared<ModuleCatalog>();
}

void QtPrismApplication::RegisterRequiredTypes(IContainerRegistry* registry)
{
    registry->RegisterSingleton<IEventAggregator, EventAggregator>();
    registry->RegisterSingleton<IRegionManager, RegionManager>();
    registry->RegisterSingleton<IViewRegistry, ViewRegistry>();
    registry->RegisterSingleton<RegionAdapterMappings, RegionAdapterMappings>();

    registry->RegisterInstance<IModuleCatalog>(m_catalog);

    auto moduleManager = std::make_shared<ModuleManager>(m_catalog, m_container);
    registry->RegisterInstance<IModuleManager>(
        std::static_pointer_cast<IModuleManager>(moduleManager));
}

void QtPrismApplication::InitializeModules()
{
    auto moduleManager = m_container->Resolve<IModuleManager>();
    if (moduleManager)
    {
        moduleManager->Run();
    }
}

void QtPrismApplication::RegisterFrameworkQmlTypes()
{
}

void QtPrismApplication::OnInitialized()
{
    qDebug() << "[QtPrism] Application initialized successfully";
}
