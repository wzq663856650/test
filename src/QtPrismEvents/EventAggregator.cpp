#include "EventAggregator.h"
#include "PubSubEvent.h"

std::atomic<uint64_t> PubSubEvent<void>::s_nextId{1};

std::shared_ptr<EventBase> EventAggregator::GetEventImpl(std::type_index eventType)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_events.find(eventType);
    if (it != m_events.end())
    {
        return it->second;
    }
    return nullptr;
}

void EventAggregator::StoreEvent(std::type_index eventType, std::shared_ptr<EventBase> event)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_events[eventType] = std::move(event);
}
