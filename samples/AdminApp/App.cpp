#include "App.h"
#include "Shell/ShellController.h"
#include "Modularity/ModuleCatalog.h"
#include "Regions/IRegionManager.h"
#include "Regions/Region.h"
#include "Navigation/NavigationJournal.h"
#include "Views/ViewRegistry.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QQmlContext>

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

QString App::pluginFileName(const QString& baseName) const
{
#ifdef Q_OS_WIN
    return baseName + ".dll";
#else
    return baseName + ".so";
#endif
}

void App::RegisterTypes(IContainerRegistry* registry)
{
    Q_UNUSED(registry)
    qDebug() << "[App] RegisterTypes (all modules loaded as plugins)";
}

void App::ConfigureModuleCatalog(IModuleCatalog* catalog)
{
    qDebug() << "[App] ConfigureModuleCatalog";

    QString dir = pluginDir();
    qDebug() << "[App] Plugin directory:" << dir;

    auto* moduleCatalog = static_cast<ModuleCatalog*>(catalog);

    moduleCatalog->AddModule("DashboardModule",
        dir + "/" + pluginFileName("DashboardModule"),
        InitializationMode::WhenAvailable);

    moduleCatalog->AddModule("OrderModule",
        dir + "/" + pluginFileName("OrderModule"),
        InitializationMode::WhenAvailable,
        {"DashboardModule"});

    moduleCatalog->AddModule("SettingsModule",
        dir + "/" + pluginFileName("SettingsModule"),
        InitializationMode::OnDemand);
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
    }

    // Create ShellController and configure navigation items
    auto moduleManager = Container()->Resolve<IModuleManager>();

    m_shellController = new ShellController(viewRegistry, regionManager, moduleManager, this);

    m_shellController->addNavigationItem(
        "Dashboard", "DashboardView", "\xe2\x8c\x82", "DashboardModule", false);
    m_shellController->addNavigationItem(
        "Orders", "OrderListView", "\xe2\x98\xb7", "OrderModule", false);
    m_shellController->addNavigationItem(
        "Settings", "SettingsView", "\xe2\x9a\x99", "SettingsModule", true);

    Engine()->rootContext()->setContextProperty("shellController", m_shellController);

    // Trigger initial navigation to first available view
    m_shellController->refreshNavigation();

    qDebug() << "[App] ShellController created and exposed to QML";
}
