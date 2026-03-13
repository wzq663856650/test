#include "NavigationContext.h"

NavigationContext::NavigationContext(const QString& viewName, const QVariantMap& parameters)
    : m_viewName(viewName)
    , m_parameters(parameters)
{
}

QString NavigationContext::viewName() const
{
    return m_viewName;
}

QVariantMap NavigationContext::parameters() const
{
    return m_parameters;
}
