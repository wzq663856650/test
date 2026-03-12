#pragma once

class IDestructible
{
public:
    virtual ~IDestructible() = default;
    virtual void Destroy() = 0;
};
