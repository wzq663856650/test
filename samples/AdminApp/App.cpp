#include "App.h"
#include "Modularity/ModuleCatalog.h"
#include "Regions/IRegionManager.h"
#include "Regions/Region.h"
#include "Navigation/NavigationJournal.h"
#include "Views/ViewRegistry.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>

App::App(QGuiApplication* app, QQmlApplicationEngine* engine, QObject* parent)
    : QtPrismApplication(app, engine, parent)
{
}

QString App::pluginDir() const
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("../plugins");
    return dir.absolutePath();
}

void App::RegisterTypes(IContainerRegistry* registry)
{
    Q_UNUSED(registry)
    qDebug() << "[App] RegisterTypes (no static modules — all loaded as plugins)";
}

void App::ConfigureModuleCatalog(IModuleCatalog* catalog)
{
    qDebug() << "[App] ConfigureModuleCatalog";
    qDebug() << "[App] Plugin directory:" << pluginDir();

    auto* moduleCatalog = static_cast<ModuleCatalog*>(catalog);
    QString dir = pluginDir();

    moduleCatalog->AddModule("DashboardModule",
        dir + "/DashboardModule.so",
        InitializationMode::WhenAvailable);

    moduleCatalog->AddModule("OrderModule",
        dir + "/OrderModule.so",
        InitializationMode::WhenAvailable,
        {"DashboardModule"});

    moduleCatalog->AddModule("SettingsModule",
        dir + "/SettingsModule.so",
        InitializationMode::OnDemand);
}

QUrl App::CreateShell()
{
    return QUrl("qrc:/qtprism/ShellView.qml");
}

void App::OnInitialized()
{
    QtPrismApplication::OnInitialized();

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
