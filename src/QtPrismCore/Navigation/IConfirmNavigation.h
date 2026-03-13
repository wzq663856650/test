#pragma once

#include "NavigationContext.h"
#include <functional>

class IConfirmNavigation
{
public:
    virtual ~IConfirmNavigation() = default;
    virtual void ConfirmNavigationRequest(
        const NavigationContext& context,
        std::function<void(bool)> callback) = 0;
};
