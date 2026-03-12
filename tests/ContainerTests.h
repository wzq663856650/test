#pragma once

#include <QObject>
#include <QTest>

class ContainerTests : public QObject
{
    Q_OBJECT

private slots:
    void testRegisterAndResolveSingleton();
    void testRegisterAndResolveTransient();
    void testRegisterInstance();
    void testNamedRegistration();
    void testIsRegistered();
    void testResolveUnregistered();
};
