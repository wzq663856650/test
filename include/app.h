#ifndef APP_H
#define APP_H

#include <string>

class App
{
public:
    App(const std::string &name);
    ~App() = default;

    void run();
    std::string getName() const;

private:
    std::string m_name;
};

#endif // APP_H
