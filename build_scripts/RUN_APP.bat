@echo off
REM Naghuma Toolbox Launcher (Batch wrapper for PowerShell script)
REM Double-click this file to run the application

powershell.exe -ExecutionPolicy Bypass -File "%~dp0run_naghuma_toolbox.ps1"
pause
