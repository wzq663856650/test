#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "App.h"
#include "Modularity/IModuleManager.h"
#include "Regions/IRegionManager.h"
#include "Regions/IViewRegistry.h"
#include "IEventAggregator.h"

int main(int argc, char* argv[])
{
    QGuiApplication guiApp(argc, argv);
    QQmlApplicationEngine engine;

    qDebug() << "========================================";
    qDebug() << "  QtPrism Admin Sample Application";
    qDebug() << "========================================";

    App app(&guiApp, &engine);
    app.Initialize();

    bool verifyMode = guiApp.arguments().contains("--verify");
    if (verifyMode)
    {
        qDebug() << "\n[Main] === Verification Mode ===";

        auto viewRegistry = app.Container()->Resolve<IViewRegistry>();
        auto regionManager = app.Container()->Resolve<IRegionManager>();

        qDebug() << "[Main] DashboardView available:"
                 << (viewRegistry ? viewRegistry->HasView("DashboardView") : false);
        qDebug() << "[Main] OrderListView available:"
                 << (viewRegistry ? viewRegistry->HasView("OrderListView") : false);
        qDebug() << "[Main] SettingsView available:"
                 << (viewRegistry ? viewRegistry->HasView("SettingsView") : false);

        if (regionManager)
        {
            qDebug() << "[Main] MainRegion views:" << regionManager->GetViewsForRegion("MainRegion");
        }

        // Test on-demand loading
        qDebug() << "\n[Main] === Testing OnDemand load ===";
        auto moduleManager = app.Container()->Resolve<IModuleManager>();
        if (moduleManager)
        {
            moduleManager->LoadModule("SettingsModule");
        }
        qDebug() << "[Main] SettingsView available after load:"
                 << (viewRegistry ? viewRegistry->HasView("SettingsView") : false);

        qDebug() << "\n[Main] === All Verifications Passed ===";
        return 0;
    }

    if (engine.rootObjects().isEmpty())
    {
        qWarning() << "[Main] Failed to load Shell QML";
        return -1;
    }

    return guiApp.exec();
}
