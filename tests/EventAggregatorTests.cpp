#include "EventAggregatorTests.h"
#include "EventAggregator.h"
#include "PubSubEvent.h"

class TestStringEvent : public PubSubEvent<QString> {};
class TestIntEvent : public PubSubEvent<int> {};
class TestVoidEvent : public PubSubEvent<void> {};

void EventAggregatorTests::testPublishSubscribe()
{
    EventAggregator ea;
    QString received;

    ea.GetEvent<TestStringEvent>()->Subscribe(
        [&received](const QString& msg) { received = msg; });

    ea.GetEvent<TestStringEvent>()->Publish("Hello QtPrism");

    QCOMPARE(received, QString("Hello QtPrism"));
}

void EventAggregatorTests::testEventFilter()
{
    EventAggregator ea;
    int received = -1;

    ea.GetEvent<TestIntEvent>()->Subscribe(
        [&received](const int& val) { received = val; },
        ThreadOption::PublisherThread,
        true,
        [](const int& val) { return val > 10; }
    );

    ea.GetEvent<TestIntEvent>()->Publish(5);
    QCOMPARE(received, -1);

    ea.GetEvent<TestIntEvent>()->Publish(42);
    QCOMPARE(received, 42);
}

void EventAggregatorTests::testUnsubscribe()
{
    EventAggregator ea;
    int callCount = 0;

    auto token = ea.GetEvent<TestIntEvent>()->Subscribe(
        [&callCount](const int&) { callCount++; });

    ea.GetEvent<TestIntEvent>()->Publish(1);
    QCOMPARE(callCount, 1);

    ea.GetEvent<TestIntEvent>()->Unsubscribe(token);
    ea.GetEvent<TestIntEvent>()->Publish(2);
    QCOMPARE(callCount, 1);
}

void EventAggregatorTests::testMultipleSubscribers()
{
    EventAggregator ea;
    int count1 = 0, count2 = 0;

    ea.GetEvent<TestIntEvent>()->Subscribe(
        [&count1](const int&) { count1++; });
    ea.GetEvent<TestIntEvent>()->Subscribe(
        [&count2](const int&) { count2++; });

    ea.GetEvent<TestIntEvent>()->Publish(1);

    QCOMPARE(count1, 1);
    QCOMPARE(count2, 1);
}

void EventAggregatorTests::testVoidEvent()
{
    EventAggregator ea;
    int callCount = 0;

    ea.GetEvent<TestVoidEvent>()->Subscribe(
        [&callCount]() { callCount++; });

    ea.GetEvent<TestVoidEvent>()->Publish();
    ea.GetEvent<TestVoidEvent>()->Publish();

    QCOMPARE(callCount, 2);
}
