#pragma once

#include <QObject>
#include <QString>

class IModuleManager : public QObject
{
    Q_OBJECT

public:
    explicit IModuleManager(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IModuleManager() = default;

    virtual void Run() = 0;
    virtual void LoadModule(const QString& moduleName) = 0;

signals:
    void moduleLoaded(const QString& moduleName);
    void moduleLoadFailed(const QString& moduleName, const QString& error);
};
