#pragma once

#include <QObject>
#include <QTest>

class RegionTests : public QObject
{
    Q_OBJECT

private slots:
    void testAddView();
    void testActivateDeactivate();
    void testRemoveView();
    void testNamedView();
    void testRegionManager();
};
