#include "ContainerTests.h"
#include "QtPrismContainerImpl.h"
#include "ContainerLocator.h"

class ITestService
{
public:
    virtual ~ITestService() = default;
    virtual int GetValue() const = 0;
};

class TestServiceImpl : public ITestService
{
public:
    TestServiceImpl() : m_id(s_nextId++) {}
    int GetValue() const override { return m_id; }
    static int s_nextId;

private:
    int m_id;
};

int TestServiceImpl::s_nextId = 1;

void ContainerTests::testRegisterAndResolveSingleton()
{
    auto container = std::make_shared<QtPrismContainerImpl>();
    container->RegisterSingleton<ITestService, TestServiceImpl>();

    auto s1 = container->Resolve<ITestService>();
    auto s2 = container->Resolve<ITestService>();

    QVERIFY(s1 != nullptr);
    QVERIFY(s2 != nullptr);
    QCOMPARE(s1.get(), s2.get());
}

void ContainerTests::testRegisterAndResolveTransient()
{
    auto container = std::make_shared<QtPrismContainerImpl>();
    container->Register<ITestService, TestServiceImpl>();

    auto s1 = container->Resolve<ITestService>();
    auto s2 = container->Resolve<ITestService>();

    QVERIFY(s1 != nullptr);
    QVERIFY(s2 != nullptr);
    QVERIFY(s1.get() != s2.get());
}

void ContainerTests::testRegisterInstance()
{
    auto container = std::make_shared<QtPrismContainerImpl>();
    auto instance = std::make_shared<TestServiceImpl>();

    container->RegisterInstance<ITestService>(instance);
    auto resolved = container->Resolve<ITestService>();

    QVERIFY(resolved != nullptr);
    QCOMPARE(resolved.get(), instance.get());
}

void ContainerTests::testNamedRegistration()
{
    auto container = std::make_shared<QtPrismContainerImpl>();
    container->Register<ITestService, TestServiceImpl>("ServiceA");

    auto resolved = container->Resolve<ITestService>("ServiceA");
    QVERIFY(resolved != nullptr);
}

void ContainerTests::testIsRegistered()
{
    auto container = std::make_shared<QtPrismContainerImpl>();
    QVERIFY(!container->IsRegistered<ITestService>());

    container->Register<ITestService, TestServiceImpl>();
    QVERIFY(container->IsRegistered<ITestService>());
}

void ContainerTests::testResolveUnregistered()
{
    auto container = std::make_shared<QtPrismContainerImpl>();
    auto result = container->Resolve<ITestService>();
    QVERIFY(result == nullptr);
}
