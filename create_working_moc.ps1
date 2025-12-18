# Create Complete Working MOC File for ColorProcessingDialog
Write-Host "Creating complete MOC file..." -ForegroundColor Cyan

$mocContent = @'
/****************************************************************************
** Meta object code from reading C++ file 'ColorProcessingDialog.h'
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
*****************************************************************************/

#include "../include/ColorProcessingDialog.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qproperty.h>

QT_BEGIN_MOC_NAMESPACE

// STRINGDATA
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS = QtMocHelpers::stringData(
    "ColorProcessingDialog",
    "previewUpdated",
    "",
    "cv::Mat&",
    "preview",
    "onOperationChanged",
    "index",
    "onGammaChanged",
    "value",
    "onColormapChanged",
    "onMinLevelChanged",
    "onMaxLevelChanged",
    "onBitPlaneChanged",
    "onPreserveBackgroundToggled",
    "checked",
    "onApplyClicked",
    "onCancelClicked",
    "updatePreview"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or qt_version_tag not defined"
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

// META DATA
Q_CONSTINIT static const uint qt_meta_data_CLASSColorProcessingDialogENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   83,    2, 0x08,    3 /* Private */,
       7,    1,   86,    2, 0x08,    5 /* Private */,
       9,    1,   89,    2, 0x08,    7 /* Private */,
      10,    1,   92,    2, 0x08,    9 /* Private */,
      11,    1,   95,    2, 0x08,   11 /* Private */,
      12,    1,   98,    2, 0x08,   13 /* Private */,
      13,    1,  101,    2, 0x08,   15 /* Private */,
      15,    0,  104,    2, 0x08,   17 /* Private */,
      16,    0,  105,    2, 0x08,   18 /* Private */,
      17,    0,  106,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

// METACALL
void ColorProcessingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorProcessingDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->previewUpdated((*reinterpret_cast< std::add_pointer_t<cv::Mat const&>>(_a[1]))); break;
        case 1: _t->onOperationChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onGammaChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 3: _t->onColormapChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onMinLevelChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onMaxLevelChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onBitPlaneChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onPreserveBackgroundToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->onApplyClicked(); break;
        case 9: _t->onCancelClicked(); break;
        case 10: _t->updatePreview(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ColorProcessingDialog::*)(const cv::Mat & );
            if (_t _q_method = &ColorProcessingDialog::previewUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

// METAOBJECT
const QMetaObject *ColorProcessingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorProcessingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ColorProcessingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ColorProcessingDialog::previewUpdated(const cv::Mat & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// STATIC METAOBJECT
Q_CONSTINIT const QMetaObject ColorProcessingDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSColorProcessingDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t,
        QtPrivate::TypeAndForceComplete<ColorProcessingDialog, std::true_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const cv::Mat &, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_POP

QT_END_MOC_NAMESPACE
'@

$mocContent | Set-Content "src\moc_ColorProcessingDialog.cpp" -Encoding UTF8
Write-Host "MOC file created successfully!" -ForegroundColor Green

# Now build
Write-Host "`nBuilding project..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n? BUILD SUCCESSFUL!" -ForegroundColor Green
    Write-Host "ColorProcessingDialog is now fully integrated!" -ForegroundColor Cyan
} else {
    Write-Host "`n? Build failed. Check errors above." -ForegroundColor Red
}
'@
$mocContent | Set-Content "create_working_moc.ps1" -Encoding UTF8

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "create_working_moc.ps1"
