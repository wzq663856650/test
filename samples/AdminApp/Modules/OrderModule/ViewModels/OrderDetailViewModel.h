#pragma once

#include "Mvvm/ViewModelBase.h"
#include "../Services/IOrderService.h"
#include "Navigation/INavigationAware.h"
#include "Navigation/NavigationContext.h"

#include <memory>

class OrderDetailViewModel : public ViewModelBase, public INavigationAware
{
    Q_OBJECT
    Q_PROPERTY(int orderId READ orderId NOTIFY orderChanged)
    Q_PROPERTY(QString customerName READ customerName NOTIFY orderChanged)
    Q_PROPERTY(double total READ total NOTIFY orderChanged)
    Q_PROPERTY(QString status READ status NOTIFY orderChanged)

public:
    explicit OrderDetailViewModel(std::shared_ptr<IOrderService> orderService,
                                  QObject* parent = nullptr);

    int orderId() const { return m_order.id; }
    QString customerName() const { return m_order.customerName; }
    double total() const { return m_order.total; }
    QString status() const { return m_order.status; }

    void OnNavigatedTo(const NavigationContext& context) override;
    void OnNavigatedFrom(const NavigationContext& context) override;
    bool IsNavigationTarget(const NavigationContext& context) override;

    Q_INVOKABLE void goBack();

signals:
    void orderChanged();

private:
    std::shared_ptr<IOrderService> m_orderService;
    Order m_order;
};
