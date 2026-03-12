#pragma once

#include <QString>
#include <QVariantMap>

class INavigationJournal
{
public:
    virtual ~INavigationJournal() = default;

    virtual bool CanGoBack() const = 0;
    virtual bool CanGoForward() const = 0;
    virtual void GoBack() = 0;
    virtual void GoForward() = 0;
    virtual void RecordNavigation(const QString& viewName, const QVariantMap& parameters) = 0;
    virtual void Clear() = 0;
    virtual QString CurrentViewName() const = 0;
};
