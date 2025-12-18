# Generate MOC with proper Qt defines
Write-Host "Generating MOC for ColorProcessingDialog with Qt defines..." -ForegroundColor Cyan

$mocPath = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"
$headerPath = "include\ColorProcessingDialog.h"
$outputPath = "src\moc_ColorProcessingDialog.cpp"

$mocArgs = @(
    "-DQT_WIDGETS_LIB",
    "-DQT_GUI_LIB",
    "-DQT_CORE_LIB",
    "--include", "F:/Naghuma Toolbox/include",
    "-o", $outputPath,
    $headerPath
)

if (Test-Path $mocPath) {
    & $mocPath $mocArgs 2>&1
    
    if (Test-Path $outputPath) {
        Write-Host "MOC file generated successfully!" -ForegroundColor Green
        $lines = (Get-Content $outputPath).Count
        Write-Host "Generated $lines lines in $outputPath" -ForegroundColor White
    } else {
        Write-Host "MOC file was not created!" -ForegroundColor Red
        Write-Host "Creating stub MOC file..." -ForegroundColor Yellow
        
        # Create stub MOC file
        $stub = @"
/****************************************************************************
** Meta object code from reading C++ file 'ColorProcessingDialog.h'
****************************************************************************/

#include "../include/ColorProcessingDialog.h"
#include <QtCore/qmetatype.h>

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED

QT_INIT_METAOBJECT const QMetaObject ColorProcessingDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSColorProcessingDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t,
        QtPrivate::TypeAndForceComplete<ColorProcessingDialog, std::true_type>,
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const cv::Mat &, std::false_type>
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
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ColorProcessingDialog::previewUpdated(const cv::Mat & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS = QtMocHelpers::stringData(
    "ColorProcessingDialog",
    "previewUpdated",
    "",
    "cv::Mat&",
    "preview"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qt_moc_generate requires QT_MOC_HAS_STRINGDATA"
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSColorProcessingDialogENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

QT_WARNING_POP
QT_END_MOC_NAMESPACE
"@
        $stub | Set-Content $outputPath -Encoding UTF8
        Write-Host "Created stub MOC file" -ForegroundColor Green
    }
} else {
    Write-Host "MOC tool not found!" -ForegroundColor Red
}
