#pragma once

#include <QString>
#include <QVariantMap>
#include <QVariant>

class NavigationContext
{
public:
    NavigationContext() = default;
    NavigationContext(const QString& viewName, const QVariantMap& parameters);

    QString viewName() const;
    QVariantMap parameters() const;

    template<typename T>
    T GetParameter(const QString& key, const T& defaultValue = T()) const
    {
        auto it = m_parameters.find(key);
        if (it != m_parameters.end())
        {
            return it->value<T>();
        }
        return defaultValue;
    }

private:
    QString m_viewName;
    QVariantMap m_parameters;
};
