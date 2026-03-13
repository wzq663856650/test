#include "OrderDetailViewModel.h"
#include "ContainerLocator.h"
#include "Regions/IRegionManager.h"

#include <QDebug>

OrderDetailViewModel::OrderDetailViewModel(
    std::shared_ptr<IOrderService> orderService,
    QObject* parent)
    : ViewModelBase(parent)
    , m_orderService(std::move(orderService))
{
}

void OrderDetailViewModel::OnNavigatedTo(const NavigationContext& context)
{
    int id = context.GetParameter<int>("orderId", 0);
    qDebug() << "[OrderDetailVM] OnNavigatedTo, orderId:" << id;

    if (m_orderService && id > 0)
    {
        m_order = m_orderService->GetOrderById(id);
        emit orderChanged();
    }
}

void OrderDetailViewModel::OnNavigatedFrom(const NavigationContext& context)
{
    Q_UNUSED(context)
    qDebug() << "[OrderDetailVM] OnNavigatedFrom";
}

bool OrderDetailViewModel::IsNavigationTarget(const NavigationContext& context)
{
    int id = context.GetParameter<int>("orderId", 0);
    return m_order.id == id;
}

void OrderDetailViewModel::goBack()
{
    auto container = ContainerLocator::Current();
    if (!container) return;

    auto regionManager = container->Resolve<IRegionManager>();
    if (regionManager)
    {
        regionManager->RequestNavigate("MainRegion", "OrderListView");
    }
}
