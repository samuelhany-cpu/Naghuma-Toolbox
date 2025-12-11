$mocPath = "C:\Qt\6.7.3\msvc2019_64\bin\moc.exe"

Write-Host "Generating MOC files..."

& $mocPath MainWindow.h -o moc_MainWindow.cpp
Write-Host "- moc_MainWindow.cpp"

& $mocPath ImageCanvas.h -o moc_ImageCanvas.cpp
Write-Host "- moc_ImageCanvas.cpp"

& $mocPath HistogramWidget.h -o moc_HistogramWidget.cpp
Write-Host "- moc_HistogramWidget.cpp"

Write-Host "MOC generation complete!"
Get-ChildItem moc_*.cpp
