@echo off
REM Set Tesseract environment variables
set TESSDATA_PREFIX=C:\Program Files\Tesseract-OCR\tessdata
set PATH=F:\Naghuma Toolbox\vcpkg\installed\x64-windows\bin;%PATH%

echo Tesseract environment configured
echo TESSDATA_PREFIX=%TESSDATA_PREFIX%
echo.

REM Run the application
"%~dp0x64\Debug\Naghuma Toolbox.exe"
