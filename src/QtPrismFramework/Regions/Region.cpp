#include "Region.h"
#include "Navigation/NavigationContext.h"

#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>

Region::Region(const QString& name, QObject* parent)
    : IRegion(parent)
    , m_name(name)
{
}

QString Region::name() const
{
    return m_name;
}

int Region::viewCount() const
{
    return m_views.size();
}

void Region::AddView(QObject* view, const QString& viewName)
{
    if (!view || m_views.contains(view))
    {
        return;
    }

    m_views.append(view);
    if (!viewName.isEmpty())
    {
        m_namedViews[viewName] = view;
    }
    emit viewsChanged();
}

void Region::RemoveView(QObject* view)
{
    if (!view)
    {
        return;
    }

    if (m_activeView == view)
    {
        m_activeView = nullptr;
        emit activeViewChanged();
    }

    m_views.removeOne(view);

    for (auto it = m_namedViews.begin(); it != m_namedViews.end(); ++it)
    {
        if (it.value() == view)
        {
            m_namedViews.erase(it);
            break;
        }
    }

    emit viewsChanged();
}

void Region::Activate(QObject* view)
{
    if (!view || m_activeView == view)
    {
        return;
    }
    if (!m_views.contains(view))
    {
        return;
    }

    m_activeView = view;
    emit activeViewChanged();
}

void Region::Deactivate(QObject* view)
{
    if (m_activeView == view)
    {
        m_activeView = nullptr;
        emit activeViewChanged();
    }
}

QObject* Region::GetView(const QString& viewName) const
{
    return m_namedViews.value(viewName, nullptr);
}

QObject* Region::activeView() const
{
    return m_activeView;
}

QList<QObject*> Region::activeViews() const
{
    QList<QObject*> list;
    if (m_activeView)
    {
        list.append(m_activeView);
    }
    return list;
}

QList<QObject*> Region::views() const
{
    return m_views;
}

void Region::RequestNavigate(
    const QString& viewName,
    const QVariantMap& parameters,
    std::function<void(bool)> callback)
{
    emit navigating(viewName);
    NavigationContext context(viewName, parameters);

    if (m_activeView)
    {
        auto* confirmNav = dynamic_cast<IConfirmNavigation*>(m_activeView);
        if (confirmNav)
        {
            confirmNav->ConfirmNavigationRequest(context,
                [this, viewName, parameters, callback](bool canNavigate)
                {
                    if (!canNavigate)
                    {
                        if (callback) callback(false);
                        return;
                    }
                    PerformNavigation(viewName, parameters, callback);
                });
            return;
        }
    }

    PerformNavigation(viewName, parameters, callback);
}

void Region::PerformNavigation(
    const QString& viewName,
    const QVariantMap& parameters,
    std::function<void(bool)> callback)
{
    NavigationContext context(viewName, parameters);

    // Notify current active view of NavigatedFrom
    if (m_activeView)
    {
        auto* navAware = dynamic_cast<INavigationAware*>(m_activeView);
        if (navAware)
        {
            navAware->OnNavigatedFrom(context);
        }
    }

    // Check existing views for a reusable target
    QObject* targetView = nullptr;
    auto* existingView = GetView(viewName);
    if (existingView)
    {
        auto* navAware = dynamic_cast<INavigationAware*>(existingView);
        if (!navAware || navAware->IsNavigationTarget(context))
        {
            targetView = existingView;
        }
    }

    // Create new view if needed
    if (!targetView && m_viewRegistry && m_viewRegistry->HasView(viewName))
    {
        QUrl url = m_viewRegistry->GetViewUrl(viewName);
        QQmlEngine* engine = qmlEngine(this);
        if (!engine && !m_views.isEmpty())
        {
            engine = qmlEngine(m_views.first());
        }

        if (engine)
        {
            QQmlComponent component(engine, url);
            if (component.isReady())
            {
                targetView = component.create();
                if (targetView)
                {
                    targetView->setParent(this);
                    AddView(targetView, viewName);
                }
            }
            else
            {
                qWarning() << "[QtPrism] Failed to create view:" << viewName
                           << component.errorString();
            }
        }
    }

    if (!targetView)
    {
        qWarning() << "[QtPrism] View not found for navigation:" << viewName;
        if (callback) callback(false);
        return;
    }

    Activate(targetView);

    if (m_journal)
    {
        m_journal->RecordNavigation(viewName, parameters);
    }

    auto* navAware = dynamic_cast<INavigationAware*>(targetView);
    if (navAware)
    {
        navAware->OnNavigatedTo(context);
    }

    emit navigated(viewName);
    if (callback) callback(true);
}

void Region::SetViewRegistry(std::shared_ptr<IViewRegistry> registry)
{
    m_viewRegistry = std::move(registry);
}

void Region::SetNavigationJournal(std::shared_ptr<INavigationJournal> journal)
{
    m_journal = std::move(journal);
}

std::shared_ptr<INavigationJournal> Region::GetNavigationJournal() const
{
    return m_journal;
}
