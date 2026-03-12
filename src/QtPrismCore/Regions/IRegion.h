#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <functional>

class IRegion : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(int viewCount READ viewCount NOTIFY viewsChanged)

public:
    explicit IRegion(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IRegion() = default;

    virtual QString name() const = 0;
    virtual int viewCount() const = 0;

    virtual void AddView(QObject* view, const QString& viewName = {}) = 0;
    virtual void RemoveView(QObject* view) = 0;
    virtual void Activate(QObject* view) = 0;
    virtual void Deactivate(QObject* view) = 0;
    virtual QObject* GetView(const QString& viewName) const = 0;

    virtual QObject* activeView() const = 0;
    virtual QList<QObject*> activeViews() const = 0;
    virtual QList<QObject*> views() const = 0;

    virtual void RequestNavigate(
        const QString& viewName,
        const QVariantMap& parameters = {},
        std::function<void(bool)> callback = nullptr) = 0;

signals:
    void viewsChanged();
    void activeViewChanged();
    void navigating(const QString& viewName);
    void navigated(const QString& viewName);
};
