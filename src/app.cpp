#include "app.h"

#include <iostream>

App::App(const std::string &name)
    : m_name(name)
{
}

void App::run()
{
    std::cout << "Hello from " << m_name << "!" << std::endl;
    std::cout << "This is a simple C++/CMake project." << std::endl;
}

std::string App::getName() const
{
    return m_name;
}
