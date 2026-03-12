/****************************************************************************
** Meta object code from reading C++ file 'ContainerTests.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../tests/ContainerTests.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ContainerTests.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ContainerTests_t {
    uint offsetsAndSizes[16];
    char stringdata0[15];
    char stringdata1[32];
    char stringdata2[1];
    char stringdata3[32];
    char stringdata4[21];
    char stringdata5[22];
    char stringdata6[17];
    char stringdata7[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ContainerTests_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ContainerTests_t qt_meta_stringdata_ContainerTests = {
    {
        QT_MOC_LITERAL(0, 14),  // "ContainerTests"
        QT_MOC_LITERAL(15, 31),  // "testRegisterAndResolveSingleton"
        QT_MOC_LITERAL(47, 0),  // ""
        QT_MOC_LITERAL(48, 31),  // "testRegisterAndResolveTransient"
        QT_MOC_LITERAL(80, 20),  // "testRegisterInstance"
        QT_MOC_LITERAL(101, 21),  // "testNamedRegistration"
        QT_MOC_LITERAL(123, 16),  // "testIsRegistered"
        QT_MOC_LITERAL(140, 23)   // "testResolveUnregistered"
    },
    "ContainerTests",
    "testRegisterAndResolveSingleton",
    "",
    "testRegisterAndResolveTransient",
    "testRegisterInstance",
    "testNamedRegistration",
    "testIsRegistered",
    "testResolveUnregistered"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ContainerTests[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    0,   51,    2, 0x08,    2 /* Private */,
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    0,   54,    2, 0x08,    5 /* Private */,
       7,    0,   55,    2, 0x08,    6 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ContainerTests::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ContainerTests.offsetsAndSizes,
    qt_meta_data_ContainerTests,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ContainerTests_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ContainerTests, std::true_type>,
        // method 'testRegisterAndResolveSingleton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testRegisterAndResolveTransient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testRegisterInstance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testNamedRegistration'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testIsRegistered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testResolveUnregistered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ContainerTests::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ContainerTests *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->testRegisterAndResolveSingleton(); break;
        case 1: _t->testRegisterAndResolveTransient(); break;
        case 2: _t->testRegisterInstance(); break;
        case 3: _t->testNamedRegistration(); break;
        case 4: _t->testIsRegistered(); break;
        case 5: _t->testResolveUnregistered(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *ContainerTests::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContainerTests::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ContainerTests.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ContainerTests::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
