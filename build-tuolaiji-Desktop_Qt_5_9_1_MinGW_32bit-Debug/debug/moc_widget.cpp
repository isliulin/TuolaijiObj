/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtApptuolaiji/widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WorkObject_t {
    QByteArrayData data[9];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WorkObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WorkObject_t qt_meta_stringdata_WorkObject = {
    {
QT_MOC_LITERAL(0, 0, 10), // "WorkObject"
QT_MOC_LITERAL(1, 11, 7), // "SendLog"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 9), // "StartWork"
QT_MOC_LITERAL(4, 30, 7), // "xmlfile"
QT_MOC_LITERAL(5, 38, 11), // "setPortName"
QT_MOC_LITERAL(6, 50, 4), // "name"
QT_MOC_LITERAL(7, 55, 11), // "setBaudRate"
QT_MOC_LITERAL(8, 67, 8) // "baudRate"

    },
    "WorkObject\0SendLog\0\0StartWork\0xmlfile\0"
    "setPortName\0name\0setBaudRate\0baudRate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WorkObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   37,    2, 0x0a /* Public */,
       5,    1,   40,    2, 0x0a /* Public */,
       7,    1,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void WorkObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WorkObject *_t = static_cast<WorkObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SendLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->StartWork((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->setPortName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setBaudRate((*reinterpret_cast< qint32(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WorkObject::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WorkObject::SendLog)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject WorkObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WorkObject.data,
      qt_meta_data_WorkObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WorkObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WorkObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WorkObject.stringdata0))
        return static_cast<void*>(const_cast< WorkObject*>(this));
    return QObject::qt_metacast(_clname);
}

int WorkObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void WorkObject::SendLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CSimpleThread_t {
    QByteArrayData data[8];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSimpleThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSimpleThread_t qt_meta_stringdata_CSimpleThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CSimpleThread"
QT_MOC_LITERAL(1, 14, 11), // "resultReady"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "result"
QT_MOC_LITERAL(4, 34, 17), // "SendTotalProgress"
QT_MOC_LITERAL(5, 52, 5), // "value"
QT_MOC_LITERAL(6, 58, 18), // "SendSingleProgress"
QT_MOC_LITERAL(7, 77, 7) // "SendLog"

    },
    "CSimpleThread\0resultReady\0\0result\0"
    "SendTotalProgress\0value\0SendSingleProgress\0"
    "SendLog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSimpleThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,
       7,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void CSimpleThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSimpleThread *_t = static_cast<CSimpleThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resultReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->SendTotalProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SendSingleProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SendLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CSimpleThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSimpleThread::resultReady)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CSimpleThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSimpleThread::SendTotalProgress)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CSimpleThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSimpleThread::SendSingleProgress)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CSimpleThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CSimpleThread::SendLog)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject CSimpleThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CSimpleThread.data,
      qt_meta_data_CSimpleThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CSimpleThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSimpleThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CSimpleThread.stringdata0))
        return static_cast<void*>(const_cast< CSimpleThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CSimpleThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CSimpleThread::resultReady(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CSimpleThread::SendTotalProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CSimpleThread::SendSingleProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CSimpleThread::SendLog(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_Widget_t {
    QByteArrayData data[10];
    char stringdata0[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Widget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Widget_t qt_meta_stringdata_Widget = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Widget"
QT_MOC_LITERAL(1, 7, 8), // "writeLog"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 5), // "value"
QT_MOC_LITERAL(4, 23, 15), // "ComCheckConnect"
QT_MOC_LITERAL(5, 39, 11), // "RecviceData"
QT_MOC_LITERAL(6, 51, 17), // "SlotTotalProgress"
QT_MOC_LITERAL(7, 69, 18), // "SlotSingleProgress"
QT_MOC_LITERAL(8, 88, 29), // "on_pushButtonOpenFile_clicked"
QT_MOC_LITERAL(9, 118, 26) // "on_pushButtonStart_clicked"

    },
    "Widget\0writeLog\0\0value\0ComCheckConnect\0"
    "RecviceData\0SlotTotalProgress\0"
    "SlotSingleProgress\0on_pushButtonOpenFile_clicked\0"
    "on_pushButtonStart_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Widget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       4,    0,   52,    2, 0x0a /* Public */,
       5,    0,   53,    2, 0x0a /* Public */,
       6,    1,   54,    2, 0x08 /* Private */,
       7,    1,   57,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Widget *_t = static_cast<Widget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->writeLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ComCheckConnect(); break;
        case 2: _t->RecviceData(); break;
        case 3: _t->SlotTotalProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->SlotSingleProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_pushButtonOpenFile_clicked(); break;
        case 6: _t->on_pushButtonStart_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget.data,
      qt_meta_data_Widget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Widget.stringdata0))
        return static_cast<void*>(const_cast< Widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
