#pragma once

#include <QObject>
#include <QString>
#include <QUrl>

class IViewRegistry
{
public:
    virtual ~IViewRegistry() = default;
    virtual void RegisterView(const QString& viewName, const QUrl& qmlUrl) = 0;
    virtual QUrl GetViewUrl(const QString& viewName) const = 0;
    virtual bool HasView(const QString& viewName) const = 0;
};
