#pragma once

#include <QObject>
#include <QTest>

class NavigationJournalTests : public QObject
{
    Q_OBJECT

private slots:
    void testRecordNavigation();
    void testGoBack();
    void testGoForward();
    void testClear();
    void testNavigateCallback();
};
