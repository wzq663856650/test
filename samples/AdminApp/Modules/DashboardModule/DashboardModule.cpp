#include "DashboardModule.h"
#include "ViewModels/DashboardViewModel.h"
#include "Regions/IRegionManager.h"
#include "Regions/IViewRegistry.h"
#include "IEventAggregator.h"
#include <QDebug>

void DashboardModule::RegisterTypes(IContainerRegistry* containerRegistry)
{
    Q_UNUSED(containerRegistry)
    qDebug() << "[DashboardModule] RegisterTypes";
}

void DashboardModule::OnInitialized(IContainerProvider* containerProvider)
{
    qDebug() << "[DashboardModule] OnInitialized";

    auto regionManager = containerProvider->Resolve<IRegionManager>();
    if (regionManager)
    {
        regionManager->RegisterViewWithRegion("MainRegion", "DashboardView");
    }

    auto viewRegistry = containerProvider->Resolve<IViewRegistry>();
    if (viewRegistry)
    {
        viewRegistry->RegisterView("DashboardView",
            QUrl("qrc:/qtprism/DashboardView.qml"));
    }
}
