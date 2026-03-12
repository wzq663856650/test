#include "NavigationJournalTests.h"
#include "Navigation/NavigationJournal.h"

void NavigationJournalTests::testRecordNavigation()
{
    NavigationJournal journal;

    journal.RecordNavigation("View1", {});
    QCOMPARE(journal.CurrentViewName(), QString("View1"));
    QVERIFY(!journal.CanGoBack());
    QVERIFY(!journal.CanGoForward());

    journal.RecordNavigation("View2", {});
    QCOMPARE(journal.CurrentViewName(), QString("View2"));
    QVERIFY(journal.CanGoBack());
}

void NavigationJournalTests::testGoBack()
{
    NavigationJournal journal;
    QString navigatedTo;

    journal.SetNavigateCallback([&navigatedTo](const QString& view, const QVariantMap&)
    {
        navigatedTo = view;
    });

    journal.RecordNavigation("View1", {});
    journal.RecordNavigation("View2", {});
    journal.RecordNavigation("View3", {});

    QCOMPARE(journal.CurrentViewName(), QString("View3"));

    journal.GoBack();
    QCOMPARE(navigatedTo, QString("View2"));
    QCOMPARE(journal.CurrentViewName(), QString("View2"));

    journal.GoBack();
    QCOMPARE(navigatedTo, QString("View1"));
    QCOMPARE(journal.CurrentViewName(), QString("View1"));

    QVERIFY(!journal.CanGoBack());
    QVERIFY(journal.CanGoForward());
}

void NavigationJournalTests::testGoForward()
{
    NavigationJournal journal;
    QString navigatedTo;

    journal.SetNavigateCallback([&navigatedTo](const QString& view, const QVariantMap&)
    {
        navigatedTo = view;
    });

    journal.RecordNavigation("View1", {});
    journal.RecordNavigation("View2", {});
    journal.RecordNavigation("View3", {});

    journal.GoBack();
    journal.GoBack();
    QCOMPARE(journal.CurrentViewName(), QString("View1"));

    journal.GoForward();
    QCOMPARE(navigatedTo, QString("View2"));

    journal.GoForward();
    QCOMPARE(navigatedTo, QString("View3"));
    QVERIFY(!journal.CanGoForward());
}

void NavigationJournalTests::testClear()
{
    NavigationJournal journal;

    journal.RecordNavigation("View1", {});
    journal.RecordNavigation("View2", {});

    journal.Clear();
    QVERIFY(!journal.CanGoBack());
    QVERIFY(!journal.CanGoForward());
    QVERIFY(journal.CurrentViewName().isEmpty());
}

void NavigationJournalTests::testNavigateCallback()
{
    NavigationJournal journal;
    QVariantMap receivedParams;

    journal.SetNavigateCallback([&receivedParams](const QString&, const QVariantMap& params)
    {
        receivedParams = params;
    });

    QVariantMap params1;
    params1["key"] = "value1";
    journal.RecordNavigation("View1", params1);

    QVariantMap params2;
    params2["key"] = "value2";
    journal.RecordNavigation("View2", params2);

    journal.GoBack();
    QCOMPARE(receivedParams["key"].toString(), QString("value1"));
}
