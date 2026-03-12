#include "OrderListViewModel.h"
#include "ContainerLocator.h"
#include "IEventAggregator.h"
#include "../Events/OrderEvents.h"
#include "Regions/IRegionManager.h"

#include <QDebug>

// ---- OrderListModel ----

OrderListModel::OrderListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int OrderListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_orders.size();
}

QVariant OrderListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_orders.size())
    {
        return {};
    }

    const auto& order = m_orders[index.row()];
    switch (role)
    {
    case IdRole:       return order.id;
    case CustomerRole: return order.customerName;
    case TotalRole:    return order.total;
    case StatusRole:   return order.status;
    default:           return {};
    }
}

QHash<int, QByteArray> OrderListModel::roleNames() const
{
    return {
        {IdRole, "orderId"},
        {CustomerRole, "customerName"},
        {TotalRole, "total"},
        {StatusRole, "status"}
    };
}

void OrderListModel::setOrders(const QList<Order>& orders)
{
    beginResetModel();
    m_orders = orders;
    endResetModel();
}

// ---- OrderListViewModel ----

OrderListViewModel::OrderListViewModel(
    std::shared_ptr<IOrderService> orderService,
    QObject* parent)
    : ViewModelBase(parent)
    , m_orderService(std::move(orderService))
{
    if (m_orderService)
    {
        m_orderModel.setOrders(m_orderService->GetOrders());
    }
}

void OrderListViewModel::OnNavigatedTo(const NavigationContext& context)
{
    Q_UNUSED(context)
    qDebug() << "[OrderListVM] OnNavigatedTo";
    if (m_orderService)
    {
        m_orderModel.setOrders(m_orderService->GetOrders());
    }
}

void OrderListViewModel::OnNavigatedFrom(const NavigationContext& context)
{
    Q_UNUSED(context)
    qDebug() << "[OrderListVM] OnNavigatedFrom";
}

bool OrderListViewModel::IsNavigationTarget(const NavigationContext& context)
{
    Q_UNUSED(context)
    return true;
}

void OrderListViewModel::selectOrder(int orderId)
{
    qDebug() << "[OrderListVM] Order selected:" << orderId;

    auto container = ContainerLocator::Current();
    if (!container) return;

    auto ea = container->Resolve<IEventAggregator>();
    if (ea)
    {
        OrderSelectedPayload payload{orderId, QString("Order #%1").arg(orderId)};
        ea->GetEvent<OrderSelectedEvent>()->Publish(payload);
    }

    auto regionManager = container->Resolve<IRegionManager>();
    if (regionManager)
    {
        QVariantMap params;
        params["orderId"] = orderId;
        regionManager->RequestNavigate("MainRegion", "OrderDetailView", params);
    }
}
