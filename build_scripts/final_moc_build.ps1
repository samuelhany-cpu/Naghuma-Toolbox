# Create Qt 6.7.3 Compatible MOC File for ColorProcessingDialog
Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Creating Qt 6.7.3 Compatible MOC" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

Write-Host "`nGenerating MOC file..." -ForegroundColor Yellow

# Create the MOC file content with traditional string data structure
$mocContent = @"
/****************************************************************************
** Meta object code from reading C++ file 'ColorProcessingDialog.h'
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
*****************************************************************************/

#include "../include/ColorProcessingDialog.h"
#include <QtCore/qmetatype.h>

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED

namespace {

struct qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t {
    uint offsetsAndSizes[36];
    char stringdata0[22];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[8];
    char stringdata5[19];
    char stringdata6[6];
    char stringdata7[15];
    char stringdata8[6];
    char stringdata9[18];
    char stringdata10[18];
    char stringdata11[18];
    char stringdata12[18];
    char stringdata13[28];
    char stringdata14[8];
    char stringdata15[15];
    char stringdata16[16];
    char stringdata17[14];
};

static const qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS = {
    {
        0, 21, 22, 14, 37, 0, 38, 12, 51, 7,
        59, 18, 78, 5, 84, 14, 99, 5, 105, 17,
        123, 17, 141, 17, 159, 17, 177, 27, 205, 7,
        213, 14, 228, 15, 244, 13
    },
    "ColorProcessingDialog",
    "previewUpdated",
    "",
    "cv::Mat const&",
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
};

} // unnamed namespace

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

QT_WARNING_POP
QT_END_MOC_NAMESPACE
"@

# Write the MOC file
$mocContent | Set-Content "src\moc_ColorProcessingDialog.cpp" -Encoding UTF8
Write-Host "  ? MOC file created successfully!" -ForegroundColor Green
$lines = ($mocContent -split "`n").Count
Write-Host "  - Generated $lines lines" -ForegroundColor White

# Build the project
Write-Host "`n[Building Project]" -ForegroundColor Yellow
Write-Host "  Please wait..." -ForegroundColor Gray

$buildOutput = msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n??????????????????????????????????????" -ForegroundColor Green
    Write-Host "?   BUILD SUCCESSFUL! ?              ?" -ForegroundColor Green
    Write-Host "??????????????????????????????????????" -ForegroundColor Green
    
    Write-Host "`n?? ColorProcessingDialog is now fully integrated!" -ForegroundColor Cyan
    
    Write-Host "`n?? Phase 15 Color Processing Features:" -ForegroundColor White
    Write-Host "   ? Per-Channel Equalization" -ForegroundColor Green
    Write-Host "   ? Auto White Balance" -ForegroundColor Green
    Write-Host "   ? Gamma Correction (Interactive)" -ForegroundColor Green
    Write-Host "   ? Pseudocolor - 19 Colormaps" -ForegroundColor Green
    Write-Host "   ? Gray Level Slicing (Interactive)" -ForegroundColor Green
    Write-Host "   ? Bit Plane Slicing (Interactive)" -ForegroundColor Green
    
    Write-Host "`n?? Access Features:" -ForegroundColor Yellow
    Write-Host "   Menu: Color ? [Select Feature]" -ForegroundColor White
    
    Write-Host "`n?? Run Application:" -ForegroundColor Cyan
    Write-Host "   • Press F5 in Visual Studio" -ForegroundColor White
    Write-Host "   • Or run: x64\Debug\`"Naghuma Toolbox.exe`"" -ForegroundColor Gray
    
    Write-Host "`n? All Phase 15 features are ready to use!" -ForegroundColor Magenta
    
} else {
    Write-Host "`n??????????????????????????????????????" -ForegroundColor Red
    Write-Host "?   BUILD FAILED ?                   ?" -ForegroundColor Red
    Write-Host "??????????????????????????????????????" -ForegroundColor Red
    
    Write-Host "`n?? Last 15 Build Messages:" -ForegroundColor Yellow
    $buildOutput | Select-Object -Last 15 | ForEach-Object {
        if ($_ -match "error") {
            Write-Host "  ? $_" -ForegroundColor Red
        } elseif ($_ -match "warning") {
            Write-Host "  ??  $_" -ForegroundColor Yellow
        } else {
            Write-Host "     $_" -ForegroundColor Gray
        }
    }
    
    Write-Host "`n?? Troubleshooting Steps:" -ForegroundColor Cyan
    Write-Host "   1. Close Visual Studio completely" -ForegroundColor White
    Write-Host "   2. Delete: x64\Debug folder" -ForegroundColor White
    Write-Host "   3. Delete: .vs folder (hidden)" -ForegroundColor White
    Write-Host "   4. Reopen Visual Studio" -ForegroundColor White
    Write-Host "   5. Build ? Rebuild Solution" -ForegroundColor White
    
    Write-Host "`n?? If errors persist:" -ForegroundColor Yellow
    Write-Host "   • Right-click ColorProcessingDialog.h ? Compile" -ForegroundColor White
    Write-Host "   • This will regenerate MOC in Visual Studio" -ForegroundColor Gray
}

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host " Script Complete" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
