#pragma once

#include <cstdint>

class EventBase
{
public:
    virtual ~EventBase() = default;
    virtual void Prune() = 0;
};

struct SubscriptionToken
{
    uint64_t id = 0;

    bool operator==(const SubscriptionToken& other) const { return id == other.id; }
    bool operator!=(const SubscriptionToken& other) const { return id != other.id; }
};
