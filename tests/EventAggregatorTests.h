#pragma once

#include <QObject>
#include <QTest>

class EventAggregatorTests : public QObject
{
    Q_OBJECT

private slots:
    void testPublishSubscribe();
    void testEventFilter();
    void testUnsubscribe();
    void testMultipleSubscribers();
    void testVoidEvent();
};
