$qtMoc = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"

Write-Host "Generating MOC for ThresholdingDialog..." -ForegroundColor Cyan

& $qtMoc -IC:\Qt\6.7.3\msvc2019_64\include `
         -IC:\Qt\6.7.3\msvc2019_64\include\QtCore `
         -IC:\Qt\6.7.3\msvc2019_64\include\QtGui `
         -IC:\Qt\6.7.3\msvc2019_64\include\QtWidgets `
         -IC:\OpenCV\build\include `
         -Iinclude `
         -DQT_WIDGETS_LIB `
         -DQT_GUI_LIB `
         -DQT_CORE_LIB `
         include\ThresholdingDialog.h `
         -o src\moc_ThresholdingDialog.cpp

if (Test-Path "src\moc_ThresholdingDialog.cpp") {
    $size = (Get-Item "src\moc_ThresholdingDialog.cpp").Length
    Write-Host "? MOC generated successfully ($size bytes)" -ForegroundColor Green
} else {
    Write-Host "? MOC generation failed" -ForegroundColor Red
}
