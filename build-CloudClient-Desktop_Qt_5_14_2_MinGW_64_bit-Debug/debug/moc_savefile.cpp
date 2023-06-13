/****************************************************************************
** Meta object code from reading C++ file 'savefile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../CloudClient/savefile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'savefile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SaveFile_t {
    QByteArrayData data[9];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SaveFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SaveFile_t qt_meta_stringdata_SaveFile = {
    {
QT_MOC_LITERAL(0, 0, 8), // "SaveFile"
QT_MOC_LITERAL(1, 9, 14), // "double_clicked"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 37, 5), // "index"
QT_MOC_LITERAL(5, 43, 15), // "on_back_clicked"
QT_MOC_LITERAL(6, 59, 17), // "on_cancel_clicked"
QT_MOC_LITERAL(7, 77, 20), // "on_newFolder_clicked"
QT_MOC_LITERAL(8, 98, 18) // "on_confirm_clicked"

    },
    "SaveFile\0double_clicked\0\0QModelIndex\0"
    "index\0on_back_clicked\0on_cancel_clicked\0"
    "on_newFolder_clicked\0on_confirm_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SaveFile[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,
       7,    0,   44,    2, 0x08 /* Private */,
       8,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SaveFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SaveFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->double_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->on_back_clicked(); break;
        case 2: _t->on_cancel_clicked(); break;
        case 3: _t->on_newFolder_clicked(); break;
        case 4: _t->on_confirm_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SaveFile::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SaveFile.data,
    qt_meta_data_SaveFile,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SaveFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SaveFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SaveFile.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SaveFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
