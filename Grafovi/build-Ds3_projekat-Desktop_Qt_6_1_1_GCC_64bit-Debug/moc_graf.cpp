/****************************************************************************
** Meta object code from reading C++ file 'graf.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Ds3_grafovi_projekat/Graf/graf.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graf.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Graf_t {
    const uint offsetsAndSize[18];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Graf_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Graf_t qt_meta_stringdata_Graf = {
    {
QT_MOC_LITERAL(0, 4), // "Graf"
QT_MOC_LITERAL(5, 9), // "sig_nista"
QT_MOC_LITERAL(15, 0), // ""
QT_MOC_LITERAL(16, 8), // "sig_cvor"
QT_MOC_LITERAL(25, 5), // "Cvor*"
QT_MOC_LITERAL(31, 4), // "cvor"
QT_MOC_LITERAL(36, 9), // "sig_ivica"
QT_MOC_LITERAL(46, 6), // "Ivica*"
QT_MOC_LITERAL(53, 2) // "iv"

    },
    "Graf\0sig_nista\0\0sig_cvor\0Cvor*\0cvor\0"
    "sig_ivica\0Ivica*\0iv"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Graf[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    0 /* Public */,
       3,    1,   33,    2, 0x06,    1 /* Public */,
       6,    1,   36,    2, 0x06,    3 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void Graf::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Graf *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sig_nista(); break;
        case 1: _t->sig_cvor((*reinterpret_cast< Cvor*(*)>(_a[1]))); break;
        case 2: _t->sig_ivica((*reinterpret_cast< Ivica*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Graf::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graf::sig_nista)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Graf::*)(Cvor * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graf::sig_cvor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Graf::*)(Ivica * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Graf::sig_ivica)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Graf::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_meta_stringdata_Graf.offsetsAndSize,
    qt_meta_data_Graf,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Graf_t
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Ivica *, std::false_type>



>,
    nullptr
} };


