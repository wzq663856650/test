#include "ViewRegistry.h"
#include <QDebug>

void ViewRegistry::RegisterView(const QString& viewName, const QUrl& qmlUrl)
{
    m_views[viewName] = qmlUrl;
    qDebug() << "[QtPrism] View registered:" << viewName << "->" << qmlUrl;
}

QUrl ViewRegistry::GetViewUrl(const QString& viewName) const
{
    return m_views.value(viewName);
}

bool ViewRegistry::HasView(const QString& viewName) const
{
    return m_views.contains(viewName);
}
