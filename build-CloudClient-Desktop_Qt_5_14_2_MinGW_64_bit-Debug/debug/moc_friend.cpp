/****************************************************************************
** Meta object code from reading C++ file 'friend.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../CloudClient/friend.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'friend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Friend_t {
    QByteArrayData data[9];
    char stringdata0[164];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Friend_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Friend_t qt_meta_stringdata_Friend = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Friend"
QT_MOC_LITERAL(1, 7, 10), // "showOnline"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 23), // "on_onlineButton_clicked"
QT_MOC_LITERAL(4, 43, 23), // "on_searchButton_clicked"
QT_MOC_LITERAL(5, 67, 24), // "on_refreshButton_clicked"
QT_MOC_LITERAL(6, 92, 23), // "on_deleteButton_clicked"
QT_MOC_LITERAL(7, 116, 24), // "on_messageButton_clicked"
QT_MOC_LITERAL(8, 141, 22) // "on_broadcastPB_clicked"

    },
    "Friend\0showOnline\0\0on_onlineButton_clicked\0"
    "on_searchButton_clicked\0"
    "on_refreshButton_clicked\0"
    "on_deleteButton_clicked\0"
    "on_messageButton_clicked\0"
    "on_broadcastPB_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Friend[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Friend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Friend *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showOnline(); break;
        case 1: _t->on_onlineButton_clicked(); break;
        case 2: _t->on_searchButton_clicked(); break;
        case 3: _t->on_refreshButton_clicked(); break;
        case 4: _t->on_deleteButton_clicked(); break;
        case 5: _t->on_messageButton_clicked(); break;
        case 6: _t->on_broadcastPB_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Friend::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Friend.data,
    qt_meta_data_Friend,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Friend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Friend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Friend.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Friend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
