#pragma once

#include "QtPrismApplication.h"

class ShellController;

class App : public QtPrismApplication
{
    Q_OBJECT

public:
    explicit App(QGuiApplication* app, QQmlApplicationEngine* engine,
                 QObject* parent = nullptr);

protected:
    void RegisterTypes(IContainerRegistry* registry) override;
    void ConfigureModuleCatalog(IModuleCatalog* catalog) override;
    QUrl CreateShell() override;
    void OnInitialized() override;

private:
    QString pluginDir() const;
    QString pluginFileName(const QString& baseName) const;

    ShellController* m_shellController = nullptr;
};
