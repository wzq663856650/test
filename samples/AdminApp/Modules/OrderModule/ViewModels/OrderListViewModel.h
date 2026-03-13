#pragma once

#include "Mvvm/ViewModelBase.h"
#include "../Services/IOrderService.h"
#include "Navigation/INavigationAware.h"
#include "Navigation/NavigationContext.h"

#include <QAbstractListModel>
#include <memory>

class OrderListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        CustomerRole,
        TotalRole,
        StatusRole
    };

    explicit OrderListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setOrders(const QList<Order>& orders);

private:
    QList<Order> m_orders;
};

class OrderListViewModel : public ViewModelBase, public INavigationAware
{
    Q_OBJECT
    Q_PROPERTY(OrderListModel* orderModel READ orderModel CONSTANT)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)

public:
    explicit OrderListViewModel(std::shared_ptr<IOrderService> orderService,
                                QObject* parent = nullptr);

    OrderListModel* orderModel() { return &m_orderModel; }
    QString title() const { return m_title; }

    void OnNavigatedTo(const NavigationContext& context) override;
    void OnNavigatedFrom(const NavigationContext& context) override;
    bool IsNavigationTarget(const NavigationContext& context) override;

    Q_INVOKABLE void selectOrder(int orderId);

signals:
    void titleChanged();
    void orderSelected(int orderId);

private:
    std::shared_ptr<IOrderService> m_orderService;
    OrderListModel m_orderModel;
    QString m_title = "Order List";
};