const QMetaObject *Graf::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Graf::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Graf.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int Graf::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Graf::sig_nista()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Graf::sig_cvor(Cvor * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Graf::sig_ivica(Ivica * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_Algoritam_t {
    const uint offsetsAndSize[66];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Algoritam_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Algoritam_t qt_meta_stringdata_Algoritam = {
    {
QT_MOC_LITERAL(0, 9), // "Algoritam"
QT_MOC_LITERAL(10, 9), // "boji_cvor"
QT_MOC_LITERAL(20, 0), // ""
QT_MOC_LITERAL(21, 5), // "Cvor*"
QT_MOC_LITERAL(27, 2), // "cv"
QT_MOC_LITERAL(30, 3), // "col"
QT_MOC_LITERAL(34, 14), // "anim_boji_cvor"
QT_MOC_LITERAL(49, 4), // "col1"
QT_MOC_LITERAL(54, 4), // "col2"
QT_MOC_LITERAL(59, 5), // "param"
QT_MOC_LITERAL(65, 11), // "okviri_cvor"
QT_MOC_LITERAL(77, 2), // "rc"
QT_MOC_LITERAL(80, 3), // "len"
QT_MOC_LITERAL(84, 10), // "boji_ivica"
QT_MOC_LITERAL(95, 6), // "Ivica*"
QT_MOC_LITERAL(102, 2), // "iv"
QT_MOC_LITERAL(105, 16), // "stavi_tekst_cvor"
QT_MOC_LITERAL(122, 3), // "txt"
QT_MOC_LITERAL(126, 17), // "stavi_tekst_ivica"
QT_MOC_LITERAL(144, 14), // "animiraj_ivicu"
QT_MOC_LITERAL(159, 1), // "p"
QT_MOC_LITERAL(161, 1), // "d"
QT_MOC_LITERAL(163, 15), // "transp_cvor_sve"
QT_MOC_LITERAL(179, 1), // "c"
QT_MOC_LITERAL(181, 3), // "val"
QT_MOC_LITERAL(185, 12), // "transp_ivicu"
QT_MOC_LITERAL(198, 14), // "kraj_animacije"
QT_MOC_LITERAL(213, 9), // "error_box"
QT_MOC_LITERAL(223, 10), // "dodaj_text"
QT_MOC_LITERAL(234, 15), // "dodaj_anim_cvor"
QT_MOC_LITERAL(250, 16), // "dodaj_anim_ivicu"
QT_MOC_LITERAL(267, 1), // "i"
QT_MOC_LITERAL(269, 11) // "pomeri_cvor"

    },
    "Algoritam\0boji_cvor\0\0Cvor*\0cv\0col\0"
    "anim_boji_cvor\0col1\0col2\0param\0"
    "okviri_cvor\0rc\0len\0boji_ivica\0Ivica*\0"
    "iv\0stavi_tekst_cvor\0txt\0stavi_tekst_ivica\0"
    "animiraj_ivicu\0p\0d\0transp_cvor_sve\0c\0"
    "val\0transp_ivicu\0kraj_animacije\0"
    "error_box\0dodaj_text\0dodaj_anim_cvor\0"
    "dodaj_anim_ivicu\0i\0pomeri_cvor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Algoritam[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,  104,    2, 0x06,    0 /* Public */,
       6,    4,  109,    2, 0x06,    3 /* Public */,
      10,    4,  118,    2, 0x06,    8 /* Public */,
      13,    2,  127,    2, 0x06,   13 /* Public */,
      16,    2,  132,    2, 0x06,   16 /* Public */,
      18,    2,  137,    2, 0x06,   19 /* Public */,
      19,    5,  142,    2, 0x06,   22 /* Public */,
      22,    2,  153,    2, 0x06,   28 /* Public */,
      25,    2,  158,    2, 0x06,   31 /* Public */,
      26,    0,  163,    2, 0x06,   34 /* Public */,
      27,    1,  164,    2, 0x06,   35 /* Public */,
      28,    1,  167,    2, 0x06,   37 /* Public */,
      29,    1,  170,    2, 0x06,   39 /* Public */,
      30,    1,  173,    2, 0x06,   41 /* Public */,
      32,    2,  176,    2, 0x06,   43 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QColor,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QColor, QMetaType::QColor, QMetaType::Float,    4,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QRectF, QMetaType::Float, QMetaType::Float,    4,   11,   12,    9,
    QMetaType::Void, 0x80000000 | 14, QMetaType::QColor,   15,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,   17,
    QMetaType::Void, 0x80000000 | 14, QMetaType::QString,   15,   17,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 3, 0x80000000 | 3, QMetaType::QColor, QMetaType::Float,   15,   20,   21,    5,    9,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float,   23,   24,
    QMetaType::Void, 0x80000000 | 14, QMetaType::Float,   15,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, 0x80000000 | 3,   23,
    QMetaType::Void, 0x80000000 | 14,   31,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QPointF,   23,   20,

       0        // eod
};

void Algoritam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Algoritam *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->boji_cvor((*reinterpret_cast< Cvor*(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 1: _t->anim_boji_cvor((*reinterpret_cast< Cvor*(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2])),(*reinterpret_cast< QColor(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 2: _t->okviri_cvor((*reinterpret_cast< Cvor*(*)>(_a[1])),(*reinterpret_cast< QRectF(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 3: _t->boji_ivica((*reinterpret_cast< Ivica*(*)>(_a[1])),(*reinterpret_cast< QColor(*)>(_a[2]))); break;
        case 4: _t->stavi_tekst_cvor((*reinterpret_cast< Cvor*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->stavi_tekst_ivica((*reinterpret_cast< Ivica*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->animiraj_ivicu((*reinterpret_cast< Ivica*(*)>(_a[1])),(*reinterpret_cast< Cvor*(*)>(_a[2])),(*reinterpret_cast< Cvor*(*)>(_a[3])),(*reinterpret_cast< QColor(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5]))); break;
        case 7: _t->transp_cvor_sve((*reinterpret_cast< Cvor*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 8: _t->transp_ivicu((*reinterpret_cast< Ivica*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 9: _t->kraj_animacije(); break;
        case 10: _t->error_box((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->dodaj_text((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->dodaj_anim_cvor((*reinterpret_cast< Cvor*(*)>(_a[1]))); break;
        case 13: _t->dodaj_anim_ivicu((*reinterpret_cast< Ivica*(*)>(_a[1]))); break;
        case 14: _t->pomeri_cvor((*reinterpret_cast< Cvor*(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Algoritam::*)(Cvor * , QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::boji_cvor)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Cvor * , QColor , QColor , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::anim_boji_cvor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Cvor * , QRectF , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::okviri_cvor)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Ivica * , QColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::boji_ivica)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Cvor * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::stavi_tekst_cvor)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Ivica * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::stavi_tekst_ivica)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Ivica * , Cvor * , Cvor * , QColor , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::animiraj_ivicu)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Cvor * , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::transp_cvor_sve)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Ivica * , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::transp_ivicu)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::kraj_animacije)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::error_box)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::dodaj_text)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Cvor * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::dodaj_anim_cvor)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Ivica * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::dodaj_anim_ivicu)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Algoritam::*)(Cvor * , QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Algoritam::pomeri_cvor)) {
                *result = 14;
                return;
            }
        }
    }
}

const QMetaObject Algoritam::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_Algoritam.offsetsAndSize,
    qt_meta_data_Algoritam,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Algoritam_t
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<QColor, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<QColor, std::false_type>, QtPrivate::TypeAndForceComplete<QColor, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<QRectF, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Ivica *, std::false_type>, QtPrivate::TypeAndForceComplete<QColor, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Ivica *, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Ivica *, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<QColor, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Ivica *, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Ivica *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<Cvor *, std::false_type>, QtPrivate::TypeAndForceComplete<QPointF, std::false_type>



>,
    nullptr
} };


const QMetaObject *Algoritam::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Algoritam::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Algoritam.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Algoritam::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Algoritam::boji_cvor(Cvor * _t1, QColor _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Algoritam::anim_boji_cvor(Cvor * _t1, QColor _t2, QColor _t3, float _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Algoritam::okviri_cvor(Cvor * _t1, QRectF _t2, float _t3, float _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Algoritam::boji_ivica(Ivica * _t1, QColor _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Algoritam::stavi_tekst_cvor(Cvor * _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Algoritam::stavi_tekst_ivica(Ivica * _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Algoritam::animiraj_ivicu(Ivica * _t1, Cvor * _t2, Cvor * _t3, QColor _t4, float _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Algoritam::transp_cvor_sve(Cvor * _t1, float _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Algoritam::transp_ivicu(Ivica * _t1, float _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Algoritam::kraj_animacije()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void Algoritam::error_box(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Algoritam::dodaj_text(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Algoritam::dodaj_anim_cvor(Cvor * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Algoritam::dodaj_anim_ivicu(Ivica * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Algoritam::pomeri_cvor(Cvor * _t1, QPointF _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
