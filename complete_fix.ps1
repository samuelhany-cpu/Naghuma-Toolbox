# Complete Fix Script - Phase 15
Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Phase 15: Complete Fix Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Create minimal MOC stub for ColorProcessingDialog
Write-Host "`n[1/2] Creating MOC stub for ColorProcessingDialog..." -ForegroundColor Yellow

$mocStub = @'
// Minimal MOC stub for ColorProcessingDialog
#include "../include/ColorProcessingDialog.h"
#include <QtCore/qobject.h>

struct qt_meta_stringdata_ColorProcessingDialog_t {
    const uint offsetsAndSize[4];
    char stringdata0[22 + 15 + 1 + 1];
};

static const qt_meta_stringdata_ColorProcessingDialog_t qt_meta_stringdata_ColorProcessingDialog = {
    {
        0, 21, 22, 14
    },
    "ColorProcessingDialog\0previewUpdated\0\0cv::Mat&"
};

static const uint qt_meta_data_ColorProcessingDialog[] = {
    12,       // revision
    0,        // classname
    0,    0,  // classinfo
    1,   14,  // methods
    0,    0,  // properties
    0,    0,  // enums/sets
    0,    0,  // constructors
    0,        // flags
    1,        // signalCount
    1,    1,   20,    2, 0x06, 1,
    QMetaType::Void, QMetaType::UnknownType, 3,
    0        
};

void ColorProcessingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ColorProcessingDialog *>(_o);
        switch (_id) {
        case 0: _t->previewUpdated((*reinterpret_cast<const cv::Mat*>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ColorProcessingDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ColorProcessingDialog.offsetsAndSize,
    qt_meta_data_ColorProcessingDialog,
    qt_static_metacall,
    nullptr,
    nullptr,
    nullptr
} };

const QMetaObject *ColorProcessingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColorProcessingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ColorProcessingDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ColorProcessingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

void ColorProcessingDialog::previewUpdated(const cv::Mat & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
'@

$mocStub | Set-Content "src\moc_ColorProcessingDialog.cpp" -Encoding UTF8
Write-Host "  - Created moc_ColorProcessingDialog.cpp" -ForegroundColor Green

# Build the project
Write-Host "`n[2/2] Building the project..." -ForegroundColor Yellow
msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n========================================" -ForegroundColor Green
    Write-Host " BUILD SUCCESSFUL!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "`nYou can now run the application!" -ForegroundColor Cyan
} else {
    Write-Host "`n========================================" -ForegroundColor Red
    Write-Host " BUILD FAILED!" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "Check the errors above." -ForegroundColor Yellow
}
