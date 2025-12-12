# Rebuild script - applies all fixes
Write-Host "Building with automatic chaining enabled..."

# Regenerate MOC
C:\Qt\6.7.3\msvc2019_64\bin\moc.exe include\MainWindow.h -o src\moc_MainWindow.cpp

# Build
msbuild "Naghuma Toolbox.sln" /p:Configuration=Debug /p:Platform=x64 /t:Build /m:1 /nologo

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n? Build successful!" -ForegroundColor Green
    Write-Host "`nAutomatic chaining is now enabled:"
    Write-Host "- Grayscale -> automatically becomes current"
    Write-Host "- Then apply Laplacian -> uses grayscale version"
    Write-Host "- Each operation chains to the next automatically"
} else {
    Write-Host "`n? Build failed - see errors above" -ForegroundColor Red
}
