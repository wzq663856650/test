/****************************************************************************
** Meta object code from reading C++ file 'OrderDetailViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../samples/AdminApp/Modules/OrderModule/ViewModels/OrderDetailViewModel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OrderDetailViewModel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_OrderDetailViewModel_t {
    uint offsetsAndSizes[16];
    char stringdata0[21];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[8];
    char stringdata5[13];
    char stringdata6[6];
    char stringdata7[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_OrderDetailViewModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_OrderDetailViewModel_t qt_meta_stringdata_OrderDetailViewModel = {
    {
        QT_MOC_LITERAL(0, 20),  // "OrderDetailViewModel"
        QT_MOC_LITERAL(21, 12),  // "orderChanged"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 6),  // "goBack"
        QT_MOC_LITERAL(42, 7),  // "orderId"
        QT_MOC_LITERAL(50, 12),  // "customerName"
        QT_MOC_LITERAL(63, 5),  // "total"
        QT_MOC_LITERAL(69, 6)   // "status"
    },
    "OrderDetailViewModel",
    "orderChanged",
    "",
    "goBack",
    "orderId",
    "customerName",
    "total",
    "status"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_OrderDetailViewModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       4,   28, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x06,    5 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   27,    2, 0x02,    6 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::Int, 0x00015001, uint(0), 0,
       5, QMetaType::QString, 0x00015001, uint(0), 0,
       6, QMetaType::Double, 0x00015001, uint(0), 0,
       7, QMetaType::QString, 0x00015001, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject OrderDetailViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<ViewModelBase::staticMetaObject>(),
    qt_meta_stringdata_OrderDetailViewModel.offsetsAndSizes,
    qt_meta_data_OrderDetailViewModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_OrderDetailViewModel_t,
        // property 'orderId'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'customerName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'total'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'status'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OrderDetailViewModel, std::true_type>,
        // method 'orderChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goBack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void OrderDetailViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrderDetailViewModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->orderChanged(); break;
        case 1: _t->goBack(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrderDetailViewModel::*)();
            if (_t _q_method = &OrderDetailViewModel::orderChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<OrderDetailViewModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->orderId(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->customerName(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->total(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->status(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_a;
}

const QMetaObject *OrderDetailViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderDetailViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrderDetailViewModel.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "INavigationAware"))
        return static_cast< INavigationAware*>(this);
    return ViewModelBase::qt_metacast(_clname);
}

int OrderDetailViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ViewModelBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void OrderDetailViewModel::orderChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
