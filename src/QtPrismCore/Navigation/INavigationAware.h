#pragma once

#include "NavigationContext.h"

class INavigationAware
{
public:
    virtual ~INavigationAware() = default;
    virtual void OnNavigatedTo(const NavigationContext& context) = 0;
    virtual void OnNavigatedFrom(const NavigationContext& context) = 0;
    virtual bool IsNavigationTarget(const NavigationContext& context) = 0;
};
