#pragma once

#include "Mvvm/ViewModelBase.h"
#include <QString>

class DashboardViewModel : public ViewModelBase
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString summary READ summary NOTIFY summaryChanged)

public:
    explicit DashboardViewModel(QObject* parent = nullptr);

    QString title() const { return m_title; }
    QString summary() const { return m_summary; }

signals:
    void titleChanged();
    void summaryChanged();

private:
    QString m_title = "Dashboard";
    QString m_summary = "Welcome to QtPrism Admin Application";
};
