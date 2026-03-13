#pragma once

#include "IOrderService.h"
#include <QList>

class OrderServiceImpl : public IOrderService
{
public:
    OrderServiceImpl();

    QList<Order> GetOrders() const override;
    Order GetOrderById(int id) const override;
    void SubmitOrder(const Order& order) override;

private:
    QList<Order> m_orders;
};
