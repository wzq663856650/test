#include "ModuleCatalogTests.h"
#include "Modularity/ModuleCatalog.h"
#include <stdexcept>

void ModuleCatalogTests::testAddModule()
{
    ModuleCatalog catalog;
    catalog.AddModule("ModA", "/path/to/a");
    catalog.AddModule("ModB", "/path/to/b");

    QCOMPARE(catalog.Modules().size(), 2);
    QCOMPARE(catalog.Modules()[0].moduleName, QString("ModA"));
    QCOMPARE(catalog.Modules()[1].moduleName, QString("ModB"));
}

void ModuleCatalogTests::testTopologicalSort()
{
    ModuleCatalog catalog;
    catalog.AddModule("ModC", "", InitializationMode::WhenAvailable, {"ModA", "ModB"});
    catalog.AddModule("ModA", "", InitializationMode::WhenAvailable);
    catalog.AddModule("ModB", "", InitializationMode::WhenAvailable, {"ModA"});

    catalog.Initialize();

    auto sorted = catalog.CompleteListWithDependencies(catalog.Modules());
    QCOMPARE(sorted.size(), 3);
    QCOMPARE(sorted[0].moduleName, QString("ModA"));
    QCOMPARE(sorted[1].moduleName, QString("ModB"));
    QCOMPARE(sorted[2].moduleName, QString("ModC"));
}

void ModuleCatalogTests::testDuplicateDetection()
{
    ModuleCatalog catalog;
    catalog.AddModule("ModA", "/a");
    catalog.AddModule("ModA", "/b");

    bool threw = false;
    try
    {
        catalog.Initialize();
    }
    catch (const std::runtime_error&)
    {
        threw = true;
    }
    QVERIFY(threw);
}

void ModuleCatalogTests::testMissingDependency()
{
    ModuleCatalog catalog;
    catalog.AddModule("ModA", "", InitializationMode::WhenAvailable, {"NonExistent"});

    bool threw = false;
    try
    {
        catalog.Initialize();
    }
    catch (const std::runtime_error&)
    {
        threw = true;
    }
    QVERIFY(threw);
}
