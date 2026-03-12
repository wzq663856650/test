#pragma once

#include <QObject>

class ViewModelBase : public QObject
{
    Q_OBJECT

public:
    explicit ViewModelBase(QObject* parent = nullptr);
    ~ViewModelBase() override = default;
};

#define QTPRISM_PROPERTY_DEF(type, name, initialValue)                                          \
    Q_PROPERTY(type name READ name WRITE set_##name NOTIFY name##Changed)                       \
public:                                                                                         \
    type name() const { return m_##name; }                                                      \
    void set_##name(const type& value)                                                          \
    {                                                                                           \
        if (m_##name != value)                                                                  \
        {                                                                                       \
            m_##name = value;                                                                   \
            emit name##Changed();                                                               \
        }                                                                                       \
    }                                                                                           \
Q_SIGNALS:                                                                                      \
    void name##Changed();                                                                       \
private:                                                                                        \
    type m_##name = initialValue;
