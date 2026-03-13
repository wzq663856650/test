#pragma once

#include "Navigation/INavigationJournal.h"
#include <QList>
#include <optional>

class NavigationJournal : public INavigationJournal
{
public:
    NavigationJournal() = default;
    ~NavigationJournal() override = default;

    bool CanGoBack() const override;
    bool CanGoForward() const override;
    void GoBack() override;
    void GoForward() override;
    void RecordNavigation(const QString& viewName, const QVariantMap& parameters) override;
    void Clear() override;
    QString CurrentViewName() const override;

    struct JournalEntry
    {
        QString viewName;
        QVariantMap parameters;
    };

    const QList<JournalEntry>& BackStack() const { return m_backStack; }
    const QList<JournalEntry>& ForwardStack() const { return m_forwardStack; }

    using NavigateCallback = std::function<void(const QString&, const QVariantMap&)>;
    void SetNavigateCallback(NavigateCallback cb) { m_navigateCallback = std::move(cb); }

private:
    QList<JournalEntry> m_backStack;
    QList<JournalEntry> m_forwardStack;
    std::optional<JournalEntry> m_currentEntry;
    NavigateCallback m_navigateCallback;
};
