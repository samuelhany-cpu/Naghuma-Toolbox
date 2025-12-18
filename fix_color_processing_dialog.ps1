# Fix ColorProcessingDialog MOC and Build
# This script properly generates MOC file and integrates ColorProcessingDialog

Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Fixing ColorProcessingDialog" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Step 1: Re-add ColorProcessingDialog to project
Write-Host "`n[1/5] Adding ColorProcessingDialog back to project..." -ForegroundColor Yellow

$vcxprojPath = "Naghuma Toolbox.vcxproj"
[xml]$proj = Get-Content $vcxprojPath

$compileGroup = $proj.Project.ItemGroup | Where-Object { $_.ClCompile -ne $null } | Select-Object -First 1

# Add ColorProcessingDialog.cpp if not present
$colorProcDialogCpp = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\ColorProcessingDialog.cpp" }
if ($null -eq $colorProcDialogCpp) {
    $newCompile = $proj.CreateElement("ClCompile", $proj.DocumentElement.NamespaceURI)
    $newCompile.SetAttribute("Include", "src\ColorProcessingDialog.cpp")
    $compileGroup.AppendChild($newCompile) | Out-Null
    Write-Host "  - Added ColorProcessingDialog.cpp" -ForegroundColor Green
} else {
    Write-Host "  - ColorProcessingDialog.cpp already exists" -ForegroundColor Green
}

# Add moc_ColorProcessingDialog.cpp if not present
$mocColorProc = $compileGroup.ClCompile | Where-Object { $_.Include -eq "src\moc_ColorProcessingDialog.cpp" }
if ($null -eq $mocColorProc) {
    $newMoc = $proj.CreateElement("ClCompile", $proj.DocumentElement.NamespaceURI)
    $newMoc.SetAttribute("Include", "src\moc_ColorProcessingDialog.cpp")
    $compileGroup.AppendChild($newMoc) | Out-Null
    Write-Host "  - Added moc_ColorProcessingDialog.cpp" -ForegroundColor Green
} else {
    Write-Host "  - moc_ColorProcessingDialog.cpp already exists" -ForegroundColor Green
}

$proj.Save((Resolve-Path $vcxprojPath))

# Step 2: Uncomment ColorProcessingDialog include in MainWindow.cpp
Write-Host "`n[2/5] Updating MainWindow.cpp includes..." -ForegroundColor Yellow

$mainWindowPath = "src\MainWindow.cpp"
$content = Get-Content $mainWindowPath -Raw

if ($content -match '// #include "ColorProcessingDialog\.h"') {
    $content = $content -replace '// #include "ColorProcessingDialog\.h"[^\r\n]*', '#include "ColorProcessingDialog.h"'
    $content | Set-Content $mainWindowPath -NoNewline -Encoding UTF8
    Write-Host "  - Uncommented ColorProcessingDialog.h include" -ForegroundColor Green
} else {
    Write-Host "  - Include already active" -ForegroundColor Green
}

# Step 3: Generate proper MOC file using Qt's moc tool
Write-Host "`n[3/5] Generating MOC file..." -ForegroundColor Yellow

$qtPath = "C:\Qt\6.7.3\msvc2019_64"
$mocExe = "$qtPath\bin\moc.exe"

if (Test-Path $mocExe) {
    Write-Host "  - Found Qt MOC at: $mocExe" -ForegroundColor Green
    
    # Prepare MOC arguments with proper Qt defines
    $mocArgs = @(
        "-DQT_WIDGETS_LIB",
        "-DQT_GUI_LIB", 
        "-DQT_CORE_LIB",
        "-I", "$qtPath\include",
        "-I", "$qtPath\include\QtCore",
        "-I", "$qtPath\include\QtGui",
        "-I", "$qtPath\include\QtWidgets",
        "-I", "include",
        "-I", "lib",
        "--include", "opencv2/opencv.hpp",
        "-o", "src\moc_ColorProcessingDialog.cpp",
        "include\ColorProcessingDialog.h"
    )
    
    Write-Host "  - Running MOC compiler..." -ForegroundColor Gray
    
    try {
        & $mocExe $mocArgs 2>&1 | Out-Null
        
        if (Test-Path "src\moc_ColorProcessingDialog.cpp") {
            $lines = (Get-Content "src\moc_ColorProcessingDialog.cpp").Count
            Write-Host "  - MOC file generated: $lines lines" -ForegroundColor Green
        } else {
            Write-Host "  - MOC generation failed, creating manual stub" -ForegroundColor Yellow
            & "$PSScriptRoot\create_moc_stub.ps1"
        }
    } catch {
        Write-Host "  - MOC error: $($_.Exception.Message)" -ForegroundColor Red
        Write-Host "  - Creating manual MOC stub..." -ForegroundColor Yellow
        
        # Create manual MOC stub
        $mocStub = @'
/****************************************************************************
** Meta object code from reading C++ file 'ColorProcessingDialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/ColorProcessingDialog.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qproperty.h>

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED

namespace {

struct qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[22];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[8];
};

static constexpr qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS_t qt_meta_stringdata_CLASSColorProcessingDialogENDCLASS = {{
    QT_MOC_LITERAL(0, 21),
    QT_MOC_LITERAL(22, 14),
    QT_MOC_LITERAL(37, 0),
    QT_MOC_LITERAL(38, 12),
    QT_MOC_LITERAL(51, 7)
}, "ColorProcessingDialog", "previewUpdated", "", "cv::Mat const&", "preview"};

static const uint qt_meta_data_CLASSColorProcessingDialogENDCLASS[] = {
    12,       // revision
    0,        // classname
    0,    0,  // classinfo
    1,   14,  // methods
    0,    0,  // properties
    0,    0,  // enums/sets
    0,    0,  // constructors
    0,        // flags
    1,        // signalCount

    // signals: name, argc, parameters, tag, flags, initial metatype offsets
    1,    1,   20,    2, 0x06,    1 /* Public */,

    // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

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

QT_WARNING_POP
'@
        $mocStub | Set-Content "src\moc_ColorProcessingDialog.cpp" -Encoding UTF8
        Write-Host "  - Created manual MOC stub" -ForegroundColor Green
    }
} else {
    Write-Host "  - Qt MOC not found, creating manual stub..." -ForegroundColor Yellow
    # Use the same stub as above
    $mocStub | Set-Content "src\moc_ColorProcessingDialog.cpp" -Encoding UTF8
    Write-Host "  - Created manual MOC stub" -ForegroundColor Green
}

