#include "App.h"
#include "Modularity/ModuleCatalog.h"
#include "Modules/DashboardModule/DashboardModule.h"
#include "Modules/OrderModule/OrderModule.h"
#include "Modules/SettingsModule/SettingsModule.h"
#include "Regions/IRegionManager.h"
#include "Regions/Region.h"
#include "Navigation/NavigationJournal.h"
#include "Views/ViewRegistry.h"

#include <QDebug>

App::App(QGuiApplication* app, QQmlApplicationEngine* engine, QObject* parent)
    : QtPrismApplication(app, engine, parent)
{
}

void App::RegisterTypes(IContainerRegistry* registry)
{
    qDebug() << "[App] RegisterTypes";

    // Register modules as named IModule instances for static loading
    auto dashboardModule = std::make_shared<DashboardModule>();
    registry->RegisterNamedFactory(
        std::type_index(typeid(IModule)),
        "DashboardModule",
        [dashboardModule]() -> std::shared_ptr<void> { return dashboardModule; },
        true
    );

    auto orderModule = std::make_shared<OrderModule>();
    registry->RegisterNamedFactory(
        std::type_index(typeid(IModule)),
        "OrderModule",
        [orderModule]() -> std::shared_ptr<void> { return orderModule; },
        true
    );

    auto settingsModule = std::make_shared<SettingsModule>();
    registry->RegisterNamedFactory(
        std::type_index(typeid(IModule)),
        "SettingsModule",
        [settingsModule]() -> std::shared_ptr<void> { return settingsModule; },
        true
    );
}

void App::ConfigureModuleCatalog(IModuleCatalog* catalog)
{
    qDebug() << "[App] ConfigureModuleCatalog";

    auto* moduleCatalog = static_cast<ModuleCatalog*>(catalog);

    moduleCatalog->AddModule("DashboardModule", "", InitializationMode::WhenAvailable);
    moduleCatalog->AddModule("OrderModule", "", InitializationMode::WhenAvailable, {"DashboardModule"});
    moduleCatalog->AddModule("SettingsModule", "", InitializationMode::OnDemand);
}

QUrl App::CreateShell()
{
    return QUrl("qrc:/qtprism/ShellView.qml");
}

void App::OnInitialized()
{
    QtPrismApplication::OnInitialized();

    // Create MainRegion
    auto regionManager = Container()->Resolve<IRegionManager>();
    auto viewRegistry = Container()->Resolve<IViewRegistry>();

    if (regionManager)
    {
        auto mainRegion = std::make_shared<Region>("MainRegion");
        mainRegion->SetViewRegistry(viewRegistry);

        auto journal = std::make_shared<NavigationJournal>();
        mainRegion->SetNavigationJournal(journal);

        regionManager->AddRegion("MainRegion", mainRegion);

        qDebug() << "[App] MainRegion created. Registered views:"
                 << regionManager->GetViewsForRegion("MainRegion");
    }
}
