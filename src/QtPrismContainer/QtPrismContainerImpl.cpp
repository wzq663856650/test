#include "QtPrismContainerImpl.h"
#include <stdexcept>
#include <QDebug>

void QtPrismContainerImpl::RegisterFactory(
    std::type_index type,
    std::function<std::shared_ptr<void>()> factory,
    bool isSingleton)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_registrations[type] = Registration{std::move(factory), isSingleton, nullptr};
}

void QtPrismContainerImpl::RegisterNamedFactory(
    std::type_index type,
    const QString& name,
    std::function<std::shared_ptr<void>()> factory,
    bool isSingleton)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    NameKey key{type, name};
    m_namedRegistrations[key] = Registration{std::move(factory), isSingleton, nullptr};
}

void QtPrismContainerImpl::RegisterInstanceImpl(
    std::type_index type,
    std::shared_ptr<void> instance)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_registrations[type] = Registration{nullptr, true, std::move(instance)};
}

bool QtPrismContainerImpl::IsRegisteredImpl(std::type_index type) const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_registrations.find(type) != m_registrations.end();
}

std::shared_ptr<void> QtPrismContainerImpl::ResolveImpl(std::type_index type)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    auto it = m_registrations.find(type);
    if (it == m_registrations.end())
    {
        qWarning() << "QtPrismContainer: Type not registered:" << type.name();
        return nullptr;
    }

    auto& reg = it->second;

    if (reg.isSingleton)
    {
        if (!reg.singletonInstance && reg.factory)
        {
            reg.singletonInstance = reg.factory();
        }
        return reg.singletonInstance;
    }

    return reg.factory ? reg.factory() : nullptr;
}

std::shared_ptr<void> QtPrismContainerImpl::ResolveNamedImpl(
    std::type_index type,
    const QString& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    NameKey key{type, name};
    auto it = m_namedRegistrations.find(key);
    if (it == m_namedRegistrations.end())
    {
        qWarning() << "QtPrismContainer: Named type not registered:" << type.name() << name;
        return nullptr;
    }

    auto& reg = it->second;

    if (reg.isSingleton)
    {
        if (!reg.singletonInstance && reg.factory)
        {
            reg.singletonInstance = reg.factory();
        }
        return reg.singletonInstance;
    }

    return reg.factory ? reg.factory() : nullptr;
}
