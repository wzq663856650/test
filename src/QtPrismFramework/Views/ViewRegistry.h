#pragma once

#include "Regions/IViewRegistry.h"
#include <QMap>

class ViewRegistry : public IViewRegistry
{
public:
    ViewRegistry() = default;
    ~ViewRegistry() override = default;

    void RegisterView(const QString& viewName, const QUrl& qmlUrl) override;
    QUrl GetViewUrl(const QString& viewName) const override;
    bool HasView(const QString& viewName) const override;

private:
    QMap<QString, QUrl> m_views;
};
