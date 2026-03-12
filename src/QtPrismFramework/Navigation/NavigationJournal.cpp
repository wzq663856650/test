#include "NavigationJournal.h"

bool NavigationJournal::CanGoBack() const
{
    return !m_backStack.isEmpty();
}

bool NavigationJournal::CanGoForward() const
{
    return !m_forwardStack.isEmpty();
}

void NavigationJournal::GoBack()
{
    if (!CanGoBack())
    {
        return;
    }

    if (m_currentEntry.has_value())
    {
        m_forwardStack.append(m_currentEntry.value());
    }

    m_currentEntry = m_backStack.takeLast();

    if (m_navigateCallback && m_currentEntry.has_value())
    {
        m_navigateCallback(m_currentEntry->viewName, m_currentEntry->parameters);
    }
}

void NavigationJournal::GoForward()
{
    if (!CanGoForward())
    {
        return;
    }

    if (m_currentEntry.has_value())
    {
        m_backStack.append(m_currentEntry.value());
    }

    m_currentEntry = m_forwardStack.takeLast();

    if (m_navigateCallback && m_currentEntry.has_value())
    {
        m_navigateCallback(m_currentEntry->viewName, m_currentEntry->parameters);
    }
}

void NavigationJournal::RecordNavigation(const QString& viewName, const QVariantMap& parameters)
{
    if (m_currentEntry.has_value())
    {
        m_backStack.append(m_currentEntry.value());
    }

    m_currentEntry = JournalEntry{viewName, parameters};
    m_forwardStack.clear();
}

void NavigationJournal::Clear()
{
    m_backStack.clear();
    m_forwardStack.clear();
    m_currentEntry.reset();
}

QString NavigationJournal::CurrentViewName() const
{
    return m_currentEntry.has_value() ? m_currentEntry->viewName : QString();
}
