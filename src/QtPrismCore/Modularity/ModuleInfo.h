#pragma once

#include <QString>
#include <QStringList>

enum class InitializationMode
{
    WhenAvailable,
    OnDemand
};

enum class ModuleState
{
    NotStarted,
    ReadyForInitialization,
    Initializing,
    Initialized
};

struct ModuleInfo
{
    QString moduleName;
    QString ref;
    QStringList dependsOn;
    InitializationMode initMode = InitializationMode::WhenAvailable;
    ModuleState state = ModuleState::NotStarted;
};
