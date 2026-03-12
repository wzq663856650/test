#pragma once

#include <QString>
#include <QList>

struct Order
{
    int id = 0;
    QString customerName;
    double total = 0.0;
    QString status;
};

class IOrderService
{
public:
    virtual ~IOrderService() = default;
    virtual QList<Order> GetOrders() const = 0;
    virtual Order GetOrderById(int id) const = 0;
    virtual void SubmitOrder(const Order& order) = 0;
};
