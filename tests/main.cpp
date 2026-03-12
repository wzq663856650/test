#include <QCoreApplication>
#include <QDebug>
#include <QTest>

#include "ContainerTests.h"
#include "EventAggregatorTests.h"
#include "ModuleCatalogTests.h"
#include "RegionTests.h"
#include "NavigationJournalTests.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    int status = 0;
    qDebug() << "\n======== QtPrism Unit Tests ========\n";

    {
        ContainerTests t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        EventAggregatorTests t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        ModuleCatalogTests t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        RegionTests t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        NavigationJournalTests t;
        status |= QTest::qExec(&t, argc, argv);
    }

    qDebug() << "\n======== Tests Complete ========";
    return status;
}
