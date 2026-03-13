#pragma once

#include "IContainerExtension.h"
#include <memory>
#include <mutex>

class ContainerLocator
{
public:
    static void SetContainerExtension(std::shared_ptr<IContainerExtension> extension);
    static std::shared_ptr<IContainerExtension> Current();
    static std::shared_ptr<IContainerProvider> Container();
    static void ResetContainer();

private:
    ContainerLocator() = default;
    static std::shared_ptr<IContainerExtension> s_current;
    static std::mutex s_mutex;
};
