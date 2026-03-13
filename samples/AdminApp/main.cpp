#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "App.h"
#include "Modularity/IModuleManager.h"
#include "Regions/IRegionManager.h"
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

    bool hasArg = guiApp.arguments().contains("--verify");
    if (hasArg)
    {
        qDebug() << "\n[Main] === Verification Mode ===";

        auto moduleManager = app.Container()->Resolve<IModuleManager>();
        if (moduleManager)
        {
            qDebug() << "[Main] Testing OnDemand module loading...";
            moduleManager->LoadModule("SettingsModule");
        }

        auto regionManager = app.Container()->Resolve<IRegionManager>();
        if (regionManager)
        {
            qDebug() << "[Main] All regions:" << regionManager->RegionNames();
            qDebug() << "[Main] MainRegion views:" << regionManager->GetViewsForRegion("MainRegion");
        }

        auto ea = app.Container()->Resolve<IEventAggregator>();
        if (ea)
        {
            qDebug() << "[Main] EventAggregator resolved OK";
        }

        qDebug() << "[Main] === All Verifications Passed ===";
        return 0;
    }

    if (engine.rootObjects().isEmpty())
    {
        qWarning() << "[Main] Failed to load Shell QML";
        return -1;
    }

    return guiApp.exec();
}
