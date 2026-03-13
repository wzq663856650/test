#pragma once

#include "PubSubEvent.h"
#include <QString>

struct OrderSelectedPayload
{
    int orderId = 0;
    QString orderName;
};

class OrderSelectedEvent : public PubSubEvent<OrderSelectedPayload> {};

class OrderSubmittedEvent : public PubSubEvent<int> {};