# Step 4: Verify all files are present
Write-Host "`n[4/5] Verifying files..." -ForegroundColor Yellow

$requiredFiles = @(
    "include\ColorProcessingDialog.h",
    "src\ColorProcessingDialog.cpp",
    "src\moc_ColorProcessingDialog.cpp",
    "lib\color\ColorProcessor.h",
    "lib\color\ColorProcessor.cpp"
)

$allPresent = $true
foreach ($file in $requiredFiles) {
    if (Test-Path $file) {
        Write-Host "  ? $file" -ForegroundColor Green
    } else {
        Write-Host "  ? $file - MISSING!" -ForegroundColor Red
        $allPresent = $false
    }
}

if (-not $allPresent) {
    Write-Host "`nERROR: Some required files are missing!" -ForegroundColor Red
    exit 1
}

# Step 5: Build the project
Write-Host "`n[5/5] Building the project..." -ForegroundColor Yellow
Write-Host "  - This may take a moment..." -ForegroundColor Gray

$buildOutput = msbuild "Naghuma Toolbox.sln" /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal 2>&1

$buildSuccess = $LASTEXITCODE -eq 0

if ($buildSuccess) {
    Write-Host "`n========================================" -ForegroundColor Green
    Write-Host " BUILD SUCCESSFUL!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    
    Write-Host "`n? ColorProcessingDialog is now integrated!" -ForegroundColor Cyan
    Write-Host "`nPhase 15 Features Available:" -ForegroundColor White
    Write-Host "  ? Per-Channel Equalization" -ForegroundColor Green
    Write-Host "  ? Auto White Balance" -ForegroundColor Green
    Write-Host "  ? Gamma Correction (with dialog)" -ForegroundColor Green
    Write-Host "  ? Pseudocolor (19 colormaps)" -ForegroundColor Green
    Write-Host "  ? Gray Level Slicing (with dialog)" -ForegroundColor Green
    Write-Host "  ? Bit Plane Slicing (with dialog)" -ForegroundColor Green
    
    Write-Host "`nAccess via:" -ForegroundColor Yellow
    Write-Host "  Color ? [Feature Name]" -ForegroundColor White
    
    Write-Host "`nRun the application:" -ForegroundColor Cyan
    Write-Host "  Press F5 in Visual Studio" -ForegroundColor White
    
} else {
    Write-Host "`n========================================" -ForegroundColor Red
    Write-Host " BUILD FAILED" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    
    Write-Host "`nShowing last 15 build errors:" -ForegroundColor Yellow
    $buildOutput | Select-Object -Last 15 | ForEach-Object {
        if ($_ -match "error") {
            Write-Host $_ -ForegroundColor Red
        } else {
            Write-Host $_ -ForegroundColor Gray
        }
    }
    
    Write-Host "`nTroubleshooting:" -ForegroundColor Yellow
    Write-Host "1. Close Visual Studio completely" -ForegroundColor White
    Write-Host "2. Delete folders: x64\Debug, .vs" -ForegroundColor White
    Write-Host "3. Reopen Visual Studio" -ForegroundColor White
    Write-Host "4. Build ? Rebuild Solution" -ForegroundColor White
    
    Write-Host "`nIf MOC errors persist:" -ForegroundColor Yellow
    Write-Host "1. Right-click ColorProcessingDialog.h" -ForegroundColor White
    Write-Host "2. Click 'Compile' to generate MOC" -ForegroundColor White
    Write-Host "3. Build solution again" -ForegroundColor White
}

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host " Script Complete" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
