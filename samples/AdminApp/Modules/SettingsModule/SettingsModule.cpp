#include "SettingsModule.h"
#include "Regions/IRegionManager.h"
#include "Regions/IViewRegistry.h"
#include <QDebug>

void SettingsModule::RegisterTypes(IContainerRegistry* containerRegistry)
{
    Q_UNUSED(containerRegistry)
    qDebug() << "[SettingsModule] RegisterTypes";
}

void SettingsModule::OnInitialized(IContainerProvider* containerProvider)
{
    qDebug() << "[SettingsModule] OnInitialized (OnDemand module loaded!)";

    auto regionManager = containerProvider->Resolve<IRegionManager>();
    if (regionManager)
    {
        regionManager->RegisterViewWithRegion("MainRegion", "SettingsView");
    }

    auto viewRegistry = containerProvider->Resolve<IViewRegistry>();
    if (viewRegistry)
    {
        viewRegistry->RegisterView("SettingsView",
            QUrl("qrc:/qtprism/modules/SettingsView.qml"));
    }
}
