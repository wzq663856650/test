#pragma once

class IActiveAware
{
public:
    virtual ~IActiveAware() = default;
    virtual bool isActive() const = 0;
    virtual void setIsActive(bool active) = 0;
};
