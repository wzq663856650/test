#pragma once

#include <QObject>
#include <QTest>

class ModuleCatalogTests : public QObject
{
    Q_OBJECT

private slots:
    void testAddModule();
    void testTopologicalSort();
    void testDuplicateDetection();
    void testMissingDependency();
};
