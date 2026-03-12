#pragma once

#include "IContainerExtension.h"

#include <unordered_map>
#include <mutex>
#include <QString>
#include <QHash>

class QtPrismContainerImpl : public IContainerExtension
{
public:
    QtPrismContainerImpl() = default;
    ~QtPrismContainerImpl() override = default;

    void FinalizeExtension() override {}

    void RegisterFactory(
        std::type_index type,
        std::function<std::shared_ptr<void>()> factory,
        bool isSingleton) override;

    void RegisterNamedFactory(
        std::type_index type,
        const QString& name,
        std::function<std::shared_ptr<void>()> factory,
        bool isSingleton) override;

    void RegisterInstanceImpl(
        std::type_index type,
        std::shared_ptr<void> instance) override;

    bool IsRegisteredImpl(std::type_index type) const override;

    std::shared_ptr<void> ResolveImpl(std::type_index type) override;
    std::shared_ptr<void> ResolveNamedImpl(std::type_index type, const QString& name) override;

private:
    struct Registration
    {
        std::function<std::shared_ptr<void>()> factory;
        bool isSingleton = false;
        std::shared_ptr<void> singletonInstance;
    };

    std::unordered_map<std::type_index, Registration> m_registrations;

    struct NameKey
    {
        std::type_index type;
        QString name;
        bool operator==(const NameKey& o) const { return type == o.type && name == o.name; }
    };

    struct NameKeyHash
    {
        size_t operator()(const NameKey& k) const
        {
            return std::hash<std::type_index>()(k.type) ^ qHash(k.name);
        }
    };

    std::unordered_map<NameKey, Registration, NameKeyHash> m_namedRegistrations;
    mutable std::recursive_mutex m_mutex;
};
