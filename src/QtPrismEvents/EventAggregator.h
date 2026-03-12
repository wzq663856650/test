#pragma once

#include "IEventAggregator.h"

#include <mutex>
#include <unordered_map>

class EventAggregator : public IEventAggregator
{
public:
    EventAggregator() = default;
    ~EventAggregator() override = default;

    std::shared_ptr<EventBase> GetEventImpl(std::type_index eventType) override;
    void StoreEvent(std::type_index eventType, std::shared_ptr<EventBase> event) override;

private:
    std::unordered_map<std::type_index, std::shared_ptr<EventBase>> m_events;
    std::mutex m_mutex;
};
