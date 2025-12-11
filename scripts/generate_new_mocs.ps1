# Generate MOC files for new modular Qt classes

$qtPath = "C:\Qt\6.7.3\msvc2019_64\bin"
$mocExe = "$qtPath\moc.exe"

if (-not (Test-Path $mocExe)) {
    Write-Host "? MOC not found at: $mocExe" -ForegroundColor Red
    Write-Host "Please update the Qt path in this script" -ForegroundColor Yellow
    exit 1
}

Write-Host "?? Generating MOC files for new classes..." -ForegroundColor Cyan

# MOC files to generate
$mocFiles = @(
    @{Header="include\RightSidebarWidget.h"; Output="src\moc_RightSidebarWidget.cpp"},
    @{Header="include\LayerManager.h"; Output="src\moc_LayerManager.cpp"},
    @{Header="include\TransformDialog.h"; Output="src\moc_TransformDialog.cpp"}
)

foreach ($file in $mocFiles) {
    $header = $file.Header
    $output = $file.Output
    
    if (Test-Path $header) {
        Write-Host "  Generating $output..." -ForegroundColor Yellow
        & $mocExe $header -o $output
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "  ? $output created" -ForegroundColor Green
        } else {
            Write-Host "  ? Failed to generate $output" -ForegroundColor Red
        }
    } else {
        Write-Host "  ?? Header not found: $header" -ForegroundColor Yellow
    }
}

Write-Host "`n? MOC generation complete!" -ForegroundColor Green
