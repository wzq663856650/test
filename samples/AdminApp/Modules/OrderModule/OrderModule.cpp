#include "OrderModule.h"
#include "Services/OrderServiceImpl.h"
#include "Regions/IRegionManager.h"
#include "Regions/IViewRegistry.h"
#include "IEventAggregator.h"
#include "Events/OrderEvents.h"
#include <QDebug>

void OrderModule::RegisterTypes(IContainerRegistry* containerRegistry)
{
    qDebug() << "[OrderModule] RegisterTypes";
    containerRegistry->RegisterSingleton<IOrderService, OrderServiceImpl>();
}

void OrderModule::OnInitialized(IContainerProvider* containerProvider)
{
    qDebug() << "[OrderModule] OnInitialized";

    auto regionManager = containerProvider->Resolve<IRegionManager>();
    if (regionManager)
    {
        regionManager->RegisterViewWithRegion("MainRegion", "OrderListView");
        regionManager->RegisterViewWithRegion("MainRegion", "OrderDetailView");
    }

    auto viewRegistry = containerProvider->Resolve<IViewRegistry>();
    if (viewRegistry)
    {
        viewRegistry->RegisterView("OrderListView",
            QUrl("qrc:/qtprism/modules/OrderListView.qml"));
        viewRegistry->RegisterView("OrderDetailView",
            QUrl("qrc:/qtprism/modules/OrderDetailView.qml"));
    }

    auto ea = containerProvider->Resolve<IEventAggregator>();
    if (ea)
    {
        ea->GetEvent<OrderSelectedEvent>()->Subscribe(
            [](const OrderSelectedPayload& payload)
            {
                qDebug() << "[OrderModule] Event received - Order selected:"
                         << payload.orderId << payload.orderName;
            },
            ThreadOption::PublisherThread
        );
    }
}
