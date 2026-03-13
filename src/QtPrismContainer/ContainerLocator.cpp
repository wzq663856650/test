#include "ContainerLocator.h"

std::shared_ptr<IContainerExtension> ContainerLocator::s_current;
std::mutex ContainerLocator::s_mutex;

void ContainerLocator::SetContainerExtension(std::shared_ptr<IContainerExtension> extension)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    s_current = std::move(extension);
}

std::shared_ptr<IContainerExtension> ContainerLocator::Current()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    return s_current;
}

std::shared_ptr<IContainerProvider> ContainerLocator::Container()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    return std::static_pointer_cast<IContainerProvider>(s_current);
}

void ContainerLocator::ResetContainer()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    s_current.reset();
}
