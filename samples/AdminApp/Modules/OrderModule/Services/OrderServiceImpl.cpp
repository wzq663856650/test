#include "OrderServiceImpl.h"

OrderServiceImpl::OrderServiceImpl()
{
    m_orders = {
        {1, "Alice Johnson", 299.99, "Completed"},
        {2, "Bob Smith", 149.50, "Processing"},
        {3, "Carol White", 89.00, "Shipped"},
        {4, "David Brown", 450.00, "Completed"},
        {5, "Eve Davis", 35.99, "Pending"}
    };
}

QList<Order> OrderServiceImpl::GetOrders() const
{
    return m_orders;
}

Order OrderServiceImpl::GetOrderById(int id) const
{
    for (const auto& order : m_orders)
    {
        if (order.id == id)
        {
            return order;
        }
    }
    return Order{};
}

void OrderServiceImpl::SubmitOrder(const Order& order)
{
    m_orders.append(order);
}
