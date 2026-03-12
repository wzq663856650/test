#pragma once

#include "Regions/IRegion.h"
#include "Navigation/INavigationAware.h"
#include "Navigation/IConfirmNavigation.h"
#include "Navigation/INavigationJournal.h"
#include "Regions/IViewRegistry.h"

#include <QMap>
#include <memory>

class Region : public IRegion
{
    Q_OBJECT

public:
    explicit Region(const QString& name, QObject* parent = nullptr);
    ~Region() override = default;

    QString name() const override;
    int viewCount() const override;

    void AddView(QObject* view, const QString& viewName = {}) override;
    void RemoveView(QObject* view) override;
    void Activate(QObject* view) override;
    void Deactivate(QObject* view) override;
    QObject* GetView(const QString& viewName) const override;

    QObject* activeView() const override;
    QList<QObject*> activeViews() const override;
    QList<QObject*> views() const override;

    void RequestNavigate(
        const QString& viewName,
        const QVariantMap& parameters = {},
        std::function<void(bool)> callback = nullptr) override;

    void SetViewRegistry(std::shared_ptr<IViewRegistry> registry);
    void SetNavigationJournal(std::shared_ptr<INavigationJournal> journal);
    std::shared_ptr<INavigationJournal> GetNavigationJournal() const;

private:
    void PerformNavigation(const QString& viewName,
                           const QVariantMap& parameters,
                           std::function<void(bool)> callback);

    QString m_name;
    QList<QObject*> m_views;
    QMap<QString, QObject*> m_namedViews;
    QObject* m_activeView = nullptr;
    std::shared_ptr<IViewRegistry> m_viewRegistry;
    std::shared_ptr<INavigationJournal> m_journal;
};
