/****************************************************************************
** Meta object code from reading C++ file 'ModuleCatalogTests.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../tests/ModuleCatalogTests.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModuleCatalogTests.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ModuleCatalogTests_t {
    uint offsetsAndSizes[12];
    char stringdata0[19];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[23];
    char stringdata5[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ModuleCatalogTests_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ModuleCatalogTests_t qt_meta_stringdata_ModuleCatalogTests = {
    {
        QT_MOC_LITERAL(0, 18),  // "ModuleCatalogTests"
        QT_MOC_LITERAL(19, 13),  // "testAddModule"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 19),  // "testTopologicalSort"
        QT_MOC_LITERAL(54, 22),  // "testDuplicateDetection"
        QT_MOC_LITERAL(77, 21)   // "testMissingDependency"
    },
    "ModuleCatalogTests",
    "testAddModule",
    "",
    "testTopologicalSort",
    "testDuplicateDetection",
    "testMissingDependency"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ModuleCatalogTests[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x08,    1 /* Private */,
       3,    0,   39,    2, 0x08,    2 /* Private */,
       4,    0,   40,    2, 0x08,    3 /* Private */,
       5,    0,   41,    2, 0x08,    4 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ModuleCatalogTests::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ModuleCatalogTests.offsetsAndSizes,
    qt_meta_data_ModuleCatalogTests,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ModuleCatalogTests_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ModuleCatalogTests, std::true_type>,
        // method 'testAddModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testTopologicalSort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testDuplicateDetection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testMissingDependency'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ModuleCatalogTests::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModuleCatalogTests *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->testAddModule(); break;
        case 1: _t->testTopologicalSort(); break;
        case 2: _t->testDuplicateDetection(); break;
        case 3: _t->testMissingDependency(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *ModuleCatalogTests::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModuleCatalogTests::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModuleCatalogTests.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ModuleCatalogTests::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
