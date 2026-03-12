#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <QString>

class IContainerRegistry
{
public:
    virtual ~IContainerRegistry() = default;

    template<typename TInterface, typename TImpl>
    void Register()
    {
        RegisterFactory(
            std::type_index(typeid(TInterface)),
            []() -> std::shared_ptr<void> { return std::make_shared<TImpl>(); },
            false
        );
    }

    template<typename TInterface, typename TImpl>
    void RegisterSingleton()
    {
        RegisterFactory(
            std::type_index(typeid(TInterface)),
            []() -> std::shared_ptr<void> { return std::make_shared<TImpl>(); },
            true
        );
    }

    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> instance)
    {
        RegisterInstanceImpl(
            std::type_index(typeid(TInterface)),
            std::static_pointer_cast<void>(instance)
        );
    }

    template<typename TInterface, typename TImpl>
    void Register(const QString& name)
    {
        RegisterNamedFactory(
            std::type_index(typeid(TInterface)),
            name,
            []() -> std::shared_ptr<void> { return std::make_shared<TImpl>(); },
            false
        );
    }

    template<typename TInterface, typename TImpl>
    void RegisterSingleton(const QString& name)
    {
        RegisterNamedFactory(
            std::type_index(typeid(TInterface)),
            name,
            []() -> std::shared_ptr<void> { return std::make_shared<TImpl>(); },
            true
        );
    }

    template<typename T>
    bool IsRegistered() const
    {
        return IsRegisteredImpl(std::type_index(typeid(T)));
    }

    virtual void RegisterFactory(
        std::type_index type,
        std::function<std::shared_ptr<void>()> factory,
        bool isSingleton) = 0;

    virtual void RegisterNamedFactory(
        std::type_index type,
        const QString& name,
        std::function<std::shared_ptr<void>()> factory,
        bool isSingleton) = 0;

    virtual void RegisterInstanceImpl(
        std::type_index type,
        std::shared_ptr<void> instance) = 0;

    virtual bool IsRegisteredImpl(std::type_index type) const = 0;
};
