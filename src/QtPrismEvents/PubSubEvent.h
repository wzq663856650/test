#pragma once

#include "EventBase.h"
#include "ThreadOption.h"

#include <QMetaObject>
#include <QCoreApplication>
#include <QtConcurrent>

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <algorithm>

template<typename TPayload = void>
class PubSubEvent;

template<typename TPayload>
class PubSubEvent : public EventBase
{
public:
    using Handler = std::function<void(const TPayload&)>;
    using Filter = std::function<bool(const TPayload&)>;

    SubscriptionToken Subscribe(
        Handler action,
        ThreadOption threadOption = ThreadOption::PublisherThread,
        bool /*keepReference*/ = true,
        Filter filter = nullptr)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        uint64_t id = s_nextId.fetch_add(1);
        m_subscriptions.push_back({id, std::move(action), std::move(filter), threadOption});
        return SubscriptionToken{id};
    }

    void Publish(const TPayload& payload)
    {
        std::vector<Subscription> subs;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            subs = m_subscriptions;
        }

        for (auto& sub : subs)
        {
            if (sub.filter && !sub.filter(payload))
            {
                continue;
            }

            switch (sub.threadOption)
            {
            case ThreadOption::PublisherThread:
                sub.action(payload);
                break;

            case ThreadOption::UIThread:
            {
                auto action = sub.action;
                auto p = payload;
                QMetaObject::invokeMethod(
                    QCoreApplication::instance(),
                    [action, p]() { action(p); },
                    Qt::QueuedConnection);
                break;
            }

            case ThreadOption::BackgroundThread:
            {
                auto action = sub.action;
                auto p = payload;
                (void)QtConcurrent::run([action, p]() { action(p); });
                break;
            }
            }
        }
    }

    void Unsubscribe(const SubscriptionToken& token)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_subscriptions.erase(
            std::remove_if(m_subscriptions.begin(), m_subscriptions.end(),
                [&token](const Subscription& s) { return s.id == token.id; }),
            m_subscriptions.end());
    }

    void Prune() override
    {
        // No-op for strong reference mode
    }

    size_t SubscriptionCount() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_subscriptions.size();
    }

private:
    struct Subscription
    {
        uint64_t id;
        Handler action;
        Filter filter;
        ThreadOption threadOption;
    };

    std::vector<Subscription> m_subscriptions;
    mutable std::mutex m_mutex;
    static std::atomic<uint64_t> s_nextId;
};

template<typename TPayload>
std::atomic<uint64_t> PubSubEvent<TPayload>::s_nextId{1};

template<>
class PubSubEvent<void> : public EventBase
{
public:
    using Handler = std::function<void()>;

    SubscriptionToken Subscribe(
        Handler action,
        ThreadOption threadOption = ThreadOption::PublisherThread)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        uint64_t id = s_nextId.fetch_add(1);
        m_subscriptions.push_back({id, std::move(action), threadOption});
        return SubscriptionToken{id};
    }

    void Publish()
    {
        std::vector<Subscription> subs;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            subs = m_subscriptions;
        }

        for (auto& sub : subs)
        {
            switch (sub.threadOption)
            {
            case ThreadOption::PublisherThread:
                sub.action();
                break;

            case ThreadOption::UIThread:
            {
                auto action = sub.action;
                QMetaObject::invokeMethod(
                    QCoreApplication::instance(),
                    [action]() { action(); },
                    Qt::QueuedConnection);
                break;
            }

            case ThreadOption::BackgroundThread:
            {
                auto action = sub.action;
                (void)QtConcurrent::run([action]() { action(); });
                break;
            }
            }
        }
    }

    void Unsubscribe(const SubscriptionToken& token)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_subscriptions.erase(
            std::remove_if(m_subscriptions.begin(), m_subscriptions.end(),
                [&token](const Subscription& s) { return s.id == token.id; }),
            m_subscriptions.end());
    }

    void Prune() override {}

    size_t SubscriptionCount() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_subscriptions.size();
    }

private:
    struct Subscription
    {
        uint64_t id;
        Handler action;
        ThreadOption threadOption;
    };

    std::vector<Subscription> m_subscriptions;
    mutable std::mutex m_mutex;
    static std::atomic<uint64_t> s_nextId;
};
