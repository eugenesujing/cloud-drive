/****************************************************************************
** Meta object code from reading C++ file 'files.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../CloudClient/files.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'files.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Files_t {
    QByteArrayData data[16];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Files_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Files_t qt_meta_stringdata_Files = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Files"
QT_MOC_LITERAL(1, 6, 22), // "on_newFolderPB_clicked"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 18), // "on_freshPB_clicked"
QT_MOC_LITERAL(4, 49, 22), // "switch_to_files_widget"
QT_MOC_LITERAL(5, 72, 7), // "currRow"
QT_MOC_LITERAL(6, 80, 19), // "on_deletePB_clicked"
QT_MOC_LITERAL(7, 100, 19), // "on_renamePB_clicked"
QT_MOC_LITERAL(8, 120, 14), // "double_clicked"
QT_MOC_LITERAL(9, 135, 11), // "QModelIndex"
QT_MOC_LITERAL(10, 147, 5), // "index"
QT_MOC_LITERAL(11, 153, 17), // "on_backPB_clicked"
QT_MOC_LITERAL(12, 171, 19), // "on_uploadPB_clicked"
QT_MOC_LITERAL(13, 191, 11), // "uploadBegin"
QT_MOC_LITERAL(14, 203, 21), // "on_downloadPB_clicked"
QT_MOC_LITERAL(15, 225, 18) // "on_sharePB_clicked"

    },
    "Files\0on_newFolderPB_clicked\0\0"
    "on_freshPB_clicked\0switch_to_files_widget\0"
    "currRow\0on_deletePB_clicked\0"
    "on_renamePB_clicked\0double_clicked\0"
    "QModelIndex\0index\0on_backPB_clicked\0"
    "on_uploadPB_clicked\0uploadBegin\0"
    "on_downloadPB_clicked\0on_sharePB_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Files[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    1,   71,    2, 0x08 /* Private */,
       6,    0,   74,    2, 0x08 /* Private */,
       7,    0,   75,    2, 0x08 /* Private */,
       8,    1,   76,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,
      12,    0,   80,    2, 0x08 /* Private */,
      13,    0,   81,    2, 0x08 /* Private */,
      14,    0,   82,    2, 0x08 /* Private */,
      15,    0,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Files::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Files *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_newFolderPB_clicked(); break;
        case 1: _t->on_freshPB_clicked(); break;
        case 2: _t->switch_to_files_widget((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_deletePB_clicked(); break;
        case 4: _t->on_renamePB_clicked(); break;
        case 5: _t->double_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->on_backPB_clicked(); break;
        case 7: _t->on_uploadPB_clicked(); break;
        case 8: _t->uploadBegin(); break;
        case 9: _t->on_downloadPB_clicked(); break;
        case 10: _t->on_sharePB_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Files::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Files.data,
    qt_meta_data_Files,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Files::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Files::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Files.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Files::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
