#include "RegionTests.h"
#include "Regions/Region.h"
#include "Regions/RegionManager.h"

void RegionTests::testAddView()
{
    Region region("TestRegion");
    QObject view1, view2;

    region.AddView(&view1, "View1");
    QCOMPARE(region.viewCount(), 1);

    region.AddView(&view2, "View2");
    QCOMPARE(region.viewCount(), 2);

    // Duplicate should not add
    region.AddView(&view1, "View1");
    QCOMPARE(region.viewCount(), 2);
}

void RegionTests::testActivateDeactivate()
{
    Region region("TestRegion");
    QObject view1, view2;

    region.AddView(&view1, "View1");
    region.AddView(&view2, "View2");

    QVERIFY(region.activeView() == nullptr);

    region.Activate(&view1);
    QCOMPARE(region.activeView(), &view1);

    region.Activate(&view2);
    QCOMPARE(region.activeView(), &view2);

    region.Deactivate(&view2);
    QVERIFY(region.activeView() == nullptr);
}

void RegionTests::testRemoveView()
{
    Region region("TestRegion");
    QObject view1;

    region.AddView(&view1, "View1");
    region.Activate(&view1);
    QCOMPARE(region.activeView(), &view1);

    region.RemoveView(&view1);
    QCOMPARE(region.viewCount(), 0);
    QVERIFY(region.activeView() == nullptr);
}

void RegionTests::testNamedView()
{
    Region region("TestRegion");
    QObject view1;

    region.AddView(&view1, "MyView");
    QCOMPARE(region.GetView("MyView"), &view1);
    QVERIFY(region.GetView("NonExistent") == nullptr);
}

void RegionTests::testRegionManager()
{
    RegionManager manager;
    auto region1 = std::make_shared<Region>("Region1");
    auto region2 = std::make_shared<Region>("Region2");

    manager.AddRegion("Region1", region1);
    manager.AddRegion("Region2", region2);

    QVERIFY(manager.ContainsRegion("Region1"));
    QVERIFY(manager.ContainsRegion("Region2"));
    QVERIFY(!manager.ContainsRegion("Region3"));

    QCOMPARE(manager.RegionNames().size(), 2);

    manager.RegisterViewWithRegion("Region1", "ViewA");
    manager.RegisterViewWithRegion("Region1", "ViewB");
    QCOMPARE(manager.GetViewsForRegion("Region1").size(), 2);

    manager.RemoveRegion("Region1");
    QVERIFY(!manager.ContainsRegion("Region1"));
}
