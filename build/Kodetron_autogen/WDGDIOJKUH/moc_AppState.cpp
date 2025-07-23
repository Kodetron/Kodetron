/****************************************************************************
** Meta object code from reading C++ file 'AppState.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Global/AppState.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppState.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN8AppStateE_t {};
} // unnamed namespace

template <> constexpr inline auto AppState::qt_create_metaobjectdata<qt_meta_tag_ZN8AppStateE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppState",
        "selectedExplorerPathModified",
        "",
        "new_path",
        "std::string",
        "path_type",
        "selectedDirPathModified",
        "selectedFilePathModified",
        "setSelectedExplorerPath",
        "new_path_type",
        "setSelectedDirPath",
        "setSelectedFilePath"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectedExplorerPathModified'
        QtMocHelpers::SignalData<void(const QString &, const std::string &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { 0x80000000 | 4, 5 },
        }}),
        // Signal 'selectedDirPathModified'
        QtMocHelpers::SignalData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'selectedFilePathModified'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'setSelectedExplorerPath'
        QtMocHelpers::SlotData<void(const QString &, const std::string &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { 0x80000000 | 4, 9 },
        }}),
        // Slot 'setSelectedDirPath'
        QtMocHelpers::SlotData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'setSelectedFilePath'
        QtMocHelpers::SlotData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AppState, qt_meta_tag_ZN8AppStateE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AppState::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8AppStateE_t>.metaTypes,
    nullptr
} };

void AppState::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppState *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectedExplorerPathModified((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        case 1: _t->selectedDirPathModified((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->selectedFilePathModified((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->setSelectedExplorerPath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::string>>(_a[2]))); break;
        case 4: _t->setSelectedDirPath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->setSelectedFilePath((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppState::*)(const QString & , const std::string & )>(_a, &AppState::selectedExplorerPathModified, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)(const QString & )>(_a, &AppState::selectedDirPathModified, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppState::*)(const QString & )>(_a, &AppState::selectedFilePathModified, 2))
            return;
    }
}

const QMetaObject *AppState::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppState::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8AppStateE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppState::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AppState::selectedExplorerPathModified(const QString & _t1, const std::string & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void AppState::selectedDirPathModified(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void AppState::selectedFilePathModified(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
