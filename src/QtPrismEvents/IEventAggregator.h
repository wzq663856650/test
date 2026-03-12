#pragma once

#include "EventBase.h"

#include <memory>
#include <typeindex>

class IEventAggregator
{
public:
    virtual ~IEventAggregator() = default;

    template<typename TEvent>
    std::shared_ptr<TEvent> GetEvent()
    {
        auto typeIdx = std::type_index(typeid(TEvent));
        auto base = GetEventImpl(typeIdx);
        if (!base)
        {
            auto newEvent = std::make_shared<TEvent>();
            StoreEvent(typeIdx, newEvent);
            return newEvent;
        }
        return std::static_pointer_cast<TEvent>(base);
    }

    virtual std::shared_ptr<EventBase> GetEventImpl(std::type_index eventType) = 0;
    virtual void StoreEvent(std::type_index eventType, std::shared_ptr<EventBase> event) = 0;
};
