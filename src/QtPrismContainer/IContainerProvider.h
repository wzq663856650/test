#pragma once

#include <memory>
#include <typeindex>
#include <QString>

class IContainerProvider
{
public:
    virtual ~IContainerProvider() = default;

    template<typename T>
    std::shared_ptr<T> Resolve()
    {
        auto ptr = ResolveImpl(std::type_index(typeid(T)));
        return std::static_pointer_cast<T>(ptr);
    }

    template<typename T>
    std::shared_ptr<T> Resolve(const QString& name)
    {
        auto ptr = ResolveNamedImpl(std::type_index(typeid(T)), name);
        return std::static_pointer_cast<T>(ptr);
    }

    virtual std::shared_ptr<void> ResolveImpl(std::type_index type) = 0;
    virtual std::shared_ptr<void> ResolveNamedImpl(std::type_index type, const QString& name) = 0;
};
